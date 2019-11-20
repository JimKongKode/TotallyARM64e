#include <mach-o/loader.h>
#include <mach-o/fat.h>
#include <mach-o/swap.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"

void patchFiles(FILE *file, bool is_fat, int fat_offset, int mach_offset) {
    
    static int data = 0x02;
    
    // go to the offset (where we need to patch)
    if (is_fat) {
        printf("write fat: 0x%x\n", fat_offset);
        fseek(file, fat_offset, SEEK_SET);
        fwrite(&data, 1, 1, file);
    }
    
    fseek(file, mach_offset, SEEK_SET);
    fwrite(&data, 1, 1, file);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: Enter a file to patch and an output file\n");
        return 1;
    }
    
    FILE *inputFile;
    if (!(inputFile = fopen(argv[1], "r"))) {
        printf("File could not be opened.\n");
        return 1;
    }
    
    FILE *fileToPatch;
    if (!(fileToPatch = fopen(argv[2], "wb+")))
    {
        printf("Output file could not be written to.\n");
        return 1;
    }
    
    // copy file to output
    int a;
    while( (a = fgetc(inputFile)) != EOF )
    {
        fputc(a, fileToPatch);
    }
    fclose(inputFile);
    
    
    uint32_t magic = read_uint32_t(fileToPatch, 0);
    
    bool is_fat = (magic == FAT_MAGIC || magic == FAT_CIGAM);
    
    int arch_offset = 0;
    int fat_arch_offset = 0;
    if (is_fat)
    {
        bool contains_arm64 = false;
        arch_offset = sizeof(struct fat_header);
        
        struct fat_header *fat_header = read_bytes(fileToPatch, 0, sizeof(struct fat_header));
        swap_fat_header(fat_header, 0);
        
        int nfat_archs = fat_header->nfat_arch;
        for (int i = 0; i < nfat_archs; i++)
        {
            struct fat_arch *arch = read_bytes(fileToPatch, arch_offset, sizeof(struct fat_arch));
            swap_fat_arch(arch, 1, 0);
            
            if (arch->cputype == CPU_TYPE_ARM64 && arch->cpusubtype == 2) {
                printf("This executable already has ARM64e! Fucking dumbass!\n");
                return 1;
            }
            
            if (arch->cputype == CPU_TYPE_ARM64 && arch->cpusubtype == 0)
            {
                printf("fat arch: 0x%x\n", arch_offset);
                
                contains_arm64 = true;
                fat_arch_offset = arch_offset;
                arch_offset = arch->offset;
                
                free(arch);
                break;
            }
            
            arch_offset += sizeof(struct fat_arch);
            free(arch);
        }
        
        if (!contains_arm64)
        {
            printf("This file has not been compiled for arm64!\n");
            return 1;
        }
        
        free(fat_header);
    }
    else if (magic != MH_MAGIC_64 && magic != MH_CIGAM_64)
    {
        printf("File does not look like an arm64 executable.\n");
        return 1;
    }
    
    // do the do
    patchFiles(fileToPatch, is_fat, fat_arch_offset + 7, arch_offset + 8);
    
    printf("File patched!\nEnjoy, fuckboy.\n");
    fclose(fileToPatch);
    return 0;
}
