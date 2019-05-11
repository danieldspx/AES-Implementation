//Pag 105 do livro
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define BLOCK_SIZE_BYTE 16
#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define DECRYPT true
#define ENCRYPT false

int* extractBits(int number,  int totalBits);
int getSpecificBit(int number, int position);
int calcTotalBits(int number);
int getSBox(int line, int column);
int getInvSBox(int line, int column);
int charHexToInt(char hexChar);
int applyModularReduction(int number);
int galoisFieldAdd(int firstVal, int secondVal);
int bitShiftLeft(int number);
int substitutionBox(int num, bool inverse);
int getRoundCoef(int index);
bool needModReduce(int number);
void blockTextToInt(char *text, int *integers);
void subBytes(int *stateCypher);
void InvSubBytes(int *stateCypher);
void shiftRows(int *stateCypher, bool inverse);
void shiftArrayToLeft(int *shiftText, int rounds, int startIndex);
void shiftArrayToRight(int *shiftText, int rounds, int startIndex);
void mixColumns(int *stateCypher, bool inverse);
void vectMatrixMultip(int **columnState, bool inverse);
void rotateArrayLeft(int *array, int size);
void calcAllSubkeys(int keyWords[][4], int *keyCypher);
void addRoundKey(int *stateCypher, int keyWords[][4], int round);

