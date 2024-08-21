#ifndef _GENETICALGORITHM_H_
#define _GENETICALGORITHM_H_
#include <iostream>
#include <cstdio>
#include <cstdlib>
// #include <cstring>
#include <string>
#include <cmath>
using namespace std;



class ea {
private:
    #define SLEN 50 // 定义0-1串的长度，可用于表示解的精度
    #define MAXGEN 20 // 定义进化最大代数
    #define mProb (1.0 / SLEN) // 变异概率x
    #define PSIZE 10 // 父群体大小
    #define CSIZE 20 // 子群体大小
    #define SIZE (PSIZE + CSIZE) // 群体大小

    struct Solution {
        bool x[SLEN]; // x:解的自变量，0-1串
        double y;     // y=f(x),要优化问题的目标函数值
    };

    Solution *pop;              // 解集，父代和子代都存储在这里
    Solution *parent;           // 父代解集
    Solution *children;         // 子代解集
    void evaluate(Solution* p, double (ea::*ptrf)(bool* x)); // 使用函数指针，计算一个群体的所有解的目标函数值y
    double decode(bool* x); // 将0-1串x解码为实数*xo ,假定整数4bits，SLEN-4bits为小数部分长度
    double func1(bool* x);
    void crossover(); // 交叉
    void mutate(); // 变异算子
    void PrintPop(const string str); // 打印群体
    void Select(int k); // 选择子代
public:
    ea(); // 构造函数
    void Run(); // 运行遗传算法
};


#endif /* _GENETICALGORITHM_H_ */