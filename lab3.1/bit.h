#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef __BIT_H
#define __BIT_H

#define DE_BIT 100

typedef char ElemType;

typedef struct {
	ElemType inte[DE_BIT];
    int int_part;
	int len_int;
	int len_frac;
	ElemType * frac;
} De;


char Int_transfer_to_Char(int);
int Char_transfer_to_Int(char);
void De_Num_Display(De);
void De_trans_to_Bi(De, De);
bool Demical_Num_More_Than_0(De);
int division(De *, int);

char Int_transfer_to_Char(int a) {
    // Transfer the integer into character.
    return (char)(a + 48);
}

int Char_transfer_to_Int(char a) {
    // Transfer the character into integer.
    return (int)a - 48;
}

void De_Num_Display(De p) {
    // To present the number on the screen.
    for(int i = DE_BIT-p.len_int; i < DE_BIT; ++i) {
        printf("%d", Char_transfer_to_Int(p.inte[i]));
    }
    printf(".");
    for(int i = 0; i < p.len_frac; i++) {
        printf("%d", Char_transfer_to_Int(p.frac[i]));
    }
    printf("\n");
}

void De_trans_to_N_le_10(int N, De p1, De p2) {
    int count = 0;
    bool flag = true;

    // integer part.
    bool * p = p2.inte + DE_BIT - 1;
    while(flag) {
        
        flag = Demical_Num_More_Than_0(p1);
    }

    // fraction part.
    bool * p = p2.frac;
    for(int i = 0; i < p2.len_frac; i++) {
        
    }
}

bool Demical_Num_More_Than_0(De p) {
    // To judge whether the number is more than 0 or not.
    for(int i = DE_BIT-p.len_int; i < DE_BIT; i++) {
        if(p.inte[i] != '0') {
            return true;
        }
    }
    return false;
}

int division(De * p, int N) {
    
}

#endif /* bit.h */