int main(){
  // char plainText[] = "LoremIpsumDorSit";//Must be 16 bytes
  int keyWords[44][4];
  int stateCypher[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
  int keyCypher[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
  calcAllSubkeys(keyWords, keyCypher);

  // blockTextToInt(plainText, stateCypher);

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

int *extractBits(int number, int totalBits){
  int *bits = (int*)malloc(totalBits*sizeof(int));
  for(int i = 0; i < totalBits; i++){
    bits[i] = getSpecificBit(number, i);
  }

  return bits;
}

int getSpecificBit(int number, int position){
  return (number & (1 << position)) >> position;
}

int calcTotalBits(int number){
  return (int)log2(number) + 1;
}

int getSBox(int line, int column){
  int SBOX[16][16] = {
    {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
    {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
    {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
    {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
    {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
    {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
    {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
    {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
    {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
    {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
    {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
    {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
    {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
    {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
    {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
    {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
  };
  return SBOX[line][column];
}

int getInvSBox(int line, int column){
  int InvSBOX[16][16] = {
    {0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB},
    {0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB},
    {0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E},
    {0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25},
    {0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92},
    {0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84},
    {0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06},
    {0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B},
    {0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73},
    {0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E},
    {0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B},
    {0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4},
    {0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F},
    {0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF},
    {0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61},
    {0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D}
  };
  return InvSBOX[line][column];
}

int charHexToInt(char hexChar){
  char *endptr;
  return strtol(&hexChar, &endptr, 16);
}

void shiftArrayToLeft(int *shiftText, int rounds, int startIndex){
  //Since I am handling the matrix as a one direction array, I need to make sure
  //that I change only what is correct. As if I was changing the line of a 4x4 matrix
  //|A0|...|A12|
  //|A1|...|A13|
  //|A2|...|A14|
  //|A3|...|A15|
  const int lineSize = 4;//Same as manipulation size
  const int columnSize = 4;
  int firstVal;
  int lastIndex = startIndex+(columnSize-1)*lineSize;
  for(int round = 0; round < rounds; round++){
    firstVal = shiftText[startIndex];
    for(int i=startIndex; i < lastIndex; i+=lineSize){//Ex.: Jump from A0 to A4 ... A12
      shiftText[i] = shiftText[i+lineSize];
    }
    shiftText[lastIndex] = firstVal;
  }
}

void shiftArrayToRight(int *shiftText, int rounds, int startIndex){
  //Since I am handling the matrix as a one direction array, I need to make sure
  //that I change only what is correct. As if I was changing the line of a 4x4 matrix
  //|A0|...|A12|
  //|A1|...|A13|
  //|A2|...|A14|
  //|A3|...|A15|
  const int lineSize = 4;//Same as manipulation size
  const int columnSize = 4;
  int lastVal;
  int lastIndex = startIndex+(columnSize-1)*lineSize;
  for(int round = 0; round < rounds; round++){
    lastVal = shiftText[lastIndex];
    for(int i=lastIndex; i > startIndex; i-=lineSize){//Ex.: Jump from A0 to A4 ... A12
      shiftText[i] = shiftText[i-lineSize];
    }
    shiftText[startIndex] = lastVal;
  }
}

bool needModReduce(int number){
  //0x100 = 0001 0000 0000
  //Check if the 9th bit is 1. If so we need to reduce it
  //because its degree is 8 and in GF(2^8) the max is 7
  return number&0x100;
}

int substitutionBox(int num, bool inverse){
  char charInHex[3];
  long int line, column;
  sprintf(charInHex, "%02x", num);
  line = charHexToInt(charInHex[0]);
  column = charHexToInt(charInHex[1]);
  if(inverse){
    return getInvSBox(line, column);
  } else {
    return getSBox(line, column);
  }
}

int applyModularReduction(int number){
  //Modular reduction is used according to Galois Field
  //and using P(x) = x^8 + x^4 + x^3 + x + 1
  //Since  the number is << 1 we do not need the x^8 here, but we
  //sum the others coefficients wich is a simple bitwise XOR.
  //In GF(2^8), P(x) is '100011011' = 0x11B
  return number^0x11B;
}

int galoisFieldAdd(int firstVal, int secondVal){
  //Additions in the GF(2^8) wich are simple XOR operations
  return firstVal^secondVal;
}

int bitShiftLeft(int number){
  //Number has 8 bits before the shift
  int shifted = number<<1;
  if(getSpecificBit(shifted, 8)){
    //If in the index 8 we have a 1 then we should remove it
    //Because our number has 8 bits and the last index is 7
    shifted ^= 256;//To remove the 1 in the Index 8
  }
  return shifted;
}

int getRoundCoef(int index){
  int rc[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};
  return rc[index];
}

int galoisFieldMultiply(int a, int b) {
    int sum = 0;
    while (b > 0) {
        if (b & 1) sum = sum ^ a;             //If last bit of b is 1, add a to the sum
        b = b >> 1;                           //divide b by 2, discarding the last bit
        a = a << 1;                           //multiply a by 2
        if (needModReduce(a)) a = applyModularReduction(a);
    }
    return sum;
}

void blockTextToInt(char *text, int *integers){
  for(int i=0; i<BLOCK_SIZE_BYTE; i++){
    integers[i] = (int)text[i];
  }
}

void subBytes(int *stateCypher){
  //This is the Substitution Layer through the Lookup table
  for(int i = 0; i < BLOCK_SIZE_BYTE; i++){
    stateCypher[i] = substitutionBox(stateCypher[i], false);
  }
}

void InvSubBytes(int *stateCypher){
  //This is the Inverse Substitution Layer through the Lookup table
  for(int i = 0; i < BLOCK_SIZE_BYTE; i++){
    stateCypher[i] = substitutionBox(stateCypher[i], true);
  }
}

void shiftRows(int *stateCypher, bool inverse){
  //The specification says that the manipulation happens in a 4x4 array.
  //Instead, I use 1x16 wich is easyer to compute and pass arround
  int startIndex;
  int rounds = BLOCK_SIZE_BYTE/4;
  for(int round = 0; round < rounds; round++){
    startIndex = round;
    if(inverse){
      shiftArrayToRight(stateCypher, round, startIndex);
    } else {
      shiftArrayToLeft(stateCypher, round, startIndex);
    }
  }
}

void vectMatrixMultip(int **columnState, bool inverse){
  //Vector Matrix Multiplication for 2 number in a given line
  // ( C0 )   (02 03 01 01)   (B0)
  // ( C1 ) = (01 02 03 01) * (B1)
  // ( C2 )   (01 01 02 03)   (B2)
  // ( C3 )   (03 01 01 02)   (B3)
//Remember that data is distributed UP to DOWN and LEFT to RIGHT
  const int elemSize = BLOCK_SIZE_BYTE/4;
  int intVal, lineColState, multiVal, multiSum;//multiVal = Multiplication Value
  int resultColumnState[4];
  int mixColumnConsts[4][4] = {
    {02, 03, 01, 01},
    {01, 02, 03, 01},
    {01, 01, 02, 03},
    {03, 01, 01, 02}
  };
  int invMixColumnConsts[4][4] = {
    {0x0E, 0x0B, 0x0D, 0x09},
    {0x09, 0x0E, 0x0B, 0x0D},
    {0x0D, 0x09, 0x0E, 0x0B},
    {0x0B, 0x0D, 0x09, 0x0E}
  };
  for(int line = 0; line < elemSize; line++){
    multiSum = 0;
    for(int column = 0; column < elemSize; column++){
      lineColState = column;
      intVal = *(columnState[lineColState]);
      if(inverse){
        multiVal = galoisFieldMultiply(intVal, invMixColumnConsts[line][column]);
      } else {
        multiVal = galoisFieldMultiply(intVal, mixColumnConsts[line][column]);
      }
      multiSum = galoisFieldAdd(multiSum, multiVal);
    }
    resultColumnState[line] = multiSum;
  }

  for(int line = 0; line < elemSize; line++){
    *(columnState[line]) = resultColumnState[line];
  }
}

void mixColumns(int *stateCypher, bool inverse){
  //Perform Mix Column for all columns
  int startIndex, lastIndex;
  int columns = BLOCK_SIZE_BYTE/4;
  int *intGroup[4];
  for(int column = 0; column < columns; column++){
    startIndex = column*4;//4 is the manipulation size
    lastIndex = startIndex+4;
    for(int i=startIndex, j = 0; i < lastIndex; i++, j++){//Ex.: Goes from B0 to B1 ... B3
      intGroup[j] = &stateCypher[i];
    }
    vectMatrixMultip(intGroup, inverse);
  }
}

void rotateArrayLeft(int *array, int size){
  int lastIndex = size - 1;
  int firstVal = array[0];
  for(int i=0; i < lastIndex; i++){
    array[i] = array[i+1];
  }
  array[lastIndex] = firstVal;
}

void calcAllSubkeys(int keyWords[][4], int *keyCypher){
  int lastWordSubkey[4], roundCoef;

  for(int i = 0; i < 4; i++){//Copy the keyCypher to the Word array
    for(int j = 0; j < 4; j++){
      keyWords[i][j] = keyCypher[4*i + j];
    }
  }

  for(int round = 1; round <= 10; round++){
    for(int i = 0; i < 4; i++){
      lastWordSubkey[i] = keyWords[4*round - 1][i];
    }
    rotateArrayLeft(lastWordSubkey, 4);
    for(int i = 0; i<4; i++){
      lastWordSubkey[i] = substitutionBox(lastWordSubkey[i], false);
    }
    roundCoef = getRoundCoef(round-1);
    lastWordSubkey[0] = galoisFieldAdd(lastWordSubkey[0], roundCoef);
    for(int i = 0; i < 4; i++){//Calc Root Word
      keyWords[4*round][i] = galoisFieldAdd(lastWordSubkey[i], keyWords[4*(round - 1)][i]);
    }
    for(int j = 1; j < 4; j++){//Calc others words of the current subkey
      for(int i = 0; i < 4; i++){
        keyWords[4*round + j][i] = galoisFieldAdd(keyWords[4*round + j - 1][i], keyWords[4*(round - 1)+j][i]);
      }
    }
  }
}

void addRoundKey(int *stateCypher, int keyWords[][4], int round){
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      stateCypher[4*i+j] = galoisFieldAdd(stateCypher[4*i+j], keyWords[4*round+i][j]);
    }
  }
}
