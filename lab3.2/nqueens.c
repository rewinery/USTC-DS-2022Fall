//n-皇后问题的随机搜索算法，找到解即可
//用一维数组来存放每一列皇后的行号

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

//采用堆分配存储的方式实现
int * solution; //每列一个皇后，solution[col]=row,表示第col列的皇后在row行
int len, _len, len_1, value = 0;            //皇后数量，len*len的棋盘
unsigned * a;
unsigned * b;

//函数声明
void initSolution();
void restart();
void swap(int , int);
int traverseNeighbors();
int evaluate();
void print();

int main(){
    clock_t t0 = clock(); 
    long seed = 820;  //随机数种子，如果这个数不改变，那么每次算法运行的结果都是一样的，即使用rand()函数
    // srand(seed);  //这一句注释掉，等于每次的随机数种子都不一样

    printf("请输入皇后个数：");
    scanf("%d", &len);      // 键盘读入棋盘的大小
    _len = 2 * len - 1;
    len_1 = len - 1;
    a = (unsigned *)malloc(sizeof(unsigned) * _len);
    b = (unsigned *)malloc(sizeof(unsigned) * _len);
    memset(a, 0, _len * sizeof(unsigned));
    memset(b, 0, _len * sizeof(unsigned));
    // len = 10; // 棋盘大小
    solution = (int *)malloc(sizeof(int)*len);
    if (!solution)  return 0; //分配空间失败
    
    initSolution();   // 初始化一个棋盘布局
    // printf("value = %d", value);
    int flag;
    while ((flag = traverseNeighbors()) != 0) {
        if (flag < 0) {
            restart();
            memset(a, 0, _len * sizeof(unsigned));
            memset(b, 0, _len * sizeof(unsigned));
            value = evaluate();
        }
    }
    printf("\n冲突数为：%d\n得到的解为: ", value);
    for(int i = 0; i < len; ++i)
    printf("%d, ",solution[i]);
    getchar();

    if(len < 40) print();

    free(solution);
    clock_t t1 = clock();
    printf("\ntime = %fs\n", (1.0 * (t1 - t0)) / CLOCKS_PER_SEC);
    return 0;
}

void print() {
    printf("\n");
    for(int i = 0; i < len; i++) {
        for(int j = 0; j < len; j++) {
            if(j == solution[i]) printf("Q ");
            else printf("* ");
        }
        printf("\n");
    }
}

/***  将棋盘的第i列和第j列交换  ***/
void swap(int i, int j) {
    int swap = solution[i];
    solution[i] = solution[j];
    solution[j] = swap;
}


/***  初始化一个棋盘布局,将邻居数组准备好  ***/
void initSolution() {
 // 随机给初始布局或给定某个初始布局
    for(int i = 0; i < len; ++i)
        solution[i] = i;
    for(int i = 0; i < 2*len; ++i) // 对棋盘进行2*len次随机交换
        swap(rand()%len, rand()%len);
    value = evaluate(); // 给 value 赋初值。
}

int GetSum(unsigned n) {
    return (n * (n - 1)) / 2;
}

/***  计算棋盘的评价函数（棋盘的价值: value=可相互攻击到的皇后对数）
 ***  当皇后相互攻击不到时，目标函数的值value==0
 ***  solution[i]==solution[j], (solution[i]-solution[j])/(i-j)==1或-1时（C语言实现会有bug -- int 型除法自动转 int），两个皇后彼此能攻击到，value++
 ***  棋盘编码方式能保证不会出现 solution[i]==solution[j]，故该条件可以不用判断
 ***  思考：该函数的时间性能是否能提高？
 ***/
int evaluate() {
    int __value = 0;
    // 用斜对角线数组
    for(int i = 0; i < len; i++) {
        a[i+solution[i]]++;
        b[i-solution[i]+len_1]++;
    }
    for(int i = 0; i < _len; i++) {
        if(a[i] > 1) {
            __value += GetSum(a[i]);
        }
        if(b[i] > 1) {
            __value += GetSum(b[i]);
        }
    }
    return __value;
}

int SwapForCorrection(int i, int j) { // 修正 value, *a 和 *b
    int __value = value;
    __value -= --a[i+solution[i]];
    __value -= --b[i-solution[i]+len_1];
    __value -= --a[j+solution[j]];
    __value -= --b[j-solution[j]+len_1];
    
    int swap = solution[i];
    solution[i] = solution[j];
    solution[j] = swap; // (i, solution[i]) -> (i, solutoin[j]), (j, solution[j]) -> (j, solution[i]).
    
    __value += a[i+solution[i]]++;
    __value += b[i-solution[i]+len_1]++;
    __value += a[j+solution[j]]++;
    __value += b[j-solution[j]+len_1]++;
    return __value;
}

/***  检查当前解（棋盘）的邻居棋盘（交换solution[i]和solution[j],i和j随机选择，共有n(n-1)/2种不同的可能）
 ***  保留冲突最小的棋盘，将它视为新的解（best found策略）
 ***  若返回-1,表示没有找到更好的邻居，需要重启 restart() ,返回0表示找到所求的解，
 ***  返回大于0的值表示需要更新当前解solution后要继续遍历新解的邻居
 ***/
int traverseNeighbors() {
    _Bool flag = 0;
    //遍历邻居，评估每个邻居冲突皇后对的个数，找到最小冲突对数的邻居
    int __value = value;
    for(int i = 0; i < len; i++) {
        for(int j = i + 1; j < len; j++) {
            __value = SwapForCorrection(i, j);
            if(__value < value) {
                if(!(value = __value)) return 0;
                flag = 1;
            }
            else __value = SwapForCorrection(i, j);
        }
    }
    if(!flag) {
        printf("找不到解，正在重新生成初始解和运行搜索算法中...\n");
        return -1;
    }
    else {
        return __value;
    }
}

/***  当遍历完所有邻居，也没有找到更好的邻居
 ***  那么随机重置solution，然后再用爬山法搜索
 ***  随机重启：邻居是邻域内的“微小”的扰动，增加扰动范围，比如随机交换times=20次
 ***/
void restart() {
    int times = 20;  // 随机交换解的次数，扰动大小,设置times>1即可
    for(int i = 0; i < times; ++i)
        swap(rand()%len, rand()%len);
    puts("restart()被调用！");
}