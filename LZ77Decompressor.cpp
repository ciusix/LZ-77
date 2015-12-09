#include "LZ77Decompressor.h"

LZ77Decompressor::LZ77Decompressor(std::string inputFileName, std::string outputFileName){
    
    this->inputFileName = inputFileName;
    this->outputFileName = outputFileName;
    
}

void LZ77Decompressor::prepareInputFile() {
    inputFile.open(inputFileName.c_str(), std::ios::binary | std::ios::in);
    
    if (!inputFile.is_open()) {
        CONSOLE << "Error opening " << inputFileName << ". Quitting...";
        exit(1);
    }

    CONSOLE << "Opened " << inputFileName << ENDL;
}

void LZ77Decompressor::closeInputFile() {
    if (inputFile.is_open()) {
        inputFile.close();

        CONSOLE << "Closed " << inputFileName << ENDL;
    }
}

void LZ77Decompressor::prepareOutputFile() {
    outputFile.open(outputFileName.c_str(), std::ios::binary | std::ios::out | std::ios::app);
    
    if (!outputFile.is_open()) {
        CONSOLE << "Error opening " << outputFileName << ". Quitting...";
        exit(1);
    }

    CONSOLE << "Opened " << outputFileName << ENDL;
}

void LZ77Decompressor::closeOutputFile() {
    if (outputFile.is_open()) {
        outputFile.close();

        CONSOLE << "Closed " << outputFileName << ENDL;
    }
}