#ifndef __OPERATE_H__For_Scale_Convert
#define __OPERATE_H

int Char_to_Int(char);
char Int_to_Char(int);
NUM_List * Demical_to_N(NUM, int);
void Init_N_Scale(NUM *);
char All_Devided_by_N(NUM *, int);
char All_time_N(NUM *, int);
NUM_List * GetList(NUM_List *, int);
NUM_List * Binary_to_Demical(NUM);
NUM_List * A_Plus_B(NUM, NUM);
NUM_List * A_Minus_B(NUM, NUM);
NUM_List * A_CHENG_B(NUM, NUM);
bool Judge(NUM, NUM);
bool NUM_Empty(NUM);
bool FracEmpty(NUM);

int Char_to_Int(char a) {
    return a - 48;
}

char Int_to_Char(int a) {
    return a + 48;
}

void Init_N_Scale(NUM * Bi) {
    memset(Bi->_int , '0', sizeof(ElemType) * MAX_SIZE_INT );
    memset(Bi->_frac, '0', sizeof(ElemType) * MAX_SIZE_FRAC);
}

// 实现十进制数向 N 进制数的转换。
NUM_List * Demical_to_N(NUM Demical, int N) {
    NUM N_scale; // 建立 N 进制数.
    Init_N_Scale(&N_scale); // 初始化 N 进制数
    NUM_List * p = (NUM_List *)malloc(sizeof(NUM_List)); // 建立一个链表结点
    !p ? exit(0) : (p->next = NULL);
    ElemType * rear = N_scale._int + MAX_SIZE_INT - 1;
    
    N_scale.symbol = Demical.symbol; // 初始化符号位

    // 先转换整数部分
    while(!NUM_Empty(Demical)) { 
        *rear-- = All_Devided_by_N(&Demical, N);
    } // 指针运算，增加速度

    
    for(int i = 0; i < MAX_SIZE_FRAC; i++) {
        N_scale._frac[i] = All_time_N(&Demical, N);
        if(FracEmpty(Demical)) break;
    }
    p->num = N_scale;
    return p;
}

// 把整数部分除以 N.
char All_Devided_by_N(NUM * num, int N) {
    int n; // n 表示余数。
    int s[MAX_SIZE_INT];
    int ss[MAX_SIZE_FRAC];
    for(int i = 0; i < MAX_SIZE_INT; ++i) {
        s[i] = ss[i] = num->_int[i] - 48;
    }
    for(int i = 0; i < MAX_SIZE_INT; ++i) {
        ss[i] /= N;
        n = s[i] - ss[i] * N;
        if(n && i < MAX_SIZE_INT-1) s[i+1] = ss[i+1] += n * 10;
    }
    for(int i = 0; i < MAX_SIZE_INT; i++) {
        num->_int[i] = ss[i] + 48;
    }
    if(n < 10) {
        return n + 48; // Return type is char.
    } else {
        return n + 87; // a -> 10, b -> 11, ...
    }
}

// 整体加 N
char All_time_N(NUM * num, int N) {
    int s[MAX_SIZE_FRAC], n;
    for(int i = 0; i < MAX_SIZE_FRAC; i++) {
        s[i] = (num->_frac[i] - 48) * N; 
    } // 每个数乘以 N
    for(int i = MAX_SIZE_FRAC - 1; i >= 0; i--) {
        n = s[i] / 10;
        if(n) {
            s[i-1] += n; // 进位
            s[i] %= 10; // 修正 s[i].
        }
        num->_frac[i] = s[i] + 48;
    }
    if(n < 10)
        return n + 48;
    else 
        return n + 87;
}

bool FracEmpty(NUM p) {
    for(int i = 0; i < MAX_SIZE_FRAC; i++) {
        if(p._frac[i] != '0')
            return 0;
    }
    return 1;
}

// 判断整数部分是否为空。
bool NUM_Empty(NUM num) {
    // To judge whether the integer part of the number is 0 or not.
    for(int i = MAX_SIZE_INT-1; i >= 0; i--) {
        if(num._int[i] >= 49 && num._int[i] <= 57)
            return 0;
    }
    return 1;
}

