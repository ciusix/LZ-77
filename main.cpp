#include "LZ77Compressor.h"
#include "LZ77Decompressor.h"

int main(int argc, char* argv[]) {
    LZ77Compressor* compressor = new LZ77Compressor("test.txt", "testout.out");
    compressor->compress();
}