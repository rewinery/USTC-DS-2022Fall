#include "ReadADT.h"
#include "operate.h"
#include "Menu.h"

void Get(NUM *);
void Get_x(NUM *);

int main() {
    int n, N;
    int flag;
    NUM_List * head = InitList(); // 用来存储输入的链�?
    NUM_List * rear = head;
    NUM_List * head_result = InitList(); // 用来存储输出的链�?
    NUM_List * rear_result = head_result;
    NUM_List * p1, *p2; // 中间�?
    while(1) {
        main_menu();
        scanf("%d", &flag);
        if(!flag) {
            break;
        }
        switch(flag) {
            case 1: {
                rear->next = NewList();
                rear = rear->next;
                PutNum(rear->num);
                break;
            } 
            case 2: {
                operate_menu();
                scanf("%d", &flag);
                if(!flag) {
                    break;
                }
                switch(flag) {
                    case 1: {
                        printf("加法运算�? A + B.\n");
                        printf("请按照你输入的顺序输入你想要的操作的第一个数 (A) 的序�?.\n");
                        scanf("%d", &n);
                        p1 = GetList(head, n);
                        p1 = Demical_to_N(p1->num, 2);
                        printf("请按照你输入的顺序输入你想要的操作的第二个数 (B) 的序�?.\n");
                        scanf("%d", &n);
                        p2 = GetList(head, n);
                        p2 = Demical_to_N(p2->num, 2);
                        if(p1->num.symbol == p2->num.symbol) {  // A �? B 同号时直接相�?
                            p1 = A_Plus_B(p1->num, p2->num);
                        } 
                        else if(p1->num.symbol) {             // A < 0, B > 0 时转换成 B - (-A)
                            p1->num.symbol = false;
                            p1 = A_Minus_B(p2->num, p1->num);
                        } 
                        else {                                // A > 0, B < 0 时转换成 A - (-B);
                            p2->num.symbol = false;
                            p1 = A_Minus_B(p1->num, p2->num);
                        }
                        printf("结果的二进制形式:\n");
                        PutNum(p1->num);
                        printf("结果的十进制形式：\n");
                        p1 = Binary_to_Demical(p1->num);
                        PutNum(p1->num);
                        printf("请输入你想要的进�? N:");
                        scanf("%d", &N);
                        p1 = Demical_to_N(p1->num, N);
                        printf("结果�? %d 进制形式", N);
                        PutNum(p1->num);
                        rear_result->next = p1;
                        rear_result = rear_result->next;
                        break;
                    }
                    case 2: {
                        printf("减法运算 A - B.\n");
                        printf("请按照你输入的顺序输入你想要的操作的第一个数 (A) 的序�?.\n");
                        scanf("%d", &n);
                        p1 = GetList(head, n);
                        p1 = Demical_to_N(p1->num, 2);
                        printf("请按照你输入的顺序输入你想要的操作的第二个数 (B) 的序�?.\n");
                        scanf("%d", &n);
                        p2 = GetList(head, n);
                        p2 = Demical_to_N(p2->num, 2);

                        if(p1->num.symbol == p2->num.symbol) {          // A �? B 同号时直接相�?
                            p1 = A_Minus_B(p1->num, p2->num);
                        } 
                        else if(!p1->num.symbol && p2->num.symbol) {  // A > 0, B < 0 时转换成 A + (-B)
                            p2->num.symbol = 0;
                            p1 = A_Plus_B(p1->num, p2->num);
                        } 
                        else if(p1->num.symbol && !p2->num.symbol) {  // A < 0, B > 0 时转换成 A + (-B)
                            p2->num.symbol = 1;
                            p1 = A_Plus_B(p1->num, p2->num);
                        }
                        printf("结果的二进制形式:\n");
                        PutNum(p1->num);
                        printf("结果的十进制形式：\n");
                        p1 = Binary_to_Demical(p1->num);
                        PutNum(p1->num);
                        printf("请输入你想要的进�? N:");
                        scanf("%d", &N);
                        p1 = Demical_to_N(p1->num, N);
                        printf("结果�? %d 进制形式", N);
                        PutNum(p1->num);
                        rear_result->next = p1;
                        rear_result = rear_result->next;
                        break;
                    }
                    case 3: {
                        printf("乘法运算 A * B.\n");
                        printf("请按照你输入的顺序输入你想要的操作的第一个数 (A) 的序�?.\n");
                        scanf("%d", &n);
                        p1 = GetList(head, n);
                        p1 = Demical_to_N(p1->num, 2);
                        printf("请按照你输入的顺序输入你想要的操作的第二个数 (B) 的序�?.\n");
                        scanf("%d", &n);
                        p2 = GetList(head, n);
                        p2 = Demical_to_N(p2->num, 2);

                        p1 = A_CHENG_B(p1->num, p2->num);

                        printf("结果的二进制形式:\n");
                        PutNum(p1->num);
                        printf("结果的十进制形式：\n");
                        p1 = Binary_to_Demical(p1->num);
                        PutNum(p1->num);
                        printf("请输入你想要的进�? N:");
                        scanf("%d", &N);
                        p1 = Demical_to_N(p1->num, N);
                        printf("结果�? %d 进制形式", N);
                        PutNum(p1->num);
                        rear_result->next = p1;
                        rear_result = rear_result->next;
                        break;
                    }
                    case 4: {
                        printf("请输入你想操作的数的下标。\n");
                        scanf("%d", &n);
                        p1 = GetList(head, n);
                        PutNum(p1->num);
                        rear_result->next = Demical_to_N(p1->num, 2);
                        rear_result = rear_result->next;
                        PutNum(rear_result->num);
                        break;
                    }
                    case 5: {
                        printf("请输入你想操作的数的下标。\n");
                        scanf("%d", &n);
                        p1 = GetList(head, n);
                        PutNum(p1->num);
                        printf("请输入你想要转化到的进制：\n");
                        scanf("%d", &N);
                        rear_result->next = Demical_to_N(p1->num, N);
                        rear_result = rear_result->next;
                        PutNum(rear_result->num);
                        break;
                    }
                    case 6: {
                        printf("请输入你想操作的数的下标�?");
                        printf("确保它是一个二进制数。\n");
                        scanf("%d", &n);
                        p1 = GetList(head, n);
                        printf("\n转换之前:\n");
                        PutNum(p1->num);
                        rear_result->next = Binary_to_Demical(p1->num);
                        rear_result = rear_result->next;
                        printf("\n转换之后:\n");
                        PutNum(rear_result->num);
                        break;
                    }
                    default: {
                        printf("Wrong Input.\n");
                        break;
                    }
                }
                break;
            }
            case 3: {
                printf("请输入多项式, 形如: 0.3333*x^3+0.2222*x^2+0.1111*x^1+0.00002*x^0(必须出现x的每一�?).\n");
                
                
                // 0.0*x^3+12.31379131*x^2+1.0*x^1+8.2137619836821388*x^0


                NUM_List * pp = (NUM_List *)malloc(sizeof(NUM_List));
                !pp ? exit(0) : (pp->next = NULL);
                NUM_List * xx = (NUM_List *)malloc(sizeof(NUM_List));
                !xx ? exit(0) : (xx->next = NULL);
                NUM_List * xx0 = (NUM_List *)malloc(sizeof(NUM_List));
                !xx0 ? exit(0) : (xx0->next = NULL);
                NUM_List * nnum = (NUM_List *)malloc(sizeof(NUM_List));
                !nnum ? exit(0) : (nnum->next = NULL);
                NUM num[4];
                NUM x;
                
                for(int i = 3; i >= 0; i--) {
                    Get(num + i);
                    getchar(); getchar(); getchar();
                }
                printf("请输�? x 的�?:"); // 1.612368921371923122414
                Get_x(&x);
                pp = Demical_to_N(num[0], 2);
                xx = xx0 = Demical_to_N(x, 2);
                for(int i = 1; i <= 3; i++) {
                    nnum = Demical_to_N(num[i], 2);
                    nnum = A_CHENG_B(nnum->num, xx->num);
                    xx = A_CHENG_B(xx->num, xx0->num);
                    pp = A_Plus_B(nnum->num, pp->num);
                }
                pp = Binary_to_Demical(pp->num);
                PutNum(pp->num);
                break;
            }
            default: {
                printf("Wrong Input.\n");
                break;
            }
        }
    }

    return 0;
}

