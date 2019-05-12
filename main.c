//Pag 105 do livro
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "headers/aes/aes.h"

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define DECRYPT true
#define ENCRYPT false

int main(){
  char plainText[] = "LoremIpsumDorSit";//Must be 16 bytes
  int keyWords[44][4];
  int stateCypher[16];
  int keyCypher[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
  calcAllSubkeys(keyWords, keyCypher);

  blockTextToInt(plainText, stateCypher);

  //Encrypt
  for(int round = 0; round <= 10; round++){
    if(round != 0){
      subBytes(stateCypher);
      shiftRows(stateCypher, ENCRYPT);
      if(round != 10){
        mixColumns(stateCypher, ENCRYPT);
      }
    }
    addRoundKey(stateCypher, keyWords, round);
  }
  printf("Encrypted: \n");
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      printf("%02x", stateCypher[4*i+j]);
    }
  }
  printf("\n\n");

  //Decrypt
  for(int round = 10; round >= 0; round--){
    addRoundKey(stateCypher, keyWords, round);
    if(round != 0){
      if(round != 10){
        mixColumns(stateCypher, DECRYPT);
      }
      shiftRows(stateCypher, DECRYPT);
      InvSubBytes(stateCypher);
    }
  }

  printf("Decrypted: \n");
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      printf("%02x", stateCypher[4*i+j]);
    }
  }
  printf("\n\n");
}
