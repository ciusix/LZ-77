#ifndef LZ77COMPRESSORDECOMPRESSORBASE_H
#define LZ77COMPRESSORDECOMPRESSORBASE_H

#define u_int unsigned int
#define CONSOLE std::cout
#define ENDL std::endl
#define CHAR_READ_SUCC 1
#define CHAR_READ_FAIL 0
#define BIT_CHAR 0x00
#define BIT_REFERENCE 0x01

#include <string.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>

class LZ77CompressorDecompressorBase {

    protected:
        struct Reference {
            u_int position;
            u_int length;
        };

        std::string inputFileName;
        std::ifstream inputFile;
        std::string outputFileName;
        std::ofstream outputFile;

        void prepareInputFile();
        void closeInputFile();
        void prepareOutputFile();
        void closeOutputFile();
        char readChar(char*);
        void writeABitToFile(char);
        void writeABitToFile(char, bool);
        void writeChar(char);
        char readABitFromFile(char*);
        int getMinimalMatchLength(u_int, u_int);

    public:
        LZ77CompressorDecompressorBase(std::string, std::string);

};

#endif