//Pag 105 do livro
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "headers/aes/aes.h"

#define ENCRYPT 1
#define DECRYPT 2

void clearScreen(){
  system("clear || cls");
}

void replaceExtension(char *filename, char *newExtension){
    char *end = filename + strlen(filename);//End points to the last position of filename

    while (end > filename && *end != '.') {//Goes backwards until find the '.'
        --end;
    }

    if (end > filename) {//Inserts the \0 to say that the strings ends at this point
        *end = '\0';
    }

    strcat(end, newExtension);
}

bool isBufferWithSpace(int index, int option){
  return (index < 16 && option == ENCRYPT) || (index < 32 && option == DECRYPT);
}

int main(){
  // char *plainText = "LoremIpsumDorSit";
  int option, bufferIndex = 0;
  char *filename = (char*)calloc(128, sizeof(char));//There is no reason to pick specifically 128, but I did it.
  char *path = (char*)calloc(128, sizeof(char));

  strcpy(path, "assets/");

  char *bufferRead = (char *)calloc(32, sizeof(char));
  int *bufferResult;
  char *key = (char *)calloc(16, sizeof(char));//16 + '\0'

  FILE *fileRead, *fileWrite;
  fileRead = fileWrite = NULL;
  char charRead;

  printf("%d - Encrypt\n", ENCRYPT);
  printf("%d - Decrypt\n", DECRYPT);
  printf("Enter option: ");
  scanf("%d", &option);

  printf("Enter the key (16 bytes): ");
  scanf("%s", key);

  printf("Enter filename (must be at /assets): ");
  scanf("%s", filename);

  strcat(path, filename);

  //Open Files
  fileRead = fopen(path, "r");
  if(fileRead == NULL){
    printf("Erro ao abrir o arquivo: %s\n", filename);
    return 0;
  }

  if(option == ENCRYPT){
    replaceExtension(path, ".enc");
  } else {
    replaceExtension(path, ".dec");
  }

  remove(path);//Delete file if it exists
  fileWrite = fopen(path, "a");//Should be append
  if(fileWrite == NULL){
    printf("Erro ao criar arquivo\n");
    return 0;
  }

  printf("\n*** Doing some work ***\n");

  // Read File
  bufferIndex = 0;
  while((charRead = fgetc(fileRead)) != EOF){
    //FIXME: When reaches EOF it jumps out without padding && encrypting the last block 
    if(isBufferWithSpace(bufferIndex, option)){
      bufferRead[bufferIndex] = charRead;
      bufferIndex++;
    }

    if(!isBufferWithSpace(bufferIndex, option)){
      bufferIndex = 0;
      if(option == ENCRYPT){
        printf("Encrypting block...\n");
        bufferResult = encrypt(bufferRead, key);
        for(int i = 0; i < 16; i++){//Write on the file
          fprintf(fileWrite, "%02x", bufferResult[i]);
        }
      } else {
        printf("Decrypting block...\n");
        printf("%s\n", bufferRead);
        bufferResult = decrypt(bufferRead, key);
        for(int i = 0; i < 16; i++){//Write on the file
          fprintf(fileWrite, "%c", bufferResult[i]);
        }
      }
    }
  }

  //Write FILE
  printf("\nFIM\n");
  // putc(option, stdout);
  return 0;
}