void Get(NUM * num) {
    num->symbol = 0;
    char a[MAX_SIZE_INT];
    memset(a, '0', sizeof(a));
    char ch;
    int count = 0;
    scanf("%c", &ch);
    while(ch != '.') {
        if(ch == '-') num->symbol = 1;
        if(ch >= 48 && ch <= 57) {
            a[count++] = ch;
        }
        scanf("%c", &ch);
    }
    for(int i = 0, j = MAX_SIZE_INT - count; i < count; i++, ++j) {
        num->_int[j] = a[i];
    }
    for(int i = 0; i < MAX_SIZE_INT - count; i++) {
        num->_int[i] = '0';
    }
    count = 0;
    while(ch != '*') {
        if(ch >= 48 && ch <= 57) {
            num->_frac[count++] = ch;
        }
        scanf("%c", &ch);
    }
    for(int i = count; i < MAX_SIZE_FRAC; i++) {
        num->_frac[i] = '0';
    }
}


void Get_x(NUM * num) {
    num->symbol = 0;
    char a[MAX_SIZE_INT];
    memset(a, '0', sizeof(a));
    char ch;
    int count = 0;
    scanf("%c", &ch);
    while(ch != '.') {
        if(ch == '-') num->symbol = 1;
        if(ch >= 48 && ch <= 57) {
            a[count++] = ch;
        }
        scanf("%c", &ch);
    }
    for(int i = 0, j = MAX_SIZE_INT - count; i < count; i++, ++j) {
        num->_int[j] = a[i];
    }
    for(int i = 0; i < MAX_SIZE_INT - count; i++) {
        num->_int[i] = '0';
    }
    scanf("%s", num->_frac);
    count = strlen(num->_frac);
    for(int i = count; i < MAX_SIZE_FRAC; i++)
        num->_frac[i] = '0';
}