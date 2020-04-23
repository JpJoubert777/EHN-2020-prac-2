/**
 * @file main.c
 * @author Vishal Thalla
 * @brief 
 * @version 0.1
 * @date 2020-04-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h> // for long command line args
#include "aes.h"
#include "cfb.h"
#include "cbc.h"
#include "cipherUtils.h"


void printUsage() {
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "files: [-e|-d] [-cbc|-cfb|-ecb] <len> -fi <input file> -fo <encrypted file> -key <password> -iv <initialization vector>\n");
    fprintf(stderr, "text: [-e|-d] [-cbc|-cfb|-ecb] <len> -t <text> -key <password> -iv <initialization vector> (-streamlen <len>)\n");
    fprintf(stderr, "use -h for help\n");
    fprintf(stderr, "\n");
}

void printError(char *message) {
    fprintf(stderr, "%s See Usage.\n", message);
    printUsage();
    exit(0);
}

void printHelp() {
    
    printUsage();
    fprintf(stderr, "-e\t\t\tencryption\n");
    fprintf(stderr, "-d\t\t\tdecryption\n");
    fprintf(stderr, "-cbc <len>\t\tcbc encryption/decryption\n");
    fprintf(stderr, "-cfb <len>\t\tcfb encryption/decryption\n");
    fprintf(stderr, "-ecb <len>\t\tecb encryption/decryption\n");
    fprintf(stderr, "<len>\t\t\t128, 192 or 256\n");
    fprintf(stderr, "-t\t\t\t<text to process>\n");
    fprintf(stderr, "-key\t\t\t<password>\n");
    fprintf(stderr, "-fi\t\t\t<input file>\n");
    fprintf(stderr, "-fo\t\t\t<output file>\n");
    fprintf(stderr, "-fo\t\t\t<output file>\n");
    fprintf(stderr, "-streamlen <len>\tlength of the stream (for cfb: either 8, 64 or 128)\n");
    fprintf(stderr, "-h\t\t\thelp\n");
    // fprintf(stderr, "NOTE: Pure AES ECB mode is run by default if -cbc/cfb not specified\n");
    fprintf(stderr, "\n");
    exit(0);

}

int main(int argc,char* argv[]) {

    

    // enum {Encryption, Decryption} edMode = Encryption;

    enum {mECB, mCFB, mCBC} aesMode = mECB;

    enum {File, String} inputMode = String;

    int c;

    int textSize = 0; // length of text
    int fullSize = 0; // full size will be the length of text padded to be a multiple of the blocksize
    int numBits = 0;
    int streamlen = 0;
    const int blockSize = 16;

    char iblock[blockSize]; // will store the current read block for each iteration
    char oblock[blockSize]; // will store the current write block for each iteration
    char *iv = NULL;
    char *key = NULL;
    char *text = NULL;
    char *fullOutput = NULL; // This will store the full output to the input text if required
    char *outputtrav = fullOutput;
    char *iFile = NULL, *oFile = NULL;

    int cfbflg = 0, cbcflg = 0, ecbflg = 0, nbflg = 0;
    static int edflg; // determines whether ecryption or decryption
    static int hflg; 



    while (1) {
        static struct option long_options[] =
        {
            {"e", no_argument, &edflg, 2},
            {"d", no_argument, &edflg, 1},
            {"h", no_argument, &hflg, 1},
            {"cfb", required_argument, 0, 'a'},
            {"cbc", required_argument, 0, 'b'},
            {"ecb", required_argument, 0, 'c'},
            {"t", required_argument, 0, 't'},
            {"fi", required_argument, 0, 'i'},
            {"fo", required_argument, 0, 'o'},
            {"key", required_argument, 0, 'k'},
            {"iv", required_argument, 0, 'v'},
            {"streamlen", required_argument, 0, 's'},
            {0,0,0,0}
        };

        

        int option_index = 0;
        opterr = 0;
        c = getopt_long_only (argc, argv, "a:b:c:t:i:o:k:v:s:", long_options, option_index);


        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {
            case 0:
            /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].name != 0)
                    break;


            case 'a':
                cfbflg = 1;
                numBits = strtol(optarg, NULL, 10);
                aesMode = mCFB;
                break;

            case 'b':
                cbcflg = 1;
                numBits = strtol(optarg, NULL, 10);
                aesMode = mCBC;
                break;
            
            case 'c':
                ecbflg = 1;
                if (!optarg)
                    nbflg = 1;
                else 
                    numBits = strtol(optarg, NULL, 10);
                aesMode = mECB;
                break;
            

            case 't':
                inputMode = String;
                text = optarg;
                break;
            
            case 'i':
                inputMode = File;
                iFile = optarg;
                break;

            case 'o':
                inputMode = File;
                oFile = optarg;
                break;

            case 'k':
                key = optarg;
                break;

            case 'v':
                iv = optarg;
                break;

            case 's':
                streamlen = strtol(optarg, NULL, 10);
                break;

            case '?':
                if (optopt == 'a' || optopt == 'b' || optopt == 'c') {
                    if (!optarg)
                        nbflg = 1;
                }
                break;

            default:
                printError("Invalid command!");
                break;
        }
    
    }

    if (hflg == 1) 
        printHelp();

    if (edflg == 0) 
        printError("Please specify Encryption or Decrytion mode!");

    edflg--; // move edflg from 1 and 2 to 0 and 1
    // from here if encryption if 1 and decryption if 0 for edflg

    if (aesMode == mECB) {
        if (edflg == 0) 
            printError("Decryption of strings not allowed for ECB AES!");
        if (inputMode == File)
            printError("Files not allowed for ECB mode!");
    }
     
        

    if (nbflg) 
        printError("Please specify <len> after aes mode!");

    if (!(numBits == 128 | numBits == 192 | numBits == 256))
        printError("Please specify correct value for key length");

    if (cfbflg ^ cbcflg ^ ecbflg == 0){
        if (cfbflg || cbcflg || ecbflg)
            printError("Only one mode of cipher [cfb|cbc|ecb] may be specified!");
        else
            printError("Please specify AES mode!");   
    }

    if (text && (iFile || oFile))
        printError("Text AND File not allowed in the same command!");
    else if (!text && !(iFile && oFile))
        printError("Please specify both input and output file!");

    if (!key) 
        printError("Please specify the Key");

    if (aesMode != mECB && !iv)
        printError("Please specify the Intialization Vector");

    if (aesMode == mCFB && streamlen == 0)
        printError("Please specify valid streamlen for CFB!");
    

    if (inputMode == File) {
        if (!addFiles(iFile, oFile))
            exit(0);        
    }
    else {
        textSize = strlen(text);
        fullSize = textSize + blockSize - (textSize%blockSize);
        // printf("%i %i", textSize, fullSize);
        addString(text, textSize);
        fullOutput = (char *)calloc(fullSize, sizeof(char)); // fullOutput needs to be used for output so this pointer should hold the value of the array beginning
        outputtrav = fullOutput; //outputtrav will be incremented to point to the beginning of the current block
    }
   
    
    
    char sr[blockSize];
    // if (aesMode == mCFB)
        padRight(iv, sr, strlen(iv), blockSize); // ONLY for cfb

    int sl = streamlen/8; // bits to bytes
    int currKeySize = strlen(key);// non-padded size of the key


    int cbl = 0; // number of bytes read from current block
    do {
        cbl = getNextBlock(iblock, blockSize);
        if (cbl == 0)
            break;

        // NOTE: cbl will return the number of bytes that were ACTUALLY written from the fullstring
        // this number may be different to blocksize for the last block so check and pad iblock as required.
        //This won't interfere with cfb as long as cbl remains the same
        switch (aesMode)
        { // write output to oblock
            case mCFB:
                CFB(edflg, iblock, oblock, key, sr, currKeySize, sl, cbl, blockSize, numBits);
                break;
            
            case mCBC:
                //Sirsh
                if (edflg == 1)
                {
                    //Encrypt
                    CBC_encrypt(iblock,sr,key,currKeySize,cbl,numBits,blockSize,oblock);
                    writeBlock(oblock, sr, blockSize);
                }
                else if(edflg == 0)
                {
                    //Decrypt
                    CBC_decrypt(iblock,sr,key,currKeySize,cbl,numBits,blockSize,oblock);
                    writeBlock(iblock, sr, blockSize);

                }
                break;
            case mECB:
                //JP
		        ECB(iblock,oblock,key,currKeySize,cbl,numBits);
                break;
            default:
                break;
        }

        int lenPrint = (aesMode == mCFB) ? cbl : blockSize;

        if (inputMode == String) {
            for(int i = 0; i < lenPrint; i++)
                *(outputtrav++) = oblock[i];
        }
        else if (inputMode == File)
            writeBlock(oblock, NULL, lenPrint);
       
    } while(cbl == blockSize); // no blocks remaining if cbl != blocksize
    
    if (inputMode == String) {
        int lenPrint = (aesMode == mCFB) ? textSize : fullSize;
        for (int i = 0; i < lenPrint; i++) {
            if (i%blockSize == 0)
                printf("\n");
            printf("%02x ", (unsigned char)fullOutput[i]);
        }
            
        printf("\n");

        if (fullOutput)
            free(fullOutput);
    }
    

    printf("\n%s operation has finished.\n\n", (edflg == 1) ? "Encryption" : "Decryption");
    

}
