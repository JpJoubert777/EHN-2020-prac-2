main: main.o aes.o cfb.o shiftRegister.o
	gcc -o main main.o aes.o cfb.o shiftRegister.o
main.o: main.c 
	gcc -c main.c
aes.o: aes.c aes.h
	gcc -c aes.c
cfb.o:	cfb.c cfb.h
	gcc -c cfb.c
shiftRegister.o: shiftRegister.c
	gcc -c shiftRegister.c

testcfb.o: testcfb.c
	gcc -c testcfb.c

testcfb: testcfb.o aes.o cfb.o 
	gcc -o testcfb testcfb.o aes.o cfb.o 

clean:
	rm *.o $(objects) 
