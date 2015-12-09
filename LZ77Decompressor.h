#ifndef LZ77DECOMPRESSOR_H
#define LZ77DECOMPRESSOR_H

#define INVALID_CHAR 0x03
#define u_int unsigned int
#define CONSOLE std::cout
#define ENDL std::endl

#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>

class LZ77Decompressor {

    private:
        std::string inputFileName;
        std::ifstream inputFile;
        std::string outputFileName;
        std::ofstream outputFile;

        void prepareInputFile();
        void closeInputFile();
        void prepareOutputFile();
        void closeOutputFile();

    public:
        LZ77Decompressor(std::string, std::string);

        void decompress();
};

#endif