#include "stack.h"

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>


error_codes stack_init (sstack_t* stk1, size_t capacity, STACK_TYPE poison)
{
    assert (capacity > 0);
    assert (stk1);
    assert (stk1->data == NULL);
#ifndef NDEBUG
    stk1->poison = poison;
#endif
    stk1->capacity = capacity;
    stk1->data = (STACK_TYPE*)calloc(stk1->capacity, sizeof(*stk1->data));
    if (stk1->data == NULL)
    {
        return STACK_INITIALIZE_ERROR;
    }
    stk1->size = 0;

#ifndef NDEBUG
    for (size_t i = stk1->size ;i < stk1->capacity; i++ )
    {
        stk1->data[i] = stk1->poison;
    }
#endif

    VERIFY(stk1);
    return ALL_OKEY;
}

void stack_destructor (sstack_t* stk1)
{
    VERIFY(stk1);
    
    free (stk1->data);

#ifndef NDEBUG
    stk1->data = NULL;
    stk1->capacity = stk1->poison;
    stk1->size = stk1->poison;
#endif

    return;
}

void stack_push (sstack_t* stk1, STACK_TYPE num)
{
    VERIFY(stk1);

    if (stk1->size == stk1->capacity)
    {
        ARE_YOU_OKEY(resize(stk1));
    }

    stk1->data [stk1->size] = num;
    stk1->size++;

    VERIFY(stk1);
    return;
}

error_codes resize (sstack_t* stk1)
{
    stk1->capacity = (2 * stk1->capacity);
    STACK_TYPE* realloc_err = (STACK_TYPE*)realloc (stk1->data, stk1->capacity * sizeof (*stk1->data));
    if (realloc_err == NULL)
    {
        return STACK_RESIZE_ERROR;
    }
    stk1->data[stk1->size] = stk1->poison;
    VERIFY(stk1);
    return ALL_OKEY;
}

int stack_pop (sstack_t* stk1)
{
    VERIFY(stk1);
    STACK_TYPE pop_value = stk1->data [stk1->size - 1];
    
#ifndef NDEBUG
    stk1->data [stk1->size-1] = stk1->poison;
#endif

    stk1->size -=1;
    // printf ("size: %d\n", stk1->size);
    VERIFY(stk1);

    return pop_value;
}

#ifndef NDEBUG
ERROR_T verificator (sstack_t* stk1)
{
    ERROR_T res_error = 0;
    if (stk1 == NULL)
    {
        res_error |= NULL_STRUCT;
        printf ("1");
        return res_error;
    }

    if (stk1->data == NULL)
    {
        res_error |= NULL_DATA;
        return res_error;
        printf ("2");
    }

    if (stk1->size > stk1->capacity)
    {
        res_error |= INVALID_SIZE;
        printf ("3");
        return res_error;
    }

    for (size_t i = 0; i < stk1->size; i++)
    {
        // if (i < stk1->size)
        // {
            if (stk1->data[i] == stk1->poison)
            {
                res_error |= POISON_ERROR;
                printf ("4");
                return res_error;
                // printf ("4");
            }
        // }
        // else if (stk1->size < i)
        // {
        //     if (stk1->data[i] != stk1->poison)
        //     {
        //         res_error |= POISON_ERROR;
        //         printf ("5ю");
        //         return res_error;
        //         // printf ("5");
        //     }
        // }
    }
    // stack_dump(stk1, __FILE__, __LINE__);
    return res_error;
}

void stack_dump (sstack_t* stk1, const char *file_name, const int line_number)
{
    printf ("\nstack_dump was called from %s : %d\n", file_name, line_number);

    if (stk1 == NULL)
    {
        printf ("stack point is NULL\n");
        return;
    }

    printf ("Stack [%p]\n", stk1);

    if (stk1->data == NULL)
    {
        printf ("data pooint is NULL\n"
                "size = %llu\n"
                "capacity = %llu\n", stk1->size, stk1->capacity);
        return ;
    }

    printf ("{\n"
            "size = %llu\n"
            "capacity = %llu\n"
            "data [%p]\n", stk1->size, stk1->capacity, stk1->data);
    for (size_t i = 0; i < stk1->capacity; i++)
    {
        if (i <= stk1->size)
        {
            printf ("* [%llu] = %d\n", i, stk1->data[i]);
        }
        else
        {
            printf ("[%llu] = %d\n", i, stk1->data[i]);
        }
    }
    printf ("    }\n"
            "}\n");
    return ;
}
#endif 