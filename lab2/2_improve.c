#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define STACK_INIT_SIZE 100
#define STACKINCREAMENT 10

unsigned int_stacksize;
int * int_stacktop_ptr;
int * int_stackbase_ptr;

#define test(stack) 1

#define InitStack(stack) stack ## _stackbase_ptr = (stack*)malloc(sizeof(stack) * STACK_INIT_SIZE);\
                         if(stack ## _stackbase_ptr) {/*To judge whether the malloc is successful or not*/\
                             stack ## _stacktop_ptr = stack ## _stackbase_ptr;\
                             stack ## _stacksize = STACK_INIT_SIZE;\
                         } else exit(0) 
/*
 * To initialize the stack.
 */

#define StackEmpty(stack) stack ## _stackbase_ptr == stack ## _stacktop_ptr ? true : false
/*
 * To judge whether a stack is empty or not.
 */

#define GetTop(stack, e) stack ## _stackbase_ptr == stack ## _stacktop_ptr ? false : (e = *(stack ## _stacktop_ptr-1), true)
/*
 * To get the top element of the stack
 */

#define ClearStack(stack) stack ## _stacktop_ptr = stack ## int_stackbase_ptr
/*
 * To empty a stack.
 */

#define DestoryStack(stack) free(stack ## _stackbase_ptr)
/*
 * To destory a stack thoroughly.
 */

#define StackLength(stack) stack ## _stacktop_ptr - stack ## _stackbase_ptr
/*
 * To calculate the length of the stack.
 */

#define Pop(stack, e) (stack ## _stackbase_ptr == stack ## _stacktop_ptr) ? false : (e = *(--stack ## _stacktop_ptr), true)
/*
 * If the stack is not empty, get the top number and delete it
 * Else return error.
 */

#define Push(stack, e) if(stack ## _stacktop_ptr - stack ## _stackbase_ptr >= stack ## _stacksize) {\
                           stack ## _stackbase_ptr = (stack*)realloc(stack ## _stackbase_ptr,\
                           (stack ## _stacksize + STACKINCREAMENT)*sizeof(stack));\
                           if(!stack ## _stackbase_ptr) exit(0);\
                           stack ## _stacktop_ptr = stack ## _stackbase_ptr + stack ## _stacksize;\
                           stack ## _stacksize += STACKINCREAMENT;}\
                           *(stack ## _stacktop_ptr++) = e
/*
 * If the stack is not full, insert the new element.
 * Else realloc the space and then insert the new element. 
 */

// #define Traverse(stack, e) while(stack ## _stacktop_ptr > stack ## _stackbase_ptr) {\
//                            e = *--stack ## _stacktop_ptr;\
//                            if(!strcmp(stack, 'tnode')) 1;}

unsigned char_stacksize;
char * char_stacktop_ptr;
char * char_stackbase_ptr;

typedef struct {
    int data[10];
    float x, y;
}tnode;

unsigned tnode_stacksize;
tnode * tnode_stacktop_ptr;
tnode * tnode_stackbase_ptr;

int main() {
    // To initialize 3 stacks.
    InitStack(int);
    InitStack(char);
    InitStack(tnode);

    // To test the integer stack.
    printf("Start to test the stack of int.\n");
    int x;
    if(Pop(int, x))
        printf("Successfully pop out of the stack. %d\n", x);
    else 
        printf("The stack is empty, can't be poped out.\n");

    if(StackEmpty(int))
        printf("The stack is empty, can't get the top number.\n");
    else {
        if(GetTop(int, x))
            printf("The top num of the stack is %d", x);
    }
    Push(int, 3);
    printf("There are %d elements in the stack.\n", StackLength(int));
    Push(int, 4);
    Push(int, 5);
    printf("There are %d elements in the stack.\n", StackLength(int));
    if(Pop(int, x))
        printf("Successfully pop out of the stack. %d\n", x);
    else
        printf("The stack is empty, can't be poped out.\n");

    printf("There are %d elements in the stack.\n", StackLength(int));

    if(StackEmpty(int))
        printf("The stack is empty, can't get the top number.\n");
    else if(GetTop(int, x))
        printf("The top  number of the stack is: %d\n", x);
    printf("There are %d elements in the stack.\n", StackLength(int));

    // The end of the test of integer.
    printf("The stack of integer has already been tested.\n\nStart to test the stack of char.\n");
    // Start to test the type of char:
    char y;
    if(Pop(char, y))
        printf("Successfully pop out of the stack. %c\n", y);
    else 
        printf("The stack is empty, can't be poped out.\n");

    if(StackEmpty(char))
        printf("The stack is empty, can't get the top number.\n");
    else {
        if(GetTop(char, y))
            printf("The top num of the stack is %c", y);
    }
    Push(char, 'a');
    printf("There are %d elements in the stack.\n", StackLength(char));
    Push(char, 'b');
    Push(char, 'c');
    printf("There are %d elements in the stack.\n", StackLength(char));
    if(Pop(char, y))
        printf("Successfully pop out of the stack. %c\n", y);
    else
        printf("The stack is empty, can't be poped out.\n");

    printf("There are %d elements in the stack.\n", StackLength(char));

    if(StackEmpty(char))
        printf("The stack is empty, can't get the top number.\n");
    else if(GetTop(char, y))
        printf("The top  number of the stack is: %c\n", y);
    printf("There are %d elements in the stack.\n", StackLength(char));

    // The end of the test of integer.
    printf("The stack of char has already been tested.\n\nStart to test the stack of tnode.\n");
    // Start to test the type of char:
    printf("Start to test the stack of tnode:\n");
    tnode z;
    // Traverse(tnode, e)
    return 0;
}