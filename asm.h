#ifndef ASM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct asmm
{
    char* asm_arr;
    size_t len = 0;
    size_t n_words = 0;
};

// PUSH = 1
// POP = 2
// OUT = 3


int read_asm (FILE *TEXTIN, struct asmm *assembler);
int push_asm_file (const char *native_code_file, struct asmm *assembler);
int assembler_to_native_code (const char *asm_file , const char *native_code_file);

#endif