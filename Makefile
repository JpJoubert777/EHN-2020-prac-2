main: main.o aes.o
	gcc -o main main.o aes.o 
main.o: main.c 
	gcc -c main.c
aes.o: aes.c 
	gcc -c aes.c
