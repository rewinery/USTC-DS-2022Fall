#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node * next;
} N;

void ListLength(N * p) {
    int count = 0;
    while(p = p->next) {
        count++;
    }
    printf("%d\n", count);
}

int main() {
    N * head;
    head = (N*)malloc(sizeof(N));
    head -> next = NULL;
    N * p = head;
    for(int i = 0; i < 9; i++) {
        p->next = (N*)malloc(sizeof(N));
        p = p->next;
        p->next = NULL;
    }
    ListLength(head);
}