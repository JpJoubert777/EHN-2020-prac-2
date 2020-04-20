#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "aes.c"

void XOR(char*, char*,char*, int);
void CBC_encrypt(char*, char*, char*,const int, const int, const int, char*);
void CBC_decrypt(char*, char*, char*,const int, const int, const int, char*);
void padRight (char*, char*, int, int);

int main()
{
	char * UserIV = ",./,./,./,./,./,.";
	char * Userplaintext = "1234567890123456";
	char * Userkey = "!@#$%";

	char * plaintext [16];
	char * key [32];
	char * IV [16];

	padRight (Userplaintext,plaintext,strlen(Userplaintext), 16);
	padRight (Userkey,key,strlen(Userkey), 32);
	padRight(UserIV, IV, strlen(UserIV), 16);

	char ciphertext [16];
	char newPlain [16];
    

	CBC_encrypt(plaintext,IV,key, 16, 16, 128, ciphertext);
	//
	// for (int i = 0; i < 32; i++)
 //    {
 //    	printf("%c", ciphertext[i]);
 //    }   

    printf("%s\n", " ");

    CBC_decrypt(ciphertext,IV,key, 16, 16, 128, newPlain);

    for (int i = 0; i < 16; i++)
    {
    	printf("%c", newPlain[i]);
    }

}

void XOR(char* str1, char* str2, char* str3, int size)
{
	for (int i = 0; i < size; i++)
	{
		str3[i] = (char)((int)str1[i] ^ (int)str2[i]);
	}
}

void padRight (char* src, char* dest, int srcSize, int destSize) {
    int i = 0;
    for (; i < srcSize; dest[i++] = src[i]);
    for (; i < destSize; dest[i++] = 0); // pad with zeros to the right
}

void CBC_encrypt(char* plaintext, char* IV, char* key, const int curKeySize, const int inputSize, const int numBits, char * ciphertext)
{
	const int fullKeySize = 32;
	char fullKey[fullKeySize];
	unsigned char expandedKey[15][4][4];

	//pad whatever needs to be padded

	char first [inputSize];

	//printf("%s\n", " ");
	XOR(plaintext, IV, first, inputSize);

	//printf("%s\n", " ");
	keyExpander(fullKey, expandedKey, numBits);
	applyEncryptionRounds(first, expandedKey, numBits);

	for (int i = 0; i < 16; i++)
    {
    	ciphertext[i] = first[i];
    	printf("%c", first[i]);
    }
	//ciphertext = first;
	printf("%s\n", " ");


}

void CBC_decrypt(char* ciphertext, char* IV, char* key, const int curKeySize, const int inputSize, const int numBits, char* newPlain)
{
	const int fullKeySize = 32;
	unsigned char fullKey[fullKeySize];
	unsigned char expandedKey[15][4][4];

	//pad whatever needs to be padded

	keyExpander(fullKey, expandedKey, numBits);
	applyDecryptionRounds(ciphertext, expandedKey, numBits);


	char first [inputSize];
	XOR(ciphertext, IV, first, inputSize);

	for (int i = 0; i < 16; i++)
    {
    	newPlain[i] = first[i];
    	//printf("%c", newPlain[i]);
    }
	

	printf("%s\n", " ");

}