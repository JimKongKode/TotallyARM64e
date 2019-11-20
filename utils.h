#ifndef UTILS_INCLUDE
#define UTILS_INCLUDE
#include <stdlib.h>
#include <stdio.h>
size_t getFileSize(FILE *file);
void *read_bytes(FILE *file, int offset, int size);
uint32_t read_uint32_t(FILE *file, int offset);
#endif
