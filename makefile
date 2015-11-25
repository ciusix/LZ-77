CC = g++
CFLAGS = -Wall -static-libgcc -static-libstdc++

default: lz77

build: clean lz77

lz77:
	$(CC) $(CFLAGS) -o lz77 main.cpp
	
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