#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "aes.h"
#include "cbc.h"
#include "cipherUtils.h"

/**
 * @brief      A bitwise XOR of string 1 and string 2 and stored in string 3
 *
 * @param      str1  The string 1
 * @param      str2  The string 2
 * @param      str3  The string 3
 * @param[in]  size  The size of the strings
 */
void XOR(char* str1, char* str2, char* str3, int size)
{
	for (int i = 0; i < size; i++)
	{
		str3[i] = (char)((int)str1[i] ^ (int)str2[i]);
	}
} 



/**
 * @brief      Performs the Cipher Block Chaining algorithm with AES as encryption mode
 *
 * @param      Userplaintext  The plaintext block
 * @param      UserIV         The initialisation vector
 * @param      Userkey        The key
 * @param[in]  curKeySize     The current key size
 * @param[in]  inputSize      The input size
 * @param[in]  numBits        The number bits
 * @param[in]  blocksize      The number bytes in the block
 * @param      ciphertext     The final ciphertext storing output
 */
void CBC_encrypt(char* Userplaintext, char* UserIV, char* Userkey, const int curKeySize, const int inputSize, const int numBits, const int blocksize, char * ciphertext)
{
	char * plaintext [blocksize]; //the size per block (16 bytes), will be padded if neccessary
	char * key [32]; //the max key size (32 bytes), also padded if neccessary
	char * IV [blocksize];//the max IV size (16 bytes), will be padded if neccessary

	padRight (Userplaintext,plaintext,inputSize, blocksize);
	padRight (Userkey,key,strlen(Userkey), 32);
	// padRight(UserIV, IV, strlen(UserIV), blocksize);

	unsigned char expandedKey[15][4][4];// to be passed to AES algorithm

	char first [blocksize]; //stores output of XOR algorithm
	XOR(plaintext, UserIV, first, blocksize);

	keyExpander(key, expandedKey, numBits);
	applyEncryptionRounds(first, expandedKey, numBits);//calls AES encryption alogorithm

	for (int i = 0; i < blocksize; i++)
    {
    	ciphertext[i] = first[i];//Strore output of alogorithm in cihpertext
    }
	

}



/**
 * @brief      Performs the Cipher Block Chaining algorithm with AES as decryption mode
 *
 * @param      ciphertext  The ciphertext to be decrypted
 * @param      UserIV      The initialisation vector
 * @param      Userkey     The key
 * @param[in]  curKeySize  The current key size
 * @param[in]  inputSize   The input size
 * @param[in]  numBits     The number of bits
 * @param[in]  blocksize      The number bytes in the block
 * @param      newPlain    The final plaintext storing output
 */
void CBC_decrypt(char* ciphertext, char* UserIV, char* Userkey, const int curKeySize, const int inputSize, const int numBits,const int blocksize, char* newPlain)
{
	char * key [32];//The key to be used
	char * IV [blocksize];//the initialisation vector

	padRight (Userkey,key,strlen(Userkey), 32);//padding for correct size
	// padRight(UserIV, IV, blocksize, blocksize);//padding for correct size

	unsigned char expandedKey[15][4][4];// to be passed to AES algorithm
	keyExpander(key, expandedKey, numBits);
	applyDecryptionRounds(ciphertext, expandedKey, numBits);


	char first [blocksize];//stores output of XOR alogrithm
	XOR(ciphertext, UserIV, first, blocksize);

	for (int i = 0; i < blocksize; i++)
    {
    	newPlain[i] = first[i];//store output of algorithm in newPlain 
    }
}