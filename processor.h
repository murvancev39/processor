#ifndef PROCESSOR_H
#include "stack.h"
#include <stdio.h>


struct proc
{
    sstack_t stk;
    int *native_code_arr;
    size_t native_code_size;
    size_t native_code_capacity;
    STACK_TYPE registers[];
};

int read_native_file_and_fill_arr (FILE *TEXTIN, struct proc *processor);
int fill_processor_native_code_arr (struct proc *processor, char *native_file_arr, size_t len);
char *fill_native_file_arr (size_t *len, FILE *TEXTIN);
int execution (struct proc *processor);
int processor_launch_and_execution_assembler(const char *asm_file, const char *native_code_file);

#endif