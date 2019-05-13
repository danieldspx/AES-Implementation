#ifndef _aes_h
#define _aes_h
  #include <stdbool.h>

  int* extractBits(int number,  int totalBits);
  int getSpecificBit(int number, int position);
  int calcTotalBits(int number);
  int getSBox(int line, int column);
  int getInvSBox(int line, int column);
  int charHexToInt(char hexChar);
  int textHexToInt(char *hexChar);
  int applyModularReduction(int number);
  int galoisFieldAdd(int firstVal, int secondVal);
  int bitShiftLeft(int number);
  int substitutionBox(int num, bool inverse);
  int getRoundCoef(int index);
  bool needModReduce(int number);
  bool isKey16Bytes(char *key);
  void addPadding(int startIndex, int *integers);
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
  void hexTextToArrayInt(char *text, int *array);
  int *encrypt(char *plainText, char *key);
  int *decrypt(char *cypher, char *key);
#endif
