#ifndef LZ77COMPRESSOR_H
#define LZ77COMPRESSOR_H

#include "LZ77CompressorDecompressorBase.h"
#include <bitset>
#include <math.h>

#define DEBUG_MODE

class LZ77Compressor : public LZ77CompressorDecompressorBase {

    private:        
        std::string historySlider, activeSlider;

        u_int historyLength, activeLength; // length (letters count)
        u_int historySize, activeSize; // how many bits are needed to write reference and length (w_1, w_2)
        u_int minimalMatchLength;

        void initializeActiveSlider();
        void writeMetaData();
        void magic();
        Reference* checkForLongestMatch();

        int *possiblePositions;

        bool moveSliders();
        bool moveSliders(u_int);
        void addLetterToOutput(char);
        void addReferenceToOutput(u_int, u_int);

    public:
        LZ77Compressor(std::string, std::string, u_int, u_int);
        
        void compress();
};

#endif