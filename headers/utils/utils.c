#include "utils.h"
#include <stdio.h>
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
