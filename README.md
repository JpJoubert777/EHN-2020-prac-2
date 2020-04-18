# EHN-2020-prac-2

10 April group 20

Please download and open this readme.md in a text editor as github is hiding some of the parameters for some reason.

Open a terminal in the same folder as the source files.
Type "make" and hit enter.
The command after this has the following format:

./main <e/d> <ECB/CBC/CFB> <len> <textString> -key <keyString> -go

Parameters:

• e encryption

• d decryption

• ECB for electronic codebook

• CBC for cypher block chaining

• CFB for cypher feedback

• <len> either 128, 192 or 256
  
• <textString> for text to decrypt
  
• -key for the password

Example usage:
./main e ECB 128 EHN 410 practical 2 -key AES_encrypt -go

After this you would hit enter.

ASCII characters only.



