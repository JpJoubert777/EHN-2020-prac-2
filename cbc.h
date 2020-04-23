#ifndef CBC_H
#define CBC_H

//int main();
void XOR(char*, char*,char*, int);
void CBC_encrypt(char*, char*, char*,const int, const int, const int,const int, char*);
void CBC_decrypt(char*, char*, char*,const int, const int, const int, const int, char*);
//void padRight (char*, char*, int, int);


#endif // CBC_H