// 获得对应结点
NUM_List * GetList(NUM_List * head, int n) {
    NUM_List * p = head;
    for(int i = 0; i < n; ++i) {
        p = p->next;
    }
    return p;
}

void All_double_2(int * s) {
    int n;
    for(int i = MAX_SIZE_INT-1; i >= 0; i--) {
        s[i] *= 2;
    } //第一次循环逐位乘 2
    for(int i = MAX_SIZE_INT-1; i >= 0; --i) {
        n = s[i] / 10;
        if(n && i > 0) {
            s[i] %= 10;
            s[i-1] += n;
        }
    } // 第二次循环进位
}

void All_plus(int * result, int * s) {     
    int n;
    for(int i = MAX_SIZE_INT-1; i >= 0; i--) {
        result[i] += s[i];
    } // 先逐位加
    for(int i = MAX_SIZE_INT-1; i > 0; i--) {
        n = result[i] / 10;
        if(n && i > 0) {
            result[i] %= 10;
            result[i-1] += n;
        }
    }
}

void Frac_plus(int * result, int * s) {
    int n;
    for(int i = 0, j = MAX_SIZE_INT; i < MAX_SIZE_FRAC; i++, j++) {
        result[j] += s[i];
    } //先逐位加
    for(int i = MAX_SIZE_FRAC + MAX_SIZE_INT- 1; i >= MAX_SIZE_INT; i--) {
        n = result[i] / 10;
        if(n) {
            result[i] %= 10;
            result[i-1] += n;
        } // 二进制小数永远不可能大于 1，不需要特殊考虑
    }
}

void All_devided(int * f) {
    float s[MAX_SIZE_FRAC];
    for(int i = 0; i < MAX_SIZE_FRAC; i++) {
        s[i] = f[i];
        s[i] /= 2;
        f[i] = 0;
    }
    for(int i = 0; i < MAX_SIZE_FRAC; i++) {
        f[i] += (int)s[i];
        if(i < MAX_SIZE_FRAC-1) f[i+1] = (int)(10*s[i]) % 10; // 至多产生一位小数。 
    }
}

NUM_List * Binary_to_Demical(NUM num) { 
    int len = MAX_SIZE_FRAC + MAX_SIZE_INT; // 总长度
    int s[MAX_SIZE_INT]; // 用来递归计算
    int result[len]; // 存储结果
    memset(s, 0, sizeof(s));
    memset(result, 0, sizeof(result));
    s[MAX_SIZE_INT - 1] = 1; // 2^0


    for(int i = MAX_SIZE_INT-1; i >= 0; i--) {
        if(num._int[i] == '1') { // 如果第 i 个位置是 1， 则加上 2^{MAX_SIZE_INT-i}
            All_plus(result, s);
        }
        All_double_2(s); // 每次循环过后乘 2 ，进入下一次循环。
    }

    int f[MAX_SIZE_FRAC];
    memset(f, 0, sizeof(f));
    f[0] = 5; // 2^{-1}
    for(int i = 0; i < MAX_SIZE_FRAC; ++i) {
        if(num._frac[i] == '1') Frac_plus(result, f);
        All_devided(f);
    }


    NUM num1;
    num1.len_int = 0;
    bool flag = 0;
    for(int i = 0; i < MAX_SIZE_INT; i++) {
        num1._int[i] = result[i] + 48;
        if(result[i]) flag = 1;
        if(flag) num1.len_int++;
    }
    for(int i = 0, j = MAX_SIZE_INT; i < MAX_SIZE_FRAC; i++, j++) {
        num1._frac[i] = result[j] + 48;
    }
    num1.len_frac = MAX_SIZE_FRAC;
    NUM_List * p = (NUM_List *)malloc(sizeof(NUM_List));
    !p ? exit(0) : (p->next = NULL);
    num1.symbol = num.symbol;
    p->num = num1;
    return p;
}

bool plus(bool A, bool B, bool C, int * D) {
    int count = 0;
    if(A) count++;
    if(B) count++;
    if(C) count++;
    bool a;
    if(count == 3) {
        *D = 1;
        return 1;
    } else if(count == 2) {
        *D = 0;
        return 1;
    } else if(count == 1) {
        *D = 1;
        return 0;
    } else {
        *D = 0;
        return 0;
    }
}

