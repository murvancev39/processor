#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdint.h>


typedef int STACK_TYPE;

const int opening = 0xBEBEBE;
const int ending = 0xDABAB7;

struct stack_t
{
    STACK_TYPE *data;
    size_t size;
    size_t capacity;
#ifndef NDEBUG
    STACK_TYPE poison;
#endif
};

typedef struct stack_t sstack_t;

enum error_codes
{
    ALL_OKEY                = 0b00000,
    NULL_STRUCT             = 1 << 0,
    NULL_DATA               = 1 << 1,
    INVALID_SIZE            = 1 << 2,
    STACK_INITIALIZE_ERROR  = 1 << 3,
    STACK_RESIZE_ERROR      = 1 << 4,
    POISON_ERROR            = 1 << 5
};




typedef uint64_t ERROR_T;

error_codes stack_init (sstack_t* stk1, size_t capacity, STACK_TYPE poison);
void stack_destructor (sstack_t* stk1);
void stack_push (sstack_t* stk1, STACK_TYPE num);
error_codes resize (sstack_t* stk1);
int stack_pop (sstack_t* stk1);
#ifndef NDEBUG
ERROR_T verificator (sstack_t* stk1);
void stack_dump (sstack_t* stk1, const char *file_name, const int line_number);
#endif

#ifndef NDEBUG

// TODO strerror
#define ARE_YOU_OKEY(func, ...) do  {                                       \
                                        error_codes error = func;           \
                                        if (error == STACK_INITIALIZE_ERROR)\
                                        {                                   \
                                            fprintf (stderr, "STACK INITIALIZE ERROR INSUFFICIENT MEMORY");  \
                                            __VA_ARGS__;                    \
                                            assert (false);                 \
                                        }                                   \
                                        if (error == STACK_RESIZE_ERROR)    \
                                        {                                   \
                                            printf ("STACK RESIZE ERROR INSUFFICIENT MEMORY");               \
                                            __VA_ARGS__;                    \
                                            assert (false);                 \
                                        }                                   \
                                    } while(false);                         \

#define STACK_INIT(stk1, capacity, POISON)  stack_init (&stk1, capacity, POISON)

#define VERIFY(stk)       do{                                               \
                                ERROR_T res = verificator(stk);             \
                                if (res != 0)                               \
                                {                                           \
                                    stack_dump (stk,  __FILE__, __LINE__);  \
                                    assert(false);                          \
                                }                                           \
                            } while(false)                                  \

#else /*NDEBUG*/

#define ARE_YOU_OKEY(func, ...) do{func;} while (false)

#define STACK_INIT(stk1, capacity, POISON) stack_init (&stk1, capacity, 0)

#define VERIFY(stk) do{} while(false)

#endif /*NDEBUG*/

#endif
