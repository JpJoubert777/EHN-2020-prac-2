# EHN-2020-prac-2

10 April group 20

Please open this readme in a text editor rather than reading from gitHub. Some text doesn't show on github for some reason.
Open a terminal in the same folder as the source files.
Type "make" and hit enter.
The command after this has the following format:

./main <e/d> <ECB/CFB/CBC> <128/192/256> <textString> -key <keyString> -go

Where:

<e/d> is where you type EITHER "e" or "d" for encryption or decryption, respectively.

<ECB/CFB/CBC> is where you type EITHER "ECB", "CFB" or "CBC". This is to choose one of the modes of operation.

<128/192/256> is where you type EITHER "128", "192" or "256". This is how the key length is chosen.

<textString> Would be your plaintext if you are encrypting. Ciphertext if you are decrypting. ASCII characters only.
  
<keyString> Is your key. ASCII characters only.
  
  
Exclude quotation marks in your input. Here is an example input copied directly from the terminal:

./main e ECB 128 EHN 410 practical 2 -key AES_encrypt -go

After this, you would hit enter.
