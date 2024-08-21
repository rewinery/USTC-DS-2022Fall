#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STACK_INIT_SIZE 100
#define STACKINCREAMENT 10

unsigned int_stacksize;
int * int_stacktop_ptr;
int * int_stackbase_ptr;

#define InitStack(stack) stack ## _stackbase_ptr = (stack*)malloc(sizeof(stack) * STACK_INIT_SIZE);\
                         if(stack ## _stackbase_ptr) {/*To judge whether the malloc is successful or not*/\
                             stack ## _stacktop_ptr = stack ## _stackbase_ptr;\
                             stack ## _stacksize = STACK_INIT_SIZE;\
                         } else exit(0) 
/*
 * To initialize the stack.
 */

#define PrintStackLen(stack) printf("There are  %d  element(s) in the stack.\n", StackLength(stack))

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

/*
 * to display the elements of tnode.
 */
void Tnode_display(tnode p) {
    for(int i = 0; i < 10; i++) {
        printf("%d ", p.data[i]);
    }
    printf("\n%.3f %.3f\n", p.x, p.y);
}

/*
 * The two functions below are used to print that the operation is failed.
 */
void PrintEmptyPop() {
    printf("The stack is empty, can't be poped out.\n");
}

void PrintEmptyGettop() {
    printf("The stack is empty, can't get the top number.\n");
}

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
        PrintEmptyPop();

    if(StackEmpty(int))
        PrintEmptyGettop();
    else {
        if(GetTop(int, x))
            printf("The top num of the stack is %d", x);
    }
    Push(int, 3);
    PrintStackLen(int);
    Push(int, 4);
    Push(int, 5);
    PrintStackLen(int);
    if(Pop(int, x))
        printf("Successfully pop out of the stack. %d\n", x);
    else
        PrintEmptyPop();
    PrintStackLen(int);

    if(StackEmpty(int))
        PrintEmptyGettop();
    else if(GetTop(int, x))
        printf("The top  number of the stack is: %d\n", x);
    PrintStackLen(int);

    // The end of the test of integer.
    printf("The stack of integer has already been tested.\n\nStart to test the stack of char.\n");
    // Start to test the type of char:
    char y;
    if(Pop(char, y))
        printf("Successfully pop out of the stack. %c\n", y);
    else 
        PrintEmptyPop();

    if(StackEmpty(char))
        PrintEmptyGettop();
    else if(GetTop(char, y))
            printf("The top num of the stack is:  %c\n", y);
    Push(char, 'a');
    PrintStackLen(char);
    Push(char, 'b');
    Push(char, 'c');
    PrintStackLen(char);
    if(Pop(char, y))
        printf("Successfully pop out of the stack.  %c\n", y);
    else
        PrintEmptyPop();
    PrintStackLen(char);

    if(StackEmpty(char))
        PrintEmptyGettop;
    else if(GetTop(char, y))
        printf("The top  number of the stack is:  %c\n", y);
    PrintStackLen(char);

    // The end of the test of integer.
    printf("The stack of char has already been tested.\n\nStart to test the stack of tnode.\n");
    // Start to test the type of char:
    tnode z;
    if(Pop(tnode, z)) {
        printf("Successfully pop out of the stack: \n");
        Tnode_display(z);
    } else 
        PrintEmptyPop();

    if(StackEmpty(tnode))
        PrintEmptyGettop();
    else if(GetTop(tnode, z)) {
            printf("The top num of the stack is :");
            Tnode_display(z);
        }
    for(int i = 0; i < 10; ++i) {
        z.data[i] = i;
    }

    // To generate 3 elements of type 'tnode'
    z.x = 0.111;
    z.y = 0.222;
    Push(tnode, z);
    PrintStackLen(tnode);
    for(int i = 0; i < 10; ++i) {
        z.data[i] = i+10;
    }
    z.x = 0.333;
    z.y = 0.444;
    Push(tnode, z);
    for(int i = 0; i < 10; ++i) {
        z.data[i] = i+20;
    }
    z.x = 0.555;
    z.y = 0.666;
    Push(tnode, z);
    PrintStackLen(tnode);
    if(Pop(tnode, z)) {
        printf("Successfully pop out of the stack:\n");
        Tnode_display(z);
    } else
        PrintEmptyPop();
    PrintStackLen(tnode);

    if(StackEmpty(tnode))
        PrintEmptyGettop();
    else if(GetTop(tnode, z)) {
        printf("The top  element of the stack is:\n");
        Tnode_display(z);
    }
    PrintStackLen(tnode);
    printf("The stack of tnode has already been tested.\n");
    // Traverse(tnode, e)
    printf("...All done...\n\n");
    return 0;
}