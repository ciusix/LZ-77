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
#include <bitset>
#include <math.h>

class LZ77Compressor {

struct Reference
{
    u_int position;
    u_int length;
};

    private:
        std::string inputFileName;
        std::ifstream inputFile;
        std::string outputFileName;
        std::ofstream outputFile;
        
        std::string historySlider, activeSlider;

        u_int historySliderLength, activeSliderLength; // length (letters count)
        u_int historySliderSize, activeSliderSize; // Size 10 = 10 bits = 1024 letters
        u_int referenceHistoryBits, referenceActiveBits; // how many bits are needed to write reference and length (w_1, w_2)
        u_int minimalMatchLength;

        void initializeActiveSlider();
        void prepareInputFile();
        void closeInputFile();
        void prepareOutputFile();
        void closeOutputFile();
        void writeMetaData();
        void magic();
        Reference* checkForLongestMatch();
        char readChar(char*);
        void printSliders();
        bool moveSliders();
        bool moveSliders(u_int);
        void addLetterToOutput(char);
        void addReferenceToOutput(u_int, u_int);
        void writeABitToFile(char);
        void writeABitToFile(char, bool);
        u_int getBytesNeededForNumber(u_int);

    public:
        LZ77Compressor(std::string, std::string, u_int, u_int);
        
        void compress();
};

#endif