#include "LZ77Compressor.h"

/*

|0 00000000| - letter
|1 000..000| - reference

Reference = Distance | Length

*/

LZ77Compressor::LZ77Compressor(std::string inputFileName, std::string outputFileName){
    
    this->inputFileName = inputFileName;
    this->outputFileName = outputFileName;

    this->historySliderLength = 150;
    this->activeSliderLength = 50;
    
    this->historySlider = "";
    this->activeSlider = "";

    referenceHistoryBits = getBytesNeededForNumber(historySliderLength);
    referenceActiveBits = getBytesNeededForNumber(activeSliderLength);
}

void LZ77Compressor::compress() {
    prepareInputFile();
    prepareOutputFile();
    initializeActiveSlider();
    magic();
    closeInputFile();
    closeOutputFile();
}

void LZ77Compressor::magic() {
    addLetterToOutput(activeSlider.at(0));

    struct Reference* m;

    // TODO Replace constant with something better
    for (u_int i = 1; i > 0; i++) {
        moveSliders();

        m = checkForLongestMatch(); 
        if (m != NULL) {
            addReferenceToOutput(historySlider.length() - m->position, m->length);
            moveSliders(m->length - 1);
            free(m);
        } else {
            addLetterToOutput(activeSlider.at(0));
        }
    }
}

void LZ77Compressor::prepareInputFile() {
    inputFile.open(inputFileName.c_str(), std::ios::binary | std::ios::in);
	
	if (!inputFile.is_open()) {
        CONSOLE << "Error opening " << inputFileName << ". Quitting...";
        exit(1);
    }
}

void LZ77Compressor::closeInputFile() {
	if (inputFile.is_open()) {
        inputFile.close();
    }
}

void LZ77Compressor::prepareOutputFile() {
    outputFile.open(outputFileName.c_str(), std::ios::binary | std::ios::out | std::ios::app);
    
    if (!outputFile.is_open()) {
        CONSOLE << "Error opening " << outputFileName << ". Quitting...";
        exit(1);
    }
}

void LZ77Compressor::closeOutputFile() {
    if (outputFile.is_open()) {
        outputFile.close();
    }
}

void LZ77Compressor::moveSliders() {
    if (historySlider.length() >= historySliderLength) {
        historySlider.erase(0,1);
    }
    historySlider += activeSlider.at(0);
    activeSlider.erase(0,1);

    // TODO maybe make c static?
    char c = readChar();

    if (c == INVALID_CHAR) {
        CONSOLE<< "Error reading file. Cannot move slider." << ENDL;
        exit(1);
    }

    activeSlider += c;
}

void LZ77Compressor::moveSliders(u_int distance) {
    if (distance > historySliderLength) {
        CONSOLE << "distance > historySliderLength" << ENDL;
    }

    if (historySlider.length() >= historySliderLength) {
        historySlider.erase(0,distance);
    }
    historySlider += activeSlider.substr(0, distance);
    activeSlider.erase(0,distance);

    // TODO maybe make c static?
    char c;

    for (u_int i = 0; i < distance; i++) {
        c = readChar();
        if (c == INVALID_CHAR) {
            CONSOLE<< "Error reading file. Cannot move slider." << ENDL;
            exit(1);
        }
        activeSlider += c;
    }
}

LZ77Compressor::Reference* LZ77Compressor::checkForLongestMatch() {
    for (u_int i = activeSlider.length(); i > 2; i--) {
        std::string activeSliderCropped = activeSlider.substr(0, i);
        // CONSOLE << "Check |" << historySlider << "| with |" << activeSlider << "| length " << i << " (" << activeSliderCropped << ")" << ENDL;

        // TODO Move outside loop
        std::size_t position = historySlider.find(activeSliderCropped);

        if (position != std::string::npos) {
            // CONSOLE << "Found at position " << position << " length " << i << ENDL;
            struct Reference *m = new LZ77Compressor::Reference();
            m->position = position;
            m->length = i;
            return m;
        }
    }
    return NULL;
}

void LZ77Compressor::initializeActiveSlider() {
    char c = '\0';
    for (u_int i = 0; i < activeSliderLength; i++) {
        c = readChar();
        if (c == INVALID_CHAR) {
            CONSOLE<< "Error reading file. Cannot initialize active slider." << ENDL;
            exit(1);
        }
        activeSlider += c;
    }
}

void LZ77Compressor::printSliders() {
    CONSOLE << "|" << historySlider << "|" << activeSlider << "|" << ENDL; //  << " ";

    for (u_int i = 0; i < historySliderLength; i++) {
        CONSOLE << i % 10;
    }
    CONSOLE << " ";
    for (u_int i = 0; i < activeSliderLength; i++) {
        CONSOLE << i % 10;
    }
    CONSOLE << ENDL;
}

char LZ77Compressor::readChar() {
	static char c;
	if (!(inputFile.get(c))) {
		return INVALID_CHAR;
	}
    return c;
}

void LZ77Compressor::addLetterToOutput(char letter) {
    CONSOLE << "Letter   " << letter << ENDL;
    writeABitToFile(0);
    for (int i = 7; i >= 0; i--) {
        writeABitToFile(letter >> i);
    }
}

void LZ77Compressor::addReferenceToOutput(u_int position, u_int length) {
    CONSOLE << "Refrence " << position << " " << length << " (" << historySlider.substr(historySlider.length() - position, length) << ")" << ENDL;

    writeABitToFile(0);
    for (int i = referenceHistoryBits - 1; i >= 0; i--) {
        writeABitToFile(position >> i);
    }

    for (int i = referenceActiveBits - 1; i >= 0; i--) {
        writeABitToFile(length >> i);
    }
}

void LZ77Compressor::writeABitToFile(char bit) {
    writeABitToFile(bit, false);
}

void LZ77Compressor::writeABitToFile(char bit, bool flush) {
    static short currentBitNumber = 0;
    static char c = 0;
    
    if (flush) {
        outputFile << ((char)(c << (8 - currentBitNumber)));
    }
        
    if (currentBitNumber == 8) {
        outputFile << c;
        c = 0;
        currentBitNumber = 0;
    } else {
        c = c << 1;
    }
    
    c = (c | (bit & 0x1));  
    currentBitNumber++;
}

u_int LZ77Compressor::getBytesNeededForNumber(u_int number) {
    u_int bound = 1;
    for (int i = 0; i < 32; i++) {
        if (bound >= number) {
            return i;
        }
        bound *= 2;
    } 
    return 32;
}