#ifndef _utils_h
#define _utils_h

  #include <stdbool.h>
  #include <stdio.h>

  void replaceExtension(char *filename, char *newExtension);
  bool isBufferWithSpace(int index, int option);
  bool isBufferIndexBelowBlockSize(int index, int option);
  void writeOnFile(FILE *file, int *bufferResult, const char *format);
  bool compareFiles(char *filename1, char *filename2);

#endif
