#ifndef __READADT_H
#define __READADT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE_INT 100 // 整数部分长度
#define MAX_SIZE_FRAC 400 // 精度（小数部分长度）

#define ElemType char
// 以字符形式存储

typedef struct {
    bool symbol; // 符号, symbol = 1 时为 负， symbol = 0 时为正
    int len_int; // 整数部分已存储的长度
    int len_frac; // 小数部分已存储的长度
    ElemType * _int_rear; // 用来指向整数数组队尾的指针（从队尾开始存储）
    ElemType _frac[MAX_SIZE_FRAC];
    ElemType _int[MAX_SIZE_INT];
} NUM;
//用来存储数据

typedef struct ReadADT {
    NUM num;
    struct ReadADT * next;
} NUM_List;
// 用链表链式存储输入的每一个数和每一个运算的结果

void Read(NUM *);
void PutNum(NUM);
NUM NewNum();
NUM_List * InitList();
NUM_List * NewList();

void Read(NUM * num) {
    num->symbol = 0; // 初始化符号位
    num->len_frac = num->len_int = 0;
    int count = 0;
    ElemType ch, p[MAX_SIZE_INT];
    // 获取整数部分
    scanf("%c", &ch);
    while(ch != '.') { // 在小数点'.'前为整数部分
        if(ch == '-') {
            num->symbol = true; // use 1 to mark it '-'.
        }
        if(ch >= 48 && ch <= 57) { // 为防止奇怪的符号被读入，故加一个判断是不是在 0 - 9 之间
            p[count++] = ch;
        }
        scanf("%c", &ch);
    }
    
    num->len_int = count; // 获取整数部分长度
    for(int i = 0, j = MAX_SIZE_INT - count; i < count; i++, j++) {
        num->_int[j] = p[i];
    }
     // 存储在 _int 数组的最后面，靠近小数点
    
    // 获取小数部分
    count = 0;
    scanf("%s", num->_frac); // 读入小数部分
    num->len_frac = strlen(num->_frac);
    num->_frac[num->len_frac] = '0'; // 保证所有的符号都是数字。
}

NUM NewNum() {
    // 用来读入输入的数字
    NUM num;
    num._int_rear = num._int + MAX_SIZE_INT - 1;
    memset(num._int , '0', MAX_SIZE_INT * sizeof(ElemType));
    memset(num._frac, '0', MAX_SIZE_FRAC* sizeof(ElemType));
    Read(&num);
    return num;
}

NUM_List * InitList() {
    // 初始化链表头结点。
    NUM_List * p = (NUM_List *)malloc(sizeof(NUM_List));
    !p ? exit(0) : (p->next = NULL);
    return p;
}

NUM_List * NewList() {
    // 新建链表中的结点（读入的数字）
    printf("请输入数字:");
    NUM num = NewNum(); // Read the num first.
    NUM_List * p = (NUM_List *)malloc(sizeof(NUM_List));
    !p ? exit(0) : (p->next = NULL);
    p->num = num;
    return p;
}

void PutNum(NUM num)  {
    printf("小数部分有 %d 位\n", MAX_SIZE_FRAC);
    // printf("二进制下最大精度为：%d\n", MAX_SIZE_FRAC);
    printf("数字是: ");
    if(num.symbol) {
        printf("-");
    }
    bool flag = 0;
    for(int i = 0; i < MAX_SIZE_INT; ++i) {
        if(num._int[i] != '0') flag = 1;
        if(flag) printf("%c", num._int[i]);
    }
    if(!flag) printf("0");
    printf(".");
    for(int i = 0; i < MAX_SIZE_FRAC; ++i) {
        printf("%c", num._frac[i]);
    }
    printf("\n\n");
}
#endif /* ReadADT.h */