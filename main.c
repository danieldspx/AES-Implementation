#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "headers/aes/aes.h"
#include "headers/utils/utils.h"

#define ENCRYPT 1
#define DECRYPT 2

int main(){
  int option, bufferIndex = 0;
  char *filename = (char*)calloc(128, sizeof(char));//There is no reason to pick specifically 128, but I did it.
  char *compareFilename1 = (char*)calloc(128, sizeof(char));
  char *compareFilename2 = (char*)calloc(128, sizeof(char));
  char *path = (char*)calloc(128, sizeof(char));
  char charRead;

  char *bufferRead = (char *)calloc(32, sizeof(char));
  int *bufferResult;
  char *key = (char *)calloc(16, sizeof(char));

  FILE *fileRead, *fileWrite;

  fileRead = fileWrite = NULL;
  strcpy(path, "assets/");

  printf("%d - Encrypt\n", ENCRYPT);
  printf("%d - Decrypt\n", DECRYPT);

  printf("\nEnter option: ");
  scanf("%d", &option);

  printf("Enter the key (16 bytes): ");
  scanf("%s", key);

  if(!isKey16Bytes(key)){
    printf("The KEY size MUST be 16 bytes.\n\n");
    return 0;
  }

  printf("Enter the filename (must be at /assets): ");
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
  fileWrite = fopen(path, "a");
  if(fileWrite == NULL){
    printf("Erro ao criar arquivo\n");
    return 0;
  }

  printf("\n*** Starting ***\n\n");

  // Read File
  while((charRead = fgetc(fileRead)) != EOF || isBufferIndexBelowBlockSize(bufferIndex, option)){
    if(isBufferWithSpace(bufferIndex, option)){
      bufferRead[bufferIndex] = charRead;
      bufferIndex++;
    }

    if(!isBufferWithSpace(bufferIndex, option) || charRead == EOF){
      if(option == ENCRYPT){
        if(charRead == EOF && isBufferIndexBelowBlockSize(bufferIndex, option)){
          bufferRead[bufferIndex-1] = '\0';//This indicates where we need start the padding
        }
        printf("Encrypting block...\n");
        bufferResult = encrypt(bufferRead, key);
        writeOnFile(fileWrite, bufferResult, "%02x");
        free(bufferResult);
      } else {
        printf("Decrypting block...\n");
        bufferResult = decrypt(bufferRead, key);
        writeOnFile(fileWrite, bufferResult, "%c");
        free(bufferResult);
      }
      bufferIndex = 0;
    }

    if(charRead == EOF){
      break;
    }
  }

  fclose(fileRead);
  fclose(fileWrite);

  if(option == DECRYPT){
    printf("\n*** Compare Files ***\n\n");
    printf("Filename 1: ");
    scanf("%s", compareFilename1);

    printf("Filename 2: ");
    scanf("%s", compareFilename2);

    if(compareFiles(compareFilename1, compareFilename2)){
      printf("\nOs arquivos são idênticos.\n");
    } else {
      printf("\nOs arquivos não são idênticos.\n");
    }
  }

  printf("\nEND\n");

  return 0;
}
