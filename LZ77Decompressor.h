#ifndef LZ77DECOMPRESSOR_H
#define LZ77DECOMPRESSOR_H

#include "LZ77CompressorDecompressorBase.h"
#include <vector>
#include <math.h>

#define DEBUG_MODE

class LZ77Decompressor : public LZ77CompressorDecompressorBase {

    private:

        u_int referenceHistoryBits, referenceActiveBits; // how many bits are needed to write reference and length (w_1, w_2)
        u_int historyLength;
        u_int minimalMatchLength;

        std::vector<char> charBuffer;

        void magic();
        void readMetaData();

    public:
        LZ77Decompressor(std::string, std::string);

        void decompress();
};

#endif