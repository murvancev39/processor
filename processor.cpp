#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stack.h"
#include "asm.h"
#include "processor.h"

int main ()
{
    processor_launch_and_execution_assembler("assembler.txt", "asm_native_code");
    return 0;
}

int processor_launch_and_execution_assembler(const char *asm_file, const char *native_code_file)
{
    if (asm_file == NULL)
    {
        printf ("ASSEMBLER FILE ABSENT\n");
        return -1;
    }

    if (native_code_file == NULL)
    {
        printf ("NATIVE CODE FILE ABSENT\n");
        return -1;
    }
    struct proc processor = {};
    assembler_to_native_code (asm_file, native_code_file);
    
    FILE * TEXTIN = fopen(native_code_file, "r");
    if (TEXTIN == NULL)
    {
        printf ("daun, TEXTIN IS NULL\n");
        return -1;
    }

    read_native_file_and_fill_arr (TEXTIN, &processor);
    // for (int i = 0; i < processor.native_code_capacity; i++)
    // {
    //     printf ("arr :_______%d and i : %d\n", processor.native_code_arr[i], i);
    // }
    // printf ("%d", processor.native_code_capacity);

    execution (&processor);
    free (processor.native_code_arr);
    return 0;
}

int read_native_file_and_fill_arr (FILE *TEXTIN, struct proc *processor)
{
    if (processor == NULL)
    {
        printf ("PROCESSOR POINTER IS NULL\n");
        return -1;
    }
    
    fseek (TEXTIN, 0, SEEK_END);
    size_t len = ftell (TEXTIN);
    // printf ("_______%d_________", len);

    fseek (TEXTIN, 0, SEEK_SET);

    char *native_file_arr = fill_native_file_arr (&len, TEXTIN);
    // for (int l = 0; l < len; l++)
    // {
    //     printf ("%c\n", native_file_arr[l]);
    // }
    if (native_file_arr == NULL)
    {
        printf ("NATIVE FILE ARRAY POINTR IS NULL\n");
        return -1;
    }
    // printf ("______++++++ len = %d", len);


    if (fill_processor_native_code_arr (processor, native_file_arr, len) == -1)
    {
        printf ("ERROR IN FILLING PROCESSOR NATIVE CODE ARRAY\n");
        return -1;
    }
    // printf ("errrrorororr");
    free (native_file_arr);
    return 1;
}

char *fill_native_file_arr (size_t *len, FILE *TEXTIN)
{
    if (len == NULL)
    {
        printf ("LEN WAS NOT FOUND");
        return NULL;
    }
    char *native_file_arr = (char*)calloc (*len, sizeof(char));
    if (native_file_arr == NULL)
    {
        printf ("MEMORY ALLOCATION ERROR 1\n");
        return NULL;
    }

    fread (native_file_arr, *len, 1, TEXTIN);

    size_t n_strings = 0;
    for (size_t i = 0; i < *len; i++)
    {        
        // printf ("char : %d , %c and i : %d\n", native_file_arr[i], native_file_arr[i], i);
        if (native_file_arr[i] == '\n')
        {
            n_strings++;
        }

        if ((native_file_arr [i] != '-') && ((native_file_arr [i] < '0') || (native_file_arr [i] > '9')))
        {
            native_file_arr [i] = '\0';
            
        }
    }
    *len = *len  - n_strings;
    // printf ("N____STRINGS : %d", n_strings);
    native_file_arr = (char *)realloc(native_file_arr, sizeof(char) * (*len));
    
    // processor->native_code_capacity = *len;
    // for (size_t i = 0; i < processor->native_code_capacity; i++)
    // {
    //     printf ("NEW char : %d , %c\n", native_file_arr[i], native_file_arr[i]);
    // }
    return native_file_arr;
}

