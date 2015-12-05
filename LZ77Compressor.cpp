#include "LZ77Compressor.h"

LZ77Compressor::LZ77Compressor(std::string inputFileName){
    
    this->inputFileName = inputFileName;
    this->historySliderLength = 8;
    this->activeSliderLength = 4;
    
    this->historySlider = "";
    this->activeSlider = "";
}

void LZ77Compressor::compress() {
    prepareInputFile();
    initializeActiveSlider();
    
    printSliders();
    
    for (int i = 0; i < 10; i++) {
        moveSliders();
        printSliders();
    }
    
    closeInputFile();
}

void LZ77Compressor::prepareInputFile() {
    inputFile.open(inputFileName.c_str(), std::ios::binary | std::ios::in);
	
	if (!inputFile.is_open()) {
        std::cout << "Error opening " << inputFileName << ". Quitting...";
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
    activeSlider += readChar();
}

void LZ77Compressor::initializeActiveSlider() {
    char c = '\0';
    for (int i = 0; i < activeSliderLength; i++) {
        c = readChar();
        if (c == INVALID_CHAR) {
            std::cout<< "Error reading file. Cannot initialize active slider." << std::endl;
            exit(1);
        }
        activeSlider += c;
    }
}

void LZ77Compressor::printSliders() {
    std::cout << "|" << historySlider << "|" << activeSlider << "|" << std::endl << " ";
    for (int i = 0; i < historySliderLength; i++) {
        std::cout << i % 10;
    }
    std::cout << " ";
    for (int i = 0; i < activeSliderLength; i++) {
        std::cout << i % 10;
    }
    std::cout << std::endl;
}

char LZ77Compressor::readChar() {
	static char c;
	if (!(inputFile.get(c))) {
		return INVALID_CHAR;
	}
    return c;
}