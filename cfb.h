#ifndef CFB_H
#define CFB_H

void ArrayXor(char *one, char *two, char *output, int size);

void leftShift(char *sr, int size, int sl, char *fill = NULL);

void padRight (char* src, char* dest, int srcSize, int destSize);

void CFB(char *input, char *output, char *key, char *sr, int currKeySize, int sl, const int inputSize, const int blockSize, const int numBits);

#endif // CFB_H