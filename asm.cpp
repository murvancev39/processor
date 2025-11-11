#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "asm.h"

int read_asm (FILE *TEXTIN, struct asmm *assembler)
{
    fseek (TEXTIN, 0, SEEK_END);
    assembler->len = ftell (TEXTIN);
    fseek (TEXTIN, 0, SEEK_SET);
    // printf ("%lld\n", assembler->len);

    assembler->asm_arr = (char*)calloc (assembler->len, sizeof(char));
    if (assembler->asm_arr == NULL)
    {
        printf ("MEMORY ALLOCATION ERROR");
        return -1;
    }

    size_t a = fread (assembler->asm_arr, assembler->len, 1, TEXTIN);
    if (a == 0)
    {
        printf("INPUT FILE ERROR");
        return -1;
    }


    for (size_t i = 0; i < assembler->len; i++)
    {
        // printf ("___{%d , %c}___\n", assembler->asm_arr[i], assembler->asm_arr[i]);
    }

    for (size_t i = 0; i < assembler->len; i++)
    {        
        if ((assembler->asm_arr [i] == ' ') || (assembler->asm_arr [i] == '\n') || assembler->asm_arr[i] == '\r')
        {
            assembler->asm_arr [i] = '\0';
            // assembler->n_words++;
        }
    }
    return 1;
}

int push_asm_file (const char *native_code_file, struct asmm *assembler)
{
    // printf ("__________________\n");
    // for (int i = 0; i < assembler->len; i++)
    // {
    //     printf ("___*%d, %c *___\n", assembler->asm_arr[i], assembler->asm_arr[i]);
    // }
    
    
    
    FILE *TEXTOUT = fopen (native_code_file, "w");
    assert (assembler->len);

    for (size_t i = 0; i < assembler->len; i++)
    {
        if ((i == 0) || (((assembler->asm_arr [i-1]) < 32) && ((assembler->asm_arr [i]) >= 32)))
        {
            assembler->n_words++;
            // printf("_______NNNNN%d", assembler->n_words);
        }
    }
    fprintf (TEXTOUT, "%llu", assembler->n_words);

    for (size_t i = 0; i < assembler->len; i++)
    {
        if ((i == 0) || (((assembler->asm_arr [i-1]) < 32) && ((assembler->asm_arr [i]) >= 32)))
        {
            const char * arr = &(assembler->asm_arr [i]);
            
            // printf ("charchar %d,  %c\n", i, assembler->asm_arr [i + 3]);
            
            if (strcmp ( arr, "PUSH") == 0)
            {
                fprintf (TEXTOUT, "\n1 ");
            }

            else if (strcmp ( arr, "POP") == 0)
            {
                fprintf (TEXTOUT, "\n2 ");
            }

            else if (strcmp ( arr, "OUT") == 0)
            {
                fprintf (TEXTOUT, "\n3");
            }

            else if (strcmp ( arr, "IN") == 0)
            {
                fprintf (TEXTOUT, "\n4");
            }

            else if (strcmp ( arr, "ADD") == 0)
            {
                fprintf (TEXTOUT, "\n5");
            }

            else if (strcmp ( arr, "SUB") == 0)
            {
                fprintf (TEXTOUT, "\n6");
            }

            else if (strcmp ( arr, "MUL") == 0)
            {
                fprintf (TEXTOUT, "\n7");
            }
            
            else if (strcmp ( arr, "DIV") == 0)
            {
                fprintf (TEXTOUT, "\n8");
            }
            else if (strcmp ( arr, "SQRT") == 0)
            {
                fprintf (TEXTOUT, "\n9");
            }

            else if (strcmp ( arr, "PUSHR") == 0)
            {
                fprintf (TEXTOUT, "\n10");
            }

            else if (strcmp ( arr, "POPR") == 0)
            {
                fprintf (TEXTOUT, "\n11");
            }

            else if (strcmp ( arr, "SQR") == 0)
            {
                fprintf (TEXTOUT, "\n12");
            }

            else if (strcmp ( arr, "JMP") == 0)
            {
                fprintf (TEXTOUT, "\n13");
            }

            else
            {
                // printf ("asm : %s", arr);

                // printf ("strcmp : %d\n", strcmp ( arr, "OUT"));
                if ( (assembler->asm_arr [i] >= 'A') &&  (assembler->asm_arr [i] <= 'Z') && (assembler->asm_arr [i+1] == 'X'))
                {
                    fprintf (TEXTOUT, " %d", (assembler->asm_arr [i] - 'A'));
                }
                else
                {
                    fprintf (TEXTOUT, "%s", arr);
                }
            }
            // assembler->n_words++;
        }
    }
    fclose (TEXTOUT);
    return 1;
}

int assembler_to_native_code (const char *asm_file , const char *native_code_file)
{
    assert (asm_file);
    assert (native_code_file);

    struct asmm assembler = {};

    FILE *TEXTIN = fopen (asm_file, "rb");
    if (ferror (TEXTIN))
    {
        printf ("TEXT FILE ERROR");
    }

    if (read_asm (TEXTIN, &assembler) == -1)
    {
        return 0;
    }

    push_asm_file (native_code_file, &assembler);

    // printf ("%llu", assembler.n_words);

    free (assembler.asm_arr);
    fclose (TEXTIN);
    return 0; 
}
