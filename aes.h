#include <stdbool.h>
/**
 * @brief Variable to indicate if AES ECB steps should be printed to terminal
 */
bool showSteps;
/**
 * @brief Array containing the Rijndael S-box used in the core key sheduler
 * @see https://cryptography.fandom.com/wiki/Rijndael_S-box
 */
unsigned char sBox[256];

/**
 * @brief Array containing the Rijndael inverse S-box
 * @see https://cryptography.fandom.com/wiki/Rijndael_S-box
 */
unsigned char inverseSbox[256];

/**
 * @brief Contains the results of multiplying the numbers 0-255 with 2 modulo (x^4 + 1)
 * @see https://cryptography.fandom.com/wiki/Rijndael_mix_columns
 */
unsigned char multiply2[256];

/**
 * @brief Contains the results of multiplying the numbers 0-255 with 3 modulo (x^4 + 1)
 * @see https://cryptography.fandom.com/wiki/Rijndael_mix_columns
 */
unsigned char multiply3[256];

/**
 * @brief Contains the results of multiplying the numbers 0-255 with 9 modulo (x^4 + 1)
 * @see https://cryptography.fandom.com/wiki/Rijndael_mix_columns
 */
unsigned char multiply9[256];

/**
 * @brief Contains the results of multiplying the numbers 0-255 with 11 modulo (x^4 + 1)
 * @see https://cryptography.fandom.com/wiki/Rijndael_mix_columns
 */
unsigned char multiply11[256];

/**
 * @brief Contains the results of multiplying the numbers 0-255 with 13 modulo (x^4 + 1)
 * @see https://cryptography.fandom.com/wiki/Rijndael_mix_columns
 */
unsigned char multiply13[256];

/**
 * @brief Contains the results of multiplying the numbers 0-255 with 14 modulo (x^4 + 1)
 * @see https://cryptography.fandom.com/wiki/Rijndael_mix_columns
 */
unsigned char multiply14[256];

/**
 * @brief Array that returns the Rcon value given the Rcon index
 * @see https://cryptography.fandom.com/wiki/Rijndael_key_schedule
 */
unsigned char Rcon[255];

/**
 * @brief Prints the state array as characters to the console 
 * 
 * @param state The 16-byte (4x4 bytes) 2D array of the state vector 
 * @return void
 */
void printStateChar(unsigned char state[4][4]);
/**
 * @brief Prints the state array as hex values to the console
 * 
 * @param state The 16-byte (4x4 bytes) 2D array of the state vector 
 * @return void
 */
void printStateHex(unsigned char state[4][4]);
/**
 * @brief Sets state[0][0] as plaintext[0], state[0][1] as plaintext[1], etc.
 * 
 * @param plaintext A 16-char 1D array containing a string of plaintext
 * @param state A 4x4 ouput array where the plaintext will be stored
 * @return void
 */
void make4x4block(unsigned char plaintext[16],unsigned char state[4][4]);
/**
 * @brief Turns the rows of a 4x4 array into columns
 * 
 * @param state A 4x4 array where the plaintext has been stored that must have the rows and columns swapped
 * @return void
 */
void formatInputBlock(unsigned char state[4][4]);
/**
 * @brief Prepares a 16 byte plaintext string by turning it into a 4x4 AES state vector
 * 
 * @param plaintext A 16 character 1D array containing a string of plaintext
 * @param state A 4x4 array where the state vector will be stored
 * @return void
 */
void prepareInput(unsigned char plaintext[16],unsigned char state[4][4]);
/**
 * @brief Uses the values inside of the state vector as indexes to the sBox array and substitutes accordingly 
 * 
 * @param state A 4x4 array where the state vector has been stored that must have substitutions performed
 * @return void
 */
void substituteBytes(unsigned char state[4][4]);

/**
 * @brief Uses the values inside of the state vector as indexes to the inverse sBox and substitutes accordingly 
 * 
 * @param state A 4x4 array where the state vector has been stored that must have invserse substitutions performed
 * @return void
 */