NUM_List * A_Plus_B(NUM A, NUM B) {
    // 将 A 和 B 转化为 二进制数 之后再调用此函数。
    bool A_frac[MAX_SIZE_FRAC], B_frac[MAX_SIZE_FRAC], A_int[MAX_SIZE_INT], B_int[MAX_SIZE_INT];
    for(int i = 0; i < MAX_SIZE_INT; i++) {
        A_int[i] = A._int[i] == '1' ? 1 : 0;
        B_int[i] = B._int[i] == '1' ? 1 : 0;
    } 
    for(int i = 0; i < MAX_SIZE_FRAC; i++) {
        A_frac[i] = A._frac[i] == '1' ? 1 : 0;
        B_frac[i] = B._frac[i] == '1' ? 1 : 0;
    } // 将字符串转换为数字存储，便于操作。
    int C_frac[MAX_SIZE_FRAC], C_int[MAX_SIZE_INT];
    bool b = 0;
    for(int i = MAX_SIZE_FRAC-1; i >= 0; i--) {
        b = plus(A_frac[i], B_frac[i], b , C_frac + i);
    } // 先计算小数部分，从右往左运算
    for(int i = MAX_SIZE_INT-1; i >= 0; i--) {
        b = plus(A_int[i], B_int[i], b, C_int + i);
    } // 再计算整数部分
    // 接下来计算整数部分长度
    bool flag = 0;
    int count = 0;
    for(int i = 0; i < MAX_SIZE_INT; i++) {
        if(C_int[i]) {
            flag = 1;
        }
        if(flag) {
            count++;
        }
    }
    NUM_List * p = (NUM_List *)malloc(sizeof(NUM_List));
    !p ? exit(0) : (p->next = NULL);
    NUM num;
    num.len_int = count;
    num.len_frac = MAX_SIZE_FRAC;
    for(int i = 0; i < MAX_SIZE_INT; i++) {
        num._int[i] = C_int[i] + 48;
    }
    for(int i = 0; i < MAX_SIZE_FRAC; ++i) {
        num._frac[i] = C_frac[i] + 48;
    }
    num.symbol = A.symbol;
    p->num = num;
    return p;
}

bool _plus(bool A, bool B, bool C, bool * D) {
    int count = 0;
    if(A) count++;
    if(B) count++;
    if(C) count++;
    bool a;
    if(count == 3) {
        *D = 1;
        return 1;
    } else if(count == 2) {
        *D = 0;
        return 1;
    } else if(count == 1) {
        *D = 1;
        return 0;
    } else {
        *D = 0;
        return 0;
    }
}

NUM_List * A_Minus_B(NUM A, NUM B) {
    /*
     * 实现 A - B 
     * 将 A 和 B 全都去掉小数点
     * B 逐位取反再加 1.
     * C = A + B
     */
    NUM C; //新建一个用来存储
    int len = MAX_SIZE_FRAC + MAX_SIZE_INT; // 数组总长度
    bool a[len], b[len], c[len]; // c = a - b.
    for(int i = 0; i < MAX_SIZE_INT; i++) {
        a[i] = A._int[i] == '1' ? 1 : 0;
        b[i] = B._int[i] == '1' ? 0 : 1; // b[i] 逐位取反
    } 
    for(int i = 0, j = MAX_SIZE_INT; i < MAX_SIZE_FRAC; i++, j++) {
        a[j] = A._frac[i] == '1' ? 1 : 0;
        b[j] = B._frac[i] == '1' ? 0 : 1; // b[i] 逐位取反
    } // 将字符串转换为数字存储，便于操作。

    bool n = 0; // n 表示进位
    int i; // 开始的位置
    n = _plus(b[len-1], 1, n, b+len-1); // 对 b[i] 整个数组加上 1.
    for(i = len - 2; i >= 0; i--) {
        n = _plus(b[i], 0, n, b+i);
    }
    n = 0;
    for(int i = len-1; i >= 0; i--) {
        n = _plus(a[i], b[i], n, c+i);
    } // 计算 c

    C.symbol = A.symbol;

    if(c[0]) { // 如果 c 的首位是 1 表示 c 为负， 逐位取反再加 1
        C.symbol = !C.symbol;
        for(int i = 0; i < len; i++) {
            c[i] = !c[i];
        } // 逐位取反
        n = 0;
        i = len - 1; // 开始的位置
        n = _plus(c[i], 1, n, &c[i]); // 对 c[] 整个数组加上 1.
        i--; 
        for(; i >= 0; i--) {
            n = _plus(c[i], 0, n, &c[i]);
            if(!n) break;
        }
    }


    for(i = 0; i < MAX_SIZE_INT; i++) {
        C._int[i] = c[i] + 48;
    }
    for(int i = 0, j = MAX_SIZE_INT; i < MAX_SIZE_FRAC; i++, j++) {
        C._frac[i] = c[j] + 48;
    }
    NUM_List * p = (NUM_List *)malloc(sizeof(NUM_List));
    !p ? exit(0) : (p->next = NULL);
    p->num = C;
    C.len_frac = MAX_SIZE_FRAC;
    return p;
}

