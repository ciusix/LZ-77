CC = g++
CFLAGS = -Wall -static-libgcc -static-libstdc++

default: lz77

build: clean lz77

lz77: LZ77Compressor.o LZ77Decompressor.o LZ77CompressorDecompressorBase.o
	$(CC) $(CFLAGS) -o lz77 main.cpp LZ77Compressor.o LZ77Decompressor.o LZ77CompressorDecompressorBase.o

LZ77CompressorDecompressorBase.o:  LZ77CompressorDecompressorBase.cpp LZ77CompressorDecompressorBase.h
	$(CC) $(CFLAGS) -c LZ77CompressorDecompressorBase.cpp

LZ77Compressor.o:  LZ77Compressor.cpp LZ77Compressor.h LZ77CompressorDecompressorBase.o
	$(CC) $(CFLAGS) -c LZ77Compressor.cpp
	
LZ77Decompressor.o:  LZ77Decompressor.cpp LZ77Decompressor.h LZ77CompressorDecompressorBase.o
	$(CC) $(CFLAGS) -c LZ77Decompressor.cpp
  
clean:
ifeq ($(OS),Windows_NT)
	del *.o
	del *.gch
	del *.exe
	del *.compressed
else
	-rm *.o
	-rm *.gch
	-rm lz77
	-rm *.compressed
endif