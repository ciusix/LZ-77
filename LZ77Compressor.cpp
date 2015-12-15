#include "LZ77Compressor.h"

/*

File structure:
|00000000|00000000|00..00|
 ^        ^        ^
 |        |        |
 |        |        +- Compressed data
 |        +- Active Size (8 bits)
 +- History Size (8 bits)

Every entry look like this:
|0 00000000| - letter (1 + 8 bits)
|1 000..000| - reference (1 + HistorySize + ActiveSize bits)

Reference look like this:
|1|00..00|00..00|
 ^ ^      ^
 | |      |
 | |      +- Length of substring (ActiveSize bits)
 | +- Distance (HistorySize bits)
 +- Type (1 bit)

*/

LZ77Compressor::LZ77Compressor(std::string inputFileName, std::string outputFileName, u_int historySize, u_int activeSize)
    :LZ77CompressorDecompressorBase(inputFileName, outputFileName) {

    this->historySize = historySize;
    this->activeSize = activeSize;

    minimalMatchLength = getMinimalMatchLength(historySize, activeSize);

    this->historyLength = pow(2, historySize) - 1;
    this->activeLength = pow(2, activeSize);
    
    this->historySlider = "";
    this->activeSlider = "";

    this->activeLength += minimalMatchLength - 1;
}

void LZ77Compressor::compress() {
    prepareInputFile();
    prepareOutputFile();
    writeMetaData();
    initializeActiveSlider();

    possiblePositions = new int[historyLength];

    magic();
    closeInputFile();
    closeOutputFile();
}

void LZ77Compressor::writeMetaData() {
    #ifdef DEBUG_MODE
    CONSOLE << "History size: " << historySize << " length: " << historyLength + 1 << " needed reference bits " << historySize << ENDL;
    CONSOLE << "Active  size: " <<  activeSize << " length: " <<  activeLength << " needed reference bits " <<  activeSize << ENDL;
    CONSOLE << "Minimum match length " << minimalMatchLength << ENDL;
    #endif

    for (int i = 7; i >= 0; i--) {
        writeABitToFile(historySize >> i);
    }
    for (int i = 7; i >= 0; i--) {
        writeABitToFile(activeSize >> i);
    }
}

void LZ77Compressor::magic() {
    addLetterToOutput(activeSlider.at(0));

    struct Reference* m;
    bool finish = false;

    while (!finish) {
        finish = !moveSliders();

        if (finish) {
            break;
        }

        m = checkForLongestMatch(); 
        if (m != NULL) {
            addReferenceToOutput(historySlider.length() - m->position, m->length);
            finish = !moveSliders(m->length - 1);
            free(m);
        } else {
            addLetterToOutput(activeSlider.at(0));
        }

        if (finish) {
            break;
        }
    }

    // Flush last bits to file
    writeABitToFile(0, true);
}

bool LZ77Compressor::moveSliders() {
    return moveSliders(1);
}

bool LZ77Compressor::moveSliders(u_int distance) {
    historySlider += activeSlider.substr(0, distance);
    activeSlider.erase(0,distance);

    char c;
    char flag = CHAR_READ_SUCC;

    for (u_int i = 0; i < distance; i++) {
        c = readChar(&flag);
        if (flag != CHAR_READ_FAIL) {
            activeSlider += c;
        }
    }

    if (historySlider.length() >= historyLength) {
        historySlider.erase(0, historyLength - historySlider.length());
    }

    return activeSlider.length() != 0;
}

/*
LZ77Compressor::Reference* LZ77Compressor::checkForLongestMatch() {
    std::size_t position;
    std::string activeSliderCropped;
    for (u_int i = activeSlider.length(); i > minimalMatchLength; i--) {
        
        activeSliderCropped = activeSlider.substr(0, i);  
        position = historySlider.find(activeSliderCropped);

        if (position != std::string::npos) {
            struct Reference *m = new LZ77Compressor::Reference();
            m->position = position;
            m->length = i;
            return m;
        }
        
    }
    return NULL;
}
*/

