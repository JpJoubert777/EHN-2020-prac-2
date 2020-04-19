main: main.o aes.o cfb.o shiftRegister.o
	gcc -o main main.o aes.o cfb.o shiftRegister.o
main.o: main.c 
	gcc -c main.c
aes.o: aes.c 
	gcc -c aes.c
cfb.o:	cfb.c
	gcc -c cfb.c
shiftRegister.o: shiftRegister.c
	gcc -c shiftRegister.c
