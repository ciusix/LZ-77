#ifndef LZ77COMPRESSOR_H
#define LZ77COMPRESSOR_H

#define INVALID_CHAR 0x03
#define u_int unsigned int
#define CONSOLE std::cout
#define ENDL std::endl

#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>

class LZ77Compressor {

struct Reference
{
    u_int position;
    u_int length;
};

    private:
        std::string inputFileName;
        std::ifstream inputFile;
        
        std::string historySlider, activeSlider;
        u_int historySliderLength, activeSliderLength;
        
        void initializeActiveSlider();
        void prepareInputFile();
        void closeInputFile();
        Reference* checkForLongestMatch();
        char readChar();
        void printSliders();
        void moveSliders();
        void moveSliders(u_int);
        void addLetterToOutput(char);
        void addReferenceToOutput(u_int, u_int);

    public:
        LZ77Compressor(std::string);
        
        void compress();
};

#endif