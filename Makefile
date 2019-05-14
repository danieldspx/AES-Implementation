main: main.c headers/aes/aes.c
	gcc -o main main.c headers/aes/aes.c headers/utils/utils.c -lm -I.
