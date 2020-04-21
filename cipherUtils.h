#ifndef CIPHERUTILS_H
#define CIPHERUTILS_H

#include <stdio.h>


int addFiles(char *input, char *output); // input -> rFile, output -> wFile, string = NULL, mode = 0

void addString(char* fullString, int size); // string = fullString, stringSize = size, close r and w files, rFile = NULL, wFile = NULL, mode = 1

void closeFiles(); // closes any open files and sets file = NULL and string to 0

int getNextBlock(char *bc, int blockSize); // return number of new bytes read into bc

int writeBlock(char *bc, char* output, int size); // size is the number of bytes written from block to output
// if output == NULL, write to file else write to output
// NOTE: if output is specified, user's responsibility to ensure that size doesn't exceed the length of output

void ArrayXor(char *one, char *two, char *output, int size); // XORs two arrays together

void leftShift(char *sr, int size, int sl, char *fill); // shift sr by sl and fills in the remaining with fill

void padRight (char* src, char* dest, int srcSize, int destSize); // fill dest with src and pad zeros to right

#endif // CIPHERUTILS_H