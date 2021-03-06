main: main.o cipherUtils.o aes.o cfb.o cbc.o
	gcc -o main main.o cipherUtils.o aes.o cfb.o cbc.o
main.o: main.c 
	gcc -c main.c
aes.o: aes.c aes.h
	gcc -c aes.c
cfb.o:	cfb.c cfb.h
	gcc -c cfb.c

cbc.o: cbc.c cbc.h
	gcc -c cbc.c

shiftRegister.o: shiftRegister.c
	gcc -c shiftRegister.c

cipherUtils.o: cipherUtils.c cipherUtils.h
	gcc -c cipherUtils.c

testcfb.o: testcfb.c
	gcc -c testcfb.c

testcfb: testcfb.o cipherUtils.o aes.o cfb.o 
	gcc -o testcfb testcfb.o cipherUtils.o aes.o cfb.o 

clean:
	rm *.o $(objects) 
