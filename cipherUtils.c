#include <stdio.h>
#include "cipherUtils.h"

//These global variables are inherently private to the current file
FILE *rFile = NULL, *wFile = NULL; // rFile = file being read from, wFile = file being written to 
char *string = NULL; // current string that is divided into blocks
int stringSize = 0; // current string full length
int inputMode = 1; // 1 if string inputMode, 0 if file inputMode

/**
 * @brief Treats input and output as file paths that can be used to write them. Opens the files in their respective access modes(input -> read-only, output -> write) and assigns them to global pointers that can be used by other functions. 
 * NOTE: output file will be created if it doesn't already exist and will be overwritten if it exists
 * Non-existent or invalid file paths will return 0
 * 
 * @param input Read file path
 * @param output Write file path
 * @return int 1 if successfully opened, 0 if unsuccessful
 */
int addFiles(char *input, char *output) {
    
    rFile = fopen(input, "r");
    if (rFile == NULL) {
        fprintf(stderr, "Could not open %s or File not found\n", input);
        return 0;
    }    
    wFile = fopen(output, "w+");
    if (wFile == NULL) {
        fprintf(stderr, "Could not open or create file: %s\n", output);
        return 0;
    }
        
    string = NULL;
    inputMode = 0; // file inputMode

    return 1;
} 

/**
 * @brief adds the new string to divide into blocks. Its full size is also stored.
 * 
 * @param fullString pointer to large string being divided
 * @param size size of fullString
 */
void addString(char* fullString, int size) {
    closeFiles();
    string = fullString;
    stringSize = size;
    inputMode = 1; // string inputMode
} 

/**
 * @brief Closes files linked to rFile and wFile and sets them to NULL
 * NOTE: this is called automatically after the last block from rFile is encrypted and written to wFile automatically. But when stopping early, this should be called by user program.
 */
void closeFiles() {
    if (rFile != NULL) 
        fclose(rFile);
    if (wFile != NULL)
        fclose(wFile);
    rFile = wFile = NULL;
    string = NULL;
} 

/**
 * @brief Reads the next block of bytes into bc, either from the file being read or the string based on the inputMode. Returns the number of bytes that were read into bc. This number lower than blockSize if there are not enough bytes left in the string.
 * 
 * @param bc block will be loaded into this array
 * @param blockSize number of bytes to be loaded into this block
 * @return int number of bytes actually read into bc
 */
int getNextBlock(char *bc, int blockSize) {
    int size = 0;
    if (inputMode) { // string inputMode
        if (stringSize == 0) return 0;
        
        size = (stringSize > blockSize) ? blockSize : stringSize; // stream cipher requirement
        
        for (int i = 0; i < size; bc[i++] = *(string++));
        // string ptr will point to the beginning of the new block each call of this function 

        stringSize -= size;
    }

    else { //file inputMode
        if (rFile == NULL) return 0;

        size = fread(bc, sizeof(char), blockSize, rFile);
    }

    return size;
    
}

/**
 * @brief Copies bytes from bc to the respective output based on the inputMode(to output if inputMode=1 and to file if inputMode=0). If output == NULL and inputMode=0, write to file else write to output array. Size is the number of bytes to be written from bc to the correct output.
 * NOTE: if output is specified, user's responsibility to ensure that size doesn't exceed the length of output
 * 
 * @param bc block to copy from
 * @param output copy destination array
 * @param size size of bc to be copied
 * @return int 1 if write success else 0
 */
int writeBlock(char *bc, char* output, int size) {
    if (inputMode && output != NULL) { //string inputMode
        for (int i = 0; i < size; output[i++] = bc[i]);
    }
    else if (!inputMode && output == NULL) { //file inputMode
        fwrite(bc, sizeof(char), size, wFile);
        if (feof(rFile)) closeFiles();
    }
    else return 0; // invalid parameters
    return 1; // write success
} 

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
 * @param fill fill in characters into the new space - use NULL if no fill array
 */
void leftShift(char *sr, int size, int sl, char *fill) {
    int i = 0;
    for (int j = sl; j < size; sr[i++] = sr[j++]);
        
    if (fill != NULL) 
        for (; i < size; sr[i++] = *(fill++));
    else 
        for (; i < size; sr[i++] = 0);
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
    for (; i < srcSize; dest[i++] = src[i]);
    for (; i < destSize; dest[i++] = 0); // pad with zeros to the right
}
