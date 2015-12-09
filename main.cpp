#include "LZ77Compressor.h"
#include "LZ77Decompressor.h"

int main(int argc, char* argv[]) {
    LZ77Compressor* compressor = new LZ77Compressor(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]));
    compressor->compress();
}