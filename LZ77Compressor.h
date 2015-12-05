#ifndef LZ77COMPRESSOR_H
#define LZ77COMPRESSOR_H

#define INVALID_CHAR 0x03

#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>

class LZ77Compressor {

    private:
        std::string inputFileName;
        std::ifstream inputFile;
        
        std::string historySlider, activeSlider;
        int historySliderLength, activeSliderLength;
        
        void initializeActiveSlider();
        void prepareInputFile();
        void closeInputFile();
        char readChar();
        void printSliders();
        void moveSliders();
    public:
        LZ77Compressor(std::string);
        
        void compress();
};

#endif