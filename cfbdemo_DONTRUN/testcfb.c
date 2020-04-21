#include <stdlib.h>
#include <stdio.h>
#include "cfb.h"
#include "cipherUtils.h"


int main() {
    char iv [] = {'H', 'i', ' ', 't', 'h', 'e', 'r', 'e', ' ', 'y', 'a', ' ', 'p'};
    const int blockSize = 16;
    char sr[blockSize];

    padRight(iv, sr, sizeof(iv), blockSize); // should be manually done in the routing function to the algorithms

    int currKeySize = 6;
    char key[] = {'e', 'h', 'n', '1', '2', '3'};
    int numBits = 192;
    int sl = 1;

    char *rFile = "testR.txt";
    char *wFile = "testW.txt";
    char bc[blockSize];
    char ct[blockSize];
    char *cptr = ct;


    addFiles(rFile, wFile);
    
    int cbl = 0;
    do {
        cbl = getNextBlock(bc, blockSize);  
        CFB(1, bc, cptr, key, sr, currKeySize, sl, cbl, blockSize, numBits);
        writeBlock(cptr, NULL, cbl);
    } while (cbl == blockSize);

    padRight(iv, sr, sizeof(iv), blockSize); // reset sr for decryption
    rFile = wFile;
    wFile = "testD.txt";
    addFiles(rFile, wFile);

    do {
        cbl = getNextBlock(bc, blockSize);  
        CFB(0, bc, cptr, key, sr, currKeySize, sl, cbl, blockSize, numBits);
        writeBlock(cptr, NULL, cbl);
    } while (cbl == blockSize);



    
    // char pt[] = "Some Long ass string that is definitely bigger than 16 by";
    // const int ptSize = sizeof(pt);
    // char bc[blockSize];
    // char ct[ptSize];
    // char *ctPtr = ct;
    // addString(pt, ptSize);
    // int cbl = 0; // current block length
    // do
    // {
    //     cbl = getNextBlock(bc, blockSize);
    //     // printf("%i", cbl);
    //     // for (int i = 0; i < cbl; i++) 
    //     //     printf("%c",bc[i]);
        
    //     CFB(1, bc, ctPtr, key, sr, currKeySize, sl, cbl, blockSize, numBits);
    //     ctPtr += cbl;
    // } while (cbl == blockSize);
    // printf("\n");

    // printf("%s\n", ct);

    // char decrypted[ptSize];
    // char *dptr = decrypted;

    // addString(ct, ptSize);
    // padRight(iv, sr, sizeof(iv), blockSize);

    // cbl = 0; // current block length
    // do
    // {
    //     cbl = getNextBlock(bc, blockSize);
    //     CFB(0, bc, dptr, key, sr, currKeySize, sl, cbl, blockSize, numBits);
    //     dptr += cbl;
    // } while (cbl == blockSize);
    
    // printf("%s\n", decrypted);
    // printf("ct: %.9s", ct);
    // 
    // CFB(0, ct, decrypted, key, sr, currKeySize, sl, ptSize, blockSize, numBits);
    // printf("decrypted: %.9s", decrypted);
    return 0;
}
