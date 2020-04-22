/**
 * @file main.c
 * @author Jp Joubert (jpemail777@gmail.com)
 * @brief main file
 * @version 1
 * @date 2020-04-13
 * 
 * 
 */
/* Headers */
#include "aes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char* argv[])
{	

	
	//A lot of print statements get called to demonstrate the output

	//for (int n = 0; n < argc; n++)
	//	printf("%d -->%s\n",n,argv[n]);


/*

	./main e ECB 128 EHN 410 practical 2 -key AES_encrypt -go

	0 -->./main
	1 -->e		encrypt
	2 -->ECB	use ECB
	3 -->128	use 128 bits
	4 -->EHN	plaintext
	5 -->410	plaintext
	6 -->practical	plaintext
	7 -->2		plaintext
	8 -->-key	key delimeter command preceding the key
	9 -->AES_encrypt 
	
	note that the plaintext and key can be one word or multiple words, the code will figure it out
	-go indicates the end of the entire command line
*/	
	int numBits = atoi(argv[3]);
	//for (int n = 0; n < strlen(argv[3]); n++)
	//	printf("char[%d] = %c\n",n,argv[3][n]);


	//count how many characters are in the plaintext
	int charIterator = 0;
	int commandIterator = 4; //plaintext always starts with 4th word in the command line
	int wordCount = 0;
	int charCount = 0;	
	while (1)
	{
		if (argv[commandIterator][charIterator] == '-')
			break;
		//printf("char[%d] = %c charCount = %d\n",charIterator,argv[commandIterator][charIterator],charCount);
		charIterator++;
		charCount++;
		if (argv[commandIterator][charIterator] == '\0')
		{
			commandIterator++;
			wordCount++;
			charIterator = 0;
		}
	}
	//add spaces based on how many words there were
	if (wordCount > 1)
		charCount += wordCount -1;
	/*printf("wordCount = %d\n",wordCount);
	printf("charCount = %d\n",charCount);
	printf("commandIterator = %d\n",commandIterator);
	printf("charIterator = %d\n",charIterator);
	
	printf("numBits = %d\n",numBits);
	if (argv[1][0] == 'e')
		printf("encrypt\n");
	

	printf("##########################################\n");
	*/

	//do the same but for the key
	int keyCommandIndex = commandIterator +1; //key always starts at this part of the command
	int charIteratorKey = 0;
	int commandIteratorKey = keyCommandIndex;
	int wordCountKey = 0;
	int charCountKey = 0;	
	while (1)
	{
		if (argv[commandIteratorKey][charIteratorKey] == '-')
			break;
		//printf("char[%d] = %c charCountKey = %d\n",charIteratorKey,argv[commandIteratorKey][charIteratorKey],charCountKey);
		charIteratorKey++;
		charCountKey++;
		if (argv[commandIteratorKey][charIteratorKey] == '\0')
		{
			commandIteratorKey++;
			wordCountKey++;
			charIteratorKey = 0;
		}
	}
	//spaces
	if (wordCountKey > 1)
		charCountKey += wordCountKey -1;
	/*printf("wordCountKey = %d\n",wordCountKey);
	printf("charCountKey = %d\n",charCountKey);
	printf("commandIteratorKey = %d\n",commandIteratorKey);
	printf("charIteratorKey = %d\n",charIteratorKey);*/

	//copy the key from the terminal to an array
	unsigned char *key = malloc(charCountKey + 1);
	int internalIteratorKey = 0;
	for (int n = keyCommandIndex; n < keyCommandIndex+wordCountKey; n++)
	{
		for (int m = 0; m < strlen(argv[n]);m++)
		{
			//printf("%d -->%c\n",n,argv[n][m]);	
			key[internalIteratorKey] = argv[n][m];
			internalIteratorKey++;
		}
		key[internalIteratorKey] = ' ';
		internalIteratorKey++;
	}

	//get original key length and determine the required length 
	int originalKeyLength = charCountKey;
	int requiredKeyLength = numBits/8; // either 16, 24 or 32

	//make an array of the required length and copy the original 
	unsigned char *paddedKey = malloc(requiredKeyLength + 1);
	for (int i = 0; i < originalKeyLength; i++)
		paddedKey[i] = key[i];
	//padding with zeros		
	for (int j = originalKeyLength; j < requiredKeyLength; j++)
		paddedKey[j] = 0x0;

	//expand the key
	unsigned char expandedKeys[15][4][4];
	keyExpander(paddedKey,expandedKeys,numBits);

	/*printf("\nOriginal key:\n");
	for (int i = 0; i < requiredKeyLength; i++)
		printf("%c",paddedKey[i]);
	printf("\n");*/

	//if we the user gave the encrypt command
	if (argv[1][0] == 'e')
	{
		//copy the plaintext from the terminal to an array
		unsigned char *plaintext = malloc(charCount + 1);
		int internalIterator = 0;
		for (int n = 4; n < 4+wordCount; n++)
		{
			for (int m = 0; m < strlen(argv[n]);m++)
			{
				//printf("%d -->%c\n",n,argv[n][m]);	
				plaintext[internalIterator] = argv[n][m];
				internalIterator++;
			}
			plaintext[internalIterator] = ' ';
			internalIterator++;
		}
		//get required plaintext length
		int originalPlaintextLength = charCount;
		int requiredPlaintextLength = getrequiredLength(originalPlaintextLength);
		//get the number of blocks the plaintext must be broken up into
		int numBlocks = requiredPlaintextLength/16;
		//make an array of the required length and copy the original 
		unsigned char *paddedString = malloc(requiredPlaintextLength + 1);
		for (int i = 0; i < originalPlaintextLength; i++)
			paddedString[i] = plaintext[i];
		//padding with zeros
		for (int j = originalPlaintextLength; j < requiredPlaintextLength; j++)
			paddedString[j] = 0x0;
		/*printf("Original plaintext:\n");
		for (int i = 0; i < requiredPlaintextLength; i++)
			printf("%c",paddedString[i]);
		printf("\n");*/
		
		if (strcmp(argv[2],"ECB") == 0)
		{
			showSteps = true; //enable printing ECB steps to the console
			printf("*******Encryption*******\n");
			aesEncrypt(paddedString,expandedKeys,numBlocks,numBits);
			int printIterator = 0;
			printf("*******Encryption final result:*******\n");
			for (int m = 0; m < numBlocks; m++)
			{
				for (int n = 0; n < 16; n++)
				{
					printf("%02x ",paddedString[printIterator]);
					printIterator++;
				}
				printf("\n");
			}
			//printf("Complete Cyphertext\n");
			//for (int i = 0; i < requiredPlaintextLength; i++)
			//	printf("%02x ",paddedString[i]);
			//printf("\n");

			/*printf("*******Decryption*******\n");
			//call the decryption function
			aesDecrypt(paddedString,expandedKeys,numBlocks,numBits);
			//printf("Complete decryptedText\n");
			for (int i = 0; i < requiredPlaintextLength; i++)
				printf("%c",paddedString[i]);
			printf("\n");*/
			showSteps = false;
		}
	}
	

	//print the expanded key
	int numRounds;
	if (numBits == 128)
		numRounds = 11;
	if (numBits == 192)
		numRounds = 13;
	if (numBits == 256)
		numRounds = 15;
	printf("\nKey expansion:\n");
	for (int x = 0; x < numRounds; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			for (int z = 0; z < 4; z++)
			{
				printf("%02x ",expandedKeys[x][z][y]);
			}
			
		}
		printf("\n");
	}

	return 0;
}
