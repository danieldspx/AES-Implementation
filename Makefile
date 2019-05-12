main: main.c headers/aes/aes.c
	gcc -o main main.c headers/aes/aes.c -lm -I.
