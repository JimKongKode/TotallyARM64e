#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "utils.h"

size_t getFileSize(FILE *file) {
    size_t orig = ftell(file);
    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    fseek(file, 0, orig);
    return fileSize;
}

void *read_bytes(FILE *file, int offset, int size)
{
    void *buffer = calloc(1, size);
    fseek(file, offset, SEEK_SET);
    fread(buffer, size, 1, file);
    return buffer;
}

uint32_t read_uint32_t(FILE *file, int offset)
{
    uint32_t r;
    fseek(file, offset, SEEK_SET);
    fread(&r, sizeof(uint32_t), 1, file);
    return r;
}
