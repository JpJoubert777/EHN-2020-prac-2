# EHN-2020-prac-2

23 April 2020
Group 20

Open a terminal in the same folder as the source files.

Compile: Type "make" and hit enter.

Run: Type ./main and refer to the Usage instructions below to specify the parameters

Usage:
- files: [-e|-d] [-cbc|-cfb|-ecb] \<len\> -fi \<input file\> -fo \<encrypted file\> -key \<password\> -iv \<initialization vector\>
- text: [-e|-d] [-cbc|-cfb|-ecb] \<len\> -t \<text to process\> -key \<password\> -iv \<initialization vector\> (-streamlen \<len\>)
- use -h for help

<pre>
Parameters:
-e			        encryption
-d			        decryption
-cbc \<len\>		cbc encryption/decryption
-cfb \<len\>		cfb encryption/decryption
-ecb \<len\>		ecb encryption/decryption
\<len\>			    128, 192 or 256
-t			        \<text to process\> (in quotation marks)
-key			    \<password\>
-fi			        \<input file\> (in quotation marks)
-fo			        \<output file\> (in quotation marks)
-streamlen \<len\>	length of the stream (for cfb: either 8, 64 or 128)
-h			        help
</pre>


ECB AES usage example:\
./main -e -ecb \<len\> -t \<text to process\> -key \<password\>
NOTE: ECB AES refers to pure AES encryption and this mode may only be used for encryption of text strings.


CFB AES usage example:\
./main -e -cfb \<len\> -t \<text to process\> -key \<password\> -iv \<initialization vector\> -streamlen \<len\>\
NOTE: streamlen must be specified for CFB.

CBC AES usage example:\
./main -e -cbc \<len\> -t \<text to process\> -key \<password\> -iv \<initialization vector\>

NOTE:Initialization Vector (iv) is not required for ecb but must be specified for CFB and CBC

The output for text string input will be printed in the hexadecimal format with spaces between each byte and 16 bytes per line.\
This output MAY NOT be used for the text string with decryption; Decryption may only be used with files.

NOTE: sub-directories will not be created automatically!





