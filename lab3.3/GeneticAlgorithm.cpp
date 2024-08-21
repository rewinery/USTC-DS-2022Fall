#include "GeneticAlgorithm.h"

ea::ea() {
    pop = new Solution[SIZE];
    parent = pop;
    children = pop+PSIZE;
    for (int i = 0; i < PSIZE; ++i) {
        for (int j = 0; j < SLEN; ++j) {
            parent[i].x[j] = rand() % 2;
        }
        evaluate(parent+i, &ea::func1);
    }
}

double ea::decode(bool *x) {
    double res = 0;
    for (int i = 0; i < 4; i++)
        res += x[i] << (3 - i);
    for (int i = 4; i < SLEN; i++)
        if (x[i])
            res += 1.0 / (1 << (i - 3));
    return res;
}

void ea::PrintPop(const string str) {
    cout << str << "/解集信息如下: " << endl;
    for (int i = 0; i < PSIZE; ++i) {
        printf("个体 %3d : y=%10.6lf=f(", i, pop[i].y);
        for (int j = 0; j < SLEN; ++j)
            cout << pop[i].x[j];
        cout << ")" << endl;
    }
}

void ea::Select(int k) {
    double besty;
    int best_cidx;
    Solution tmp[PSIZE];
    for (int i = 0; i < PSIZE; i++) {
        besty = INT_MAX;
        for (int j = 0; j < k; j++) {
            int idx = rand() % SIZE;
            if (pop[idx].y < besty) {
                besty = pop[idx].y;
                best_cidx = idx;
            }
        }
        memcpy(tmp + i, pop + best_cidx, sizeof(Solution));
    }
    memcpy(parent, tmp, sizeof(Solution) * PSIZE);
}

void ea::evaluate(Solution* p, double (ea::*ptrf)(bool* x)) {
    p->y = (this->*ptrf)(p->x);
}

double ea::func1(bool* x) {
    double xo = decode(x);
    return xo * sin(10 * M_PI * xo) + 2.0;
}

void ea::crossover() {
    int k = 0;
    while (k < CSIZE) {
        int pidx1 = rand() % PSIZE;
        int pidx2 = rand() % PSIZE;
        if (pidx1 == pidx2)
            continue;
        int crossPoint = rand() % SLEN;
        for (int i = 0; i < crossPoint; i++) {
            children[k].x[i] = parent[pidx1].x[i];
            children[k+1].x[i] = parent[pidx2].x[i];
        }
        for (int i = crossPoint; i < SLEN; i++) {
            children[k].x[i] = parent[pidx2].x[i];
            children[k+1].x[i] = parent[pidx1].x[i];
        }
        evaluate(children+k++, &ea::func1);
        evaluate(children+k++, &ea::func1);
    }
}

void ea::mutate() {
    for (int i = 0; i < CSIZE; ++i)
        for (int j = 0; j < SLEN; ++j)
            if ((rand() % 100000) < mProb * 100000)
                children[i].x[j] = !children[i].x[j];
}

void ea::Run() {
    PrintPop("初始群体");
    for (int i = 0; i < MAXGEN; i++) {
        crossover();
        mutate();
        Select(3);
        // PrintPop("第" + to_string(i) + "代");
    }
    PrintPop("最终群体");
}