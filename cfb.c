#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "aes.h"
#include "cfb.h"


/**
 * @brief Utility for xor operation across two arrays(one and two). Result is loaded to output.
 * NOTE: all arrays to be at least of length size
 * @param one 
 * @param two 
 * @param output 
 * @param size size of arrays in bytes
 */
void ArrayXor(char *one, char *two, char *output, int size) {
    for (int i = 0; i < size; i++) 
        *(output++) = *(one++) ^ *(two++); // note that pointers one, two and output will remain unchanged when function call returns but data will change
}

/**
 * @brief shifts the shiftRegister sr to the left by sl bytes. fills in the new space at the end with fill if fill doesn't point to NULL
 * 
 * @param sr shiftRegister
 * @param size size of the shiftRegister sr
 * @param sl shiftLeft = number of bytes to shift left
 * @param fill fill in characters into the new space
 */
void leftShift(char *sr, int size, int sl, char *fill = NULL) {
    for (int i = 0, int j = sl; j < size; i++, j++) {
        sr[i] = sr[j];
    }
    if (fill != NULL) 
        for (int i = sl; i < size; sr[i++] = *(fill++));
    else 
        for (int i = sl; i < size; sr[i++] = 0);
}

/**
 * @brief Utility function to pad zeros to the right. Copies src to dest and pads the remaining of dest with zeros.
 * 
 * @param src unpadded array
 * @param dest padded array
 * @param srcSize size of src array
 * @param destSize size of dest array.
 */
void padRight (char* src, char* dest, int srcSize, int destSize) {
    int i = 0;
    for (; i < srcSize; dest[i] = src[i++]);
    for (; i < destSize; dest[i++] = 0); // pad with zeros to the right
}

/**
 * @brief AES encryption in CFB mode. Works for both encryption and decryption.
 * For Encryption: input = pt, output = ct
 * For Decryption: input = ct, output = pt
 * 
 * @param input input array = input to the CFB encryption algorithm (E and D)
 * @param output output array = stores the CFB result (E and D)
 * @param key unpadded key to be used in the AES algorithm
 * @param sr shift register to be used as input to the AES algorithm
 * @param currKeySize unpadded size of the given key
 * @param sl stream length(bytes) = sizes of streams to the divide the block into
 * @param inputSize number of bytes in the current input - does not have to be a multiple of sl - this case is handled.
 * @param blockSize number of bytes per block
 * @param numBits AES key size mode
 */
void CFB(char *input, char *output, char *key, char *sr, int currKeySize, int sl, const int inputSize, const int blockSize, const int numBits) {
    const int fullKeySize = 32;
    char fullKey[fullKeySize];
    padRight(key, fullKey, currKeySize, fullKeySize);

    int iters = inputSize/sl;
    if (inputSize % sl != 0) 
        iters++;

    for (int i = 0; i < iters; i++) { // splits the block into streams and processes till block end
        if (i == iters-1)
            sl = inputSize % sl; // so that input creates same sized output without padding needed when last stream isn't a multiple of sl
        char expandedKey[15][4][4];
        keyExpander(fullKey, expandedKey, numBits);
        char bc[blockSize];
        for (int j = 0; j < blockSize; bc[i] = sr[i++]); // copy shiftRegister to blockCipher
        applyEncryinptionRounds(bc, expandedKey, numBits);
        // now bc will be altered by the AES but sr will remain as is
        ArrayXor(bc, input, output, sl);
        leftShift(sr, blockSize, sl, output);
        output += sl;
        input += sl;
    }
}