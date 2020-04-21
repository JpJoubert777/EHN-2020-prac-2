#ifndef CFB_H
#define CFB_H

void CFB(uint mode, char *input, char *output, char *key, char *sr, int currKeySize, int sl, const int inputSize, const int blockSize, const int numBits);

#endif // CFB_H