int fill_processor_native_code_arr (struct proc *processor, char *native_file_arr, size_t len)
{
    
    int how_many_words = 0;
    // how_many_words = atoi(native_file_arr);
    sscanf (native_file_arr, "%d", &how_many_words);
    processor->native_code_capacity = how_many_words;
    

    processor->native_code_arr = (int *)calloc(processor->native_code_capacity, sizeof(int));
    if (processor->native_code_arr == NULL)
    {
        printf ("MEMORY ALLOCATION ERROR 2\n");
        return -1;
    }

    for (size_t i = 1; i < len; i++)
    {
        if (native_file_arr[i-1] == '\0')
        {
            // printf ("i : %d\n", i);
            const char *num = &(native_file_arr[i]);
            sscanf (num, "%d", &(processor->native_code_arr[processor->native_code_size]));
            // printf ("i : %d | size: %d | %d\n", i, processor->native_code_size, atoi(num));
            processor->native_code_size++;

        }
    }

    // printf ("capacity : %d", processor->native_code_capacity);
    processor->native_code_size = 0;
    return 1;
}

int execution (struct proc *processor)
{
    // printf ("34");

    if (processor == NULL)
    {
        printf ("PROCESSOR POINET IS NULL ___ line : %d\n", __LINE__);
        return -1;
    }
    
    if (processor->native_code_arr == NULL)
    {
        printf ("NATIVE CODE ARRAY POINTER IS NULL ___ line: %d\n", __LINE__);
        return -1;
    }

    sstack_t stack1 = {};
    processor->registers[26] = {};

    int a = 0;
    int b = 0;
    int num = 0;
    // double a1 = 1.0;


    stack_init (&stack1, 1, 1234567890);
    // printf ("\n______capacity : %d______\n", processor->native_code_capacity);
    while (processor->native_code_size < processor->native_code_capacity)
    {
        // stack_dump (&stack1, __FILE__, __LINE__);
        switch (processor->native_code_arr[processor->native_code_size])
        {
        case 1: // PUSH
            processor->native_code_size++;
            stack_push (&stack1, processor->native_code_arr[processor->native_code_size]);
            processor->native_code_size++;
            break;
        case 2: // POP
            processor->native_code_size++;
            stack_pop (&stack1);
            break;
        case 3: // OUT
            processor->native_code_size++;
            printf ("OUT : %d\n", stack_pop (&stack1));
            break;
        case 4: // IN
            processor->native_code_size++;
            printf ("\ninput num :");
            scanf ("%d", &num);
            stack_push (&stack1, num);
            break;
        case 5: // ADD
            processor->native_code_size++;
            a = stack_pop(&stack1);
            b = stack_pop(&stack1);
            stack_push (&stack1, a+b);
            break;
        case 6: // SUB
            processor->native_code_size++;
            a = stack_pop(&stack1);
            b = stack_pop(&stack1);
            stack_push (&stack1, b -a);
            break;
        case 7: // MUL
            processor->native_code_size++;
            a = stack_pop(&stack1);
            b = stack_pop(&stack1);
            stack_push (&stack1, b*a);
            break;
        case 8: // DIV
            processor->native_code_size++;
            a = stack_pop(&stack1);
            b = stack_pop(&stack1);
            stack_push (&stack1, b/a);
            break;
        case 9: // SQRT
            processor->native_code_size++;
            a = stack_pop(&stack1);
            stack_push (&stack1, ((int )sqrt(a * 1.0) % 1));
            break;
        case 10: // PUSHR
            // printf ("%d, %d", processor->native_code_arr[processor->native_code_size], processor->native_code_arr[processor->native_code_size+1]);
            processor->native_code_size++;
            processor->registers[processor->native_code_arr[processor->native_code_size]] = stack_pop(&stack1);
            processor->native_code_size++;
            // printf ("%d", processor->registers[processor->native_code_arr[processor->native_code_size]]);
            break;
        case 11: // POPR
            processor->native_code_size++;
            // printf ("size processor %d\n", processor->native_code_size);
            stack_push(&stack1, processor->registers[processor->native_code_arr[processor->native_code_size]]);
            processor->native_code_size++;
            break;
        case 12: // sqr
            processor->native_code_size++;
            a = stack_pop(&stack1);
            stack_push (&stack1, a * a);
            break;
        default:
            printf ("eblan");
            abort();
            break;
        }
    }
    // printf ("234567");
    return 0;
}