/*
LZ77Compressor::Reference* LZ77Compressor::checkForLongestMatch() {
    static u_int historySliderLength, activeSliderLength;
    historySliderLength = historySlider.length();
    activeSliderLength = activeSlider.length();

    static u_int possibilities;
    possibilities = historySliderLength;

    static int resultPos, resultLen;
    resultPos = -1;
    resultLen = -1;
    
    static u_int i, length, hPos;

    static char chr;
    
    static u_int hPosMinLength, historySliderLengthPlusLength;

    static int first;

    for (i = 0; i < historySliderLength; i++) {
        possiblePositions[i] = 1;
    }
        
    for (length = 0; length < activeSliderLength; length++) {
        chr = activeSlider[length];
        historySliderLengthPlusLength = historySliderLength + length;
        first = 1;
        for (hPos = length; hPos < historySliderLengthPlusLength; hPos++) {
            hPosMinLength = hPos - length;

            if (possiblePositions[hPosMinLength]) {
                if (chr == historySlider[hPos] && (hPos < historySliderLength)) {
                    // Nothing to do

                    if (first) {
                        first = 0;
                        resultPos = hPos;
                        resultLen = length;
                    }
                } else {
                    possiblePositions[hPosMinLength] = 0;
                    possibilities--;

                    if (possibilities <= 0) {
                        break;
                    }
                }
            }
        }
        if (possibilities <= 0) {
            break;
        }
    }

    if (length >= minimalMatchLength) {
        struct LZ77Compressor::Reference *m = new LZ77Compressor::Reference();
        m->position = resultPos;
        m->length = resultLen;
        return m;
    }
        
    return NULL;
}
*/

LZ77Compressor::Reference* LZ77Compressor::checkForLongestMatch() {
    static u_int historySliderLength, activeSliderLength;
    historySliderLength = historySlider.length();
    activeSliderLength = activeSlider.length();

    static u_int ii, jj;
    static u_int numberOfPossiblePositions;
    numberOfPossiblePositions = 0;

    static char chr;
    chr = activeSlider[0];

    static u_int maxLength, resultPos, resultLen, kk;
    maxLength = 0;

    for (ii = 0; ii < historySliderLength; ii++) {
        if (chr == historySlider[ii]) {
            possiblePositions[numberOfPossiblePositions] = ii;
            numberOfPossiblePositions++;
        }
    }

    for (ii = 0; ii < numberOfPossiblePositions; ii++) {
        for (jj = 1; jj < activeSliderLength; jj++) {
            kk = jj + possiblePositions[ii];
            if (activeSlider[jj] != historySlider[kk] || kk >= historySliderLength) {
                break;
            }
        }
        if (jj > maxLength) {
            maxLength = jj;
            resultPos = possiblePositions[ii];
            resultLen = jj;
        }
    }

    if (maxLength >= minimalMatchLength) {
        struct LZ77Compressor::Reference *m = new LZ77Compressor::Reference();
        m->position = resultPos;
        m->length = resultLen;
        return m;
    }

    return NULL;
}

void LZ77Compressor::initializeActiveSlider() {
    char c = '\0';
    char flag = CHAR_READ_SUCC;
    for (u_int i = 0; i < activeLength; i++) {
        c = readChar(&flag);
        if (flag != CHAR_READ_SUCC) {
            CONSOLE<< "Error reading file. Cannot initialize active slider." << ENDL;
            exit(1);
        }
        activeSlider += c;
    }
}

void LZ77Compressor::addLetterToOutput(char letter) {
    // CONSOLE << "Letter   " << letter << ENDL;

    writeABitToFile(BIT_CHAR);
    for (int i = 7; i >= 0; i--) {
        writeABitToFile(letter >> i);
    }
}

void LZ77Compressor::addReferenceToOutput(u_int position, u_int length) {
    // CONSOLE << "Refrence " << position << " " << length << " (" << historySlider.substr(historySlider.length() - position, length) << ")" << ENDL;

    length -= minimalMatchLength;

    writeABitToFile(BIT_REFERENCE);
    for (int i = historySize - 1; i >= 0; i--) {
        writeABitToFile(position >> i);
    }

    for (int i = activeSize - 1; i >= 0; i--) {
        writeABitToFile(length >> i);
    }
}