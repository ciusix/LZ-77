#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "LZ77Compressor.h"
#include "LZ77Decompressor.h"
#include <time.h>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Error, no command." << std::endl;
        exit(1);
    }

    unsigned t0=clock(),t1;

    if (strcmp(argv[1], "compress") == 0) {
        if (argc != 6) {
            std::cout << "Error, wrong number of parameters" << std::endl;
        } else {
            LZ77Compressor* compressor = new LZ77Compressor(argv[2], argv[3], atoi(argv[4]), atoi(argv[5]));
            compressor->compress();
        }
    
    } else if (strcmp(argv[1], "decompress") == 0) {
    
        if (argc != 4) {
            std::cout << "Error, wrong number of parameters" << std::endl;
        } else {
            LZ77Decompressor* decompressor = new LZ77Decompressor(argv[2], argv[3]);
            decompressor->decompress();
        }
        
    } else {
        std::cout << "Error, unknown command." << std::endl;
    }

    t1=clock()-t0;
    std::cout << "Time: " << t1 << "ms" << std::endl;
}