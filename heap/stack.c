#include <stdio.h>
#include <stdlib.h>

#define MAX_CAP 1024

struct stack {
    int *val;
    int capacity;
    int tail;
};

void initstack(struct stack *s) {
    s->capacity = MAX_CAP;
    s->val = (int *)malloc(sizeof(int) * s->capacity);
    s->tail = 0; // 无元素，队尾指向 0
}


/**
 * @brief 入栈
 * @param elem 要入栈的元素
 */
void push(struct stack *s, int elem) {
    if(s->tail == s->capacity) { // 如果栈满
        s->capacity += MAX_CAP;
        s->val = (int *)realloc(s->val, sizeof(int)*s->capacity);
    }
    s->val[s->tail++] = elem;
}

void pop(struct stack *s, int *elem) {
    if(s->tail == 0) *elem = -114514;
    else   *elem = s->val[--s->tail];
}


int main() {

}