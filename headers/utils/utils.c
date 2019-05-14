#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ENCRYPT 1
#define DECRYPT 2

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

bool isBufferIndexBelowBlockSize(int index, int option){
  return index < 16 && option == ENCRYPT;
}

void writeOnFile(FILE *file, int *bufferResult, const char *format){
  if(bufferResult != NULL){
    for(int i = 0; i < 16; i++){//Write on the file
      if(bufferResult[i] != 0){//The zero was added in the padding so it shouldnt be printed
        fprintf(file, format, bufferResult[i]);
      }
    }
  } else {
    printf("Key size MUST be 16 bytes.\n");
  }
}

bool compareFiles(char *filename1, char *filename2){
  char *path1 = (char*)calloc(128, sizeof(char));
  char *path2 = (char*)calloc(128, sizeof(char));

  char charRead1 = 0, charRead2 = 0;

  strcpy(path1, "assets/");
  strcpy(path2, "assets/");

  strcat(path1, filename1);
  strcat(path2, filename2);

  FILE *file1 = NULL, *file2 = NULL;

  file1 = fopen(path1, "r");
  file2 = fopen(path2, "r");

  if(file1 == NULL || file2 == NULL){
    printf("Erro ao abrir arquivos.\n");
    return false;
  }

  do{
    charRead1 = fgetc(file1);
    charRead2 = fgetc(file2);
    if(charRead1 != charRead2){//Different
      return false;
    }
  }while(charRead1 != EOF && charRead2 != EOF);

  if(charRead1 != EOF || charRead2 != EOF){//One file has finished first, therefore they are different
    return false;
  }

  return true;
}