void _CHENG_Plus(int * sum, int * a, int k) {
    int start_a, start_sum, len = MAX_SIZE_FRAC * 2 + MAX_SIZE_INT;
    if(k <= MAX_SIZE_INT) {
        start_a = MAX_SIZE_INT - k;
        for(int i = start_a, j = 0; i < len; ++i, j++) {
            sum[j] += a[i];
        }
    }
    else {
        start_sum = k - MAX_SIZE_INT;
        for(int i = 0, j = start_sum; j < len; ++i, ++j) {
            sum[j] += a[i];
        }
    }
}

NUM_List * A_CHENG_B(NUM A, NUM B) {
    // 传入二进制数 
    int len = MAX_SIZE_FRAC * 2 + MAX_SIZE_INT ;
    int a[len], b[len], c[len];
    // for(int i = MAX_SIZE_INT-5; i < MAX_SIZE_INT; i++) {
    //     printf("%c", B._int[i]);
    // }
    // printf(".");
    // for(int i = 0; i <5; i++) {
    //     printf("%c", B._frac[i]);
    // }
    // printf("\n\n");
    for(int i = 0; i < MAX_SIZE_INT; ++i) {
        a[i] = A._int[i] == '1' ? 1 : 0;
        b[i] = B._int[i] == '1' ? 1 : 0;
        c[i] = 0;
    }
    for(int i = 0, j = MAX_SIZE_INT; i < MAX_SIZE_FRAC; i++, j++) {
        a[j] = A._frac[i] == '1' ? 1 : 0;
        b[j] = B._frac[i] == '1' ? 1 : 0;
        c[j] = 0;
    } // 转换为整型数组
    for(int i = MAX_SIZE_FRAC+MAX_SIZE_INT; i < len; i++) {
        a[i] = b[i] = c[i] = 0;
    }

            // for(int i = MAX_SIZE_INT-5; i <= MAX_SIZE_INT+5; i++) {
            //     printf("%d", a[i]);
            // }
            // printf("\n\n");
            // for(int i = MAX_SIZE_INT-5; i <= MAX_SIZE_INT+5; i++) {
            //     printf("%d", b[i]);
            // }
            // printf("\n\n");
    
    int len_1 = len - MAX_SIZE_FRAC;
    for(int i = 1; i <= len; i++) {
        if(b[i-1]) {
            _CHENG_Plus(c, a, i);
            // for(int i = MAX_SIZE_INT-5; i <= MAX_SIZE_INT+5; i++) {
            //     printf("%d", c[i]);
            // }
            // printf("\n\n");
        }
        
    } // 逐位乘。

    int n;
    for(int i = len - 1; i >0; i--) {
        if(c[i] > 1) {
            n = c[i] / 2;
            c[i] %= 2;
            c[i-1] += n;
        }
    }


    NUM_List * result = (NUM_List *)malloc(sizeof(NUM_List));
    !result ? exit(0) : (result->next = NULL);
    NUM num;
    for(int i = 0; i < MAX_SIZE_INT; i++) {
        num._int[i] = c[i] + 48;
    }
    for(int i = 0, j = MAX_SIZE_INT; i < MAX_SIZE_FRAC; i++, j++) {
        num._frac[i] = c[j] + 48;
    }
    num.symbol = A.symbol == B.symbol ? 0 : 1;
    result->num = num;

    return result;
}

#endif /* operate.h */