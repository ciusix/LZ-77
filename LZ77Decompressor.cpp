#include "LZ77Decompressor.h"

LZ77Decompressor::LZ77Decompressor(std::string inputFileName, std::string outputFileName)
    :LZ77CompressorDecompressorBase(inputFileName, outputFileName) {
        
}

void LZ77Decompressor::decompress() {
    prepareInputFile();
    prepareOutputFile();
    readMetaData();

    magic();
    closeInputFile();
    closeOutputFile();
}

void LZ77Decompressor::magic() {
    char c, bit, flag = CHAR_READ_SUCC;
    u_int referenceDistance, referenceLength;
    u_int cleanupSize = historyLength * 3;

    while (flag != CHAR_READ_FAIL) {
        bit = readABitFromFile(&flag);

        if (flag == CHAR_READ_FAIL) {
            CONSOLE << "Char read fail" << ENDL;
        }

        if (bit == BIT_CHAR) {
            // Letter
            c = 0;
            for (u_int i = 0; i < 8; i++) {
                bit = readABitFromFile(&flag);
                if (flag != CHAR_READ_SUCC) {
                    return;
                }

                c = (c | (bit & 0x01)) << (i != 7 ? 1 : 0);
            }

            charBuffer.push_back(c);
            writeChar(c);
        } else if (bit == BIT_REFERENCE) {
            // Reference

            // Distance
            referenceDistance = 0;
            for (u_int i = 0; i < referenceHistoryBits; i++) {
                bit = readABitFromFile(&flag);
                if (flag != CHAR_READ_SUCC) {
                    return;
                }

                referenceDistance = (referenceDistance | (bit & 0x01)) << (i != referenceHistoryBits - 1 ? 1 : 0);
            }

            // Length
            referenceLength = 0;
            for (u_int i = 0; i < referenceActiveBits; i++) {
                bit = readABitFromFile(&flag);
                if (flag != CHAR_READ_SUCC) {
                    return;
                }

                referenceLength = (referenceLength | (bit & 0x01)) << (i != referenceActiveBits - 1 ? 1 : 0);
            }
            referenceLength += minimalMatchLength;

            // Character
            for (u_int i = 0, j = charBuffer.size(); i < referenceLength; i++) {
                c = charBuffer.at(j - referenceDistance + i);
                charBuffer.push_back(c);

                writeChar(c);
            }
        }

        // Keep the buffer clean
        if (charBuffer.size() > cleanupSize) {
            charBuffer.erase(charBuffer.begin(), charBuffer.begin() + (cleanupSize - historyLength - 1));
        }
    }
}

void LZ77Decompressor::readMetaData() {
    char c, flag = CHAR_READ_SUCC;

    c = readChar(&flag);
    if (flag != CHAR_READ_SUCC) {
        CONSOLE << "Error reading meta data" << ENDL;
        exit(1);
    }

    referenceHistoryBits = (u_int)c;

    c = readChar(&flag);
    if (flag != CHAR_READ_SUCC) {
        CONSOLE << "Error reading meta data" << ENDL;
        exit(1);
    }

    referenceActiveBits = (u_int)c;

    minimalMatchLength = getMinimalMatchLength(referenceHistoryBits, referenceActiveBits);
    historyLength = pow(2, referenceHistoryBits);
}