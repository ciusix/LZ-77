#include "LZ77CompressorDecompressorBase.h"

LZ77CompressorDecompressorBase::LZ77CompressorDecompressorBase(std::string inputFileName, std::string outputFileName) {
    this->inputFileName = inputFileName;
    this->outputFileName = outputFileName;
}

char LZ77CompressorDecompressorBase::readChar(char *flag) {
    static char c;
    if (!(inputFile.get(c))) {
        (*flag) = CHAR_READ_FAIL;
    }
    return c;
}

void LZ77CompressorDecompressorBase::writeABitToFile(char bit) {
    writeABitToFile(bit, false);
}

void LZ77CompressorDecompressorBase::writeABitToFile(char bit, bool flush) {
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

void LZ77CompressorDecompressorBase::prepareInputFile() {
    inputFile.open(inputFileName.c_str(), std::ios::binary | std::ios::in);
    
    if (!inputFile.is_open()) {
        CONSOLE << "Error opening " << inputFileName << ". Quitting...";
        exit(1);
    }
    CONSOLE << "Opened " << inputFileName << ENDL;
}

void LZ77CompressorDecompressorBase::closeInputFile() {
    if (inputFile.is_open()) {
        inputFile.close();
        CONSOLE << "Closed " << inputFileName << ENDL;
    }
}

void LZ77CompressorDecompressorBase::prepareOutputFile() {
    outputFile.open(outputFileName.c_str(), std::ios::binary | std::ios::out);
    
    if (!outputFile.is_open()) {
        CONSOLE << "Error opening " << outputFileName << ". Quitting...";
        exit(1);
    }
    CONSOLE << "Opened " << outputFileName << ENDL;
}

void LZ77CompressorDecompressorBase::closeOutputFile() {
    if (outputFile.is_open()) {
        outputFile.close();
        CONSOLE << "Closed " << outputFileName << ENDL;
    }
}

char LZ77CompressorDecompressorBase::readABitFromFile(char *flag) {
    static short currentBitNumber = 0;
    static char c;
    if (currentBitNumber == 0) {
        if (!(inputFile.get(c))) {
            (*flag) = CHAR_READ_FAIL;
        }
        currentBitNumber = 7;
    } else {
        currentBitNumber--;
    }
    return (c >> currentBitNumber) & 1;
}

int LZ77CompressorDecompressorBase::getMinimalMatchLength(u_int historyBits, u_int activeBits) {
    for (int i = 1; i < 32; i++) {
        if ((int)(1 + historyBits + activeBits) < i * 9) {
            return i;
        }
    }
    return 32;
}

void LZ77CompressorDecompressorBase::writeChar(char c) {
    outputFile << c;
}