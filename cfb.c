#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "aes.h"
#include "cfb.h"
#include "cipherUtils.h"


/**
 * @brief AES encryption in CFB mode. Works for both encryption and decryption depending on the mode.
 * For Encryption: input = pt, output = ct
 * For Decryption: input = ct, output = pt
 * 
 * @param mode encryption if 1 and decryption if 0
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
void CFB(uint mode, char *input, char *output, char *key, char *sr, int currKeySize, int sl, const int inputSize, const int blockSize, const int numBits) {
    const int fullKeySize = 32;
    char fullKey[fullKeySize];
    padRight(key, fullKey, currKeySize, fullKeySize); // pad the key to a full 32 bytes to feed into AES key expander


    int iters = inputSize/sl;
    if (inputSize % sl != 0) 
        iters++;

    char bc[blockSize];
    
    
    for (int i = 0; i < iters; i++) { // splits the block into streams and processes till block end
        if (i == iters-1 && inputSize < sl)
            sl = inputSize % sl; // so that input creates same sized output without padding needed when last stream isn't a multiple of sl       

        for (int j = 0; j < blockSize; bc[j++] = sr[j]); // copy shiftRegister to blockCipher

        unsigned char expandedKey[15][4][4];
        keyExpander(fullKey, expandedKey, numBits);
        applyEncryptionRounds(bc, expandedKey, numBits);
    
        // now bc will be altered by the AES but sr will remain as is
        if (mode) { // encryption mode
            ArrayXor(bc, input, output, sl);    
            leftShift(sr, blockSize, sl, output);
        }
        else { // decryption mode
            leftShift(sr, blockSize, sl, input);
            ArrayXor(bc, input, output, sl);    
        }
        
        
        output += sl;
        input += sl;
    }
}