void inverseSubstituteBytes(unsigned char state[4][4]);
/**
 * @brief Shifts the second row to the left by 1, the 3rd row by 2 and the 4th row by 3
 * 
 * @param state A 4x4 array where the state vector has been stored that must have the rows shifted
 * @return void
 */
void shiftRows(unsigned char state[4][4]);
/**
 * @brief Reverses a shift row operation that has already been performed
 * 
 * @param state A 4x4 array where the state vector has been stored that must have the rows shifted back to their original positions
 * @return void
 */
void inverseShiftRows(unsigned char state[4][4]);
/**
 * @brief Performs the XOR dot product multiplication mod (x^4 + 1) with the contents of state and Rijndael's Galois Field vector
 *        
 * @param state A 4x4 array where the state vector has been stored that must have the columns mixed
 * @return void
 */
void mixColumns(unsigned char state[4][4]);
/**
 * @brief Performs the XOR dot product multiplication mod (x^4 + 1) with the contents of state and Rijndael's inverse Galois Field vector
 *        
 * @param state A 4x4 array where the columns have been mixed and must be unmixed
 * @return void
 */
void inverseMixColumns(unsigned char state[4][4]);
/**
 * @brief Transforms four bytes according to Rijndael's core key scheduler
 *        
 * @param input4bytes An array of four bytes that need to be transformed
 * @param counter An integer representing the Rcon iteration number
 * @return void
 */
void coreKeyScheduler(unsigned char input4bytes[4], int counter);
/**
 * @brief Expands a single 16, 24 or 32-byte key into 11, 13 or 15 keys depending on the numBits
 *        
 * @param inputKey An input 1D array that can contain up to 32 bytes
 * @param expandedKeyArray An output array that can contain up to 15 4x4 byte keys
 * @param numBits An integer indicating how many bits the key should use
 * @return void
 */
void keyExpander(unsigned char inputKey[32], unsigned char expandedKeyArray[15][4][4],int numBits);
/**
 * @brief Adds a key to a state by performing an XOR 
 *        
 * @param state A 2D array containing the present state that needs a key added to it
 * @param key A 2D array containing the key to be XOR'ed to the state
 * @return void
 */
void addRoundKey(unsigned char state[4][4],unsigned char key[4][4]);
/**
 * @brief Applies 11,13 or 15 encryption rounds (including the intial round) depending on the numBits
 *        
 * @param plaintext A 1D array containing the 16 bytes of plaintex to be encrypted
 * @param expandedKeys An array containing a round key for each round of encryption
 * @param numBits An integer indicating how many bits the key should use
 * @return void
 */
void applyEncryptionRounds(unsigned char plaintext[16],unsigned char expandedKeys[15][4][4],int numBits);
/**
 * @brief Applies 11,13 or 15 decryption rounds (including the intial round) depending on the numBits
 *        
 * @param cyphertex A 1D array containing the 16 bytes of cyphertex to be decrypted
 * @param expandedKeys An array containing a round key for each round of encryption
 * @param numBits An integer indicating how many bits the key should use
 * @return void
 */
void applyDecryptionRounds(unsigned char cyphertext[16],unsigned char expandedKeys[15][4][4], int numBits);
/**
 * @brief Performs padding and encryption on a plaintext string of 16-bytes
 *        
 * @param unPaddedString An input string not padded with zeros
 * @param encryptedString The output encrypted string
 * @param key A key that will be expanded based on numBits
 * @param currKeySize An integer that indicates how many characters are actually present in the key array
 * @param cbl An integer that indicates how many characters are actually present in the unPaddedString array
 * @param numBits An integer indicating how many bits the key should use
 * @return void
 */
void ECB(unsigned char unPaddedString[16],unsigned char encryptedString[16],unsigned char key[32],int currKeySize,int cbl,int numBits);



