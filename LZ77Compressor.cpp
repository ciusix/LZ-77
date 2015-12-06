#include "LZ77Compressor.h"

LZ77Compressor::LZ77Compressor(std::string inputFileName){
    
    this->inputFileName = inputFileName;
    this->historySliderLength = 30;
    this->activeSliderLength = 10;
    
    this->historySlider = "";
    this->activeSlider = "";
}

void LZ77Compressor::compress() {
    prepareInputFile();
    initializeActiveSlider();

    addLetterToOutput(activeSlider.at(0));

    for (u_int i = 0; i < 80; i++) {
        moveSliders();
        // printSliders();
        struct match* m = checkForLongestMatch(); 
        if (m != NULL) {
            // CONSOLE << "Found match at " << m->position << " len " << m->length << ENDL;
            addReferenceToOutput(historySlider.length() - m->position, m->length);

            for (u_int j = 0; j < m->length - 1; j++) {
                moveSliders();
            }
            
            free(m);
        } else {
            addLetterToOutput(activeSlider.at(0));
        }
    }
    
    closeInputFile();
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

void LZ77Compressor::moveSliders() {
    if (historySlider.length() >= historySliderLength) {
        historySlider.erase(0,1);
    }
    historySlider += activeSlider.at(0);
    activeSlider.erase(0,1);
    char c = readChar();

    if (c == INVALID_CHAR) {
        CONSOLE<< "Error reading file. Cannot move slider." << ENDL;
        exit(1);
    }

    activeSlider += c;
}

LZ77Compressor::match* LZ77Compressor::checkForLongestMatch() {
    for (u_int i = activeSlider.length(); i > 2; i--) {
        std::string activeSliderCropped = activeSlider.substr(0, i);
        // CONSOLE << "Check |" << historySlider << "| with |" << activeSlider << "| length " << i << " (" << activeSliderCropped << ")" << ENDL;
        std::size_t position = historySlider.find(activeSliderCropped);

        if (position == std::string::npos) {
            // CONSOLE << "Not found" << ENDL;
        } else {
            // CONSOLE << "Found at position " << position << " length " << i << ENDL;
            struct match *m = new LZ77Compressor::match();
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
    return;
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
}

void LZ77Compressor::addReferenceToOutput(u_int position, u_int length) {
    CONSOLE << "Refrence " << position << " " << length << " (" << historySlider.substr(historySlider.length() - position, length) << ")" << ENDL;
}