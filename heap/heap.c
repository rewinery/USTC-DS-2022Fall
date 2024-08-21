/**************************
 * >>> 用堆实现优先队列 <<< *
 **************************/
#include <stdio.h>
#include <stdlib.h>

#define ElemType int
#define MAX_CAPACITY 1024
#define ERROR_PROMPT -114514

#define CHILD1(pidx) (1+(2*(pidx)))     /**左孩子 */
#define CHILD2(pidx) (1+(2*(pidx)+1))   /**右孩子 */


struct heap {
    int capacity;   // 容量
    int size;       // 目前已有的元素数量
    ElemType *val;  // 
};

/**
 * @brief 初始化一个堆
 */
void InitHeap(struct heap *h) {
    h->capacity = MAX_CAPACITY; 
    h->size = 0;                
    h->val = (ElemType *)malloc(sizeof(ElemType)*MAX_CAPACITY); // 为 value 分配存储空间
}

/**
 * @brief 获取当前结点父结点的下标
 */
void GetParent(int idx, int *pidx) {
    if(idx == 0) *pidx = ERROR_PROMPT; // 错误，root 没有父结点
    *pidx = (idx-1)/2;
}

/**
 * @brief 交换两个 int 类型的变量
 */
void swap_int(int *elem1, int *elem2) {
    int tmp = *elem1;
    *elem1 = *elem2;
    *elem2 = tmp;
}

/**
 * @brief 添加一个结点进入优先队列(小根堆)
 */
void AddElem(struct heap *h, ElemType elem) {
    if(h->size == h->capacity) {
        h->capacity += MAX_CAPACITY;
        h->val = (ElemType *)realloc(h->val, sizeof(ElemType)*h->capacity);
        if(h->val == NULL) {
            printf("堆已满，无法增大内存，添加失败...\n");
            return ;
        }
    }
    h->val[h->size++] = elem; // 先加入队尾
    int idx = h->size-1;
    while(idx > 0) {
        int pidx;  // parent index
        GetParent(idx, &pidx);
        if(h->val[pidx] > h->val[idx]) { 
            // 如果比父结点大，那么与父结点交换
            swap_int(h->val+pidx, h->val+idx);
            idx = pidx;
        }
        else return ; // 否则结束
    }
}

/**
 * @brief 头结点出队
 */
void deheap(struct heap *h, int *elem) {
    if(h->size == 0) {
        printf("空堆，获取失败...\n");
        *elem = ERROR_PROMPT;
        return ;
    }
    *elem = h->val[0]; // return
    h->size--; // 更新size
    if(h->size == 0) return ;
    int idx = 0;
    while(1) { // 将空位一步一步移到叶子
        int sidx1 = CHILD1(idx); // son index 1
        if(sidx1 >= h->size) break; // 出口
        int sidx2 = CHILD2(idx); // son index 2
        int sidx;
        if(sidx2 >= h->size) sidx = sidx1;
        else sidx = h->val[sidx1]<=h->val[sidx2] ? sidx1 : sidx2; // 选择两个子结点中较小的那个成为父结点
        swap_int(h->val+idx, h->val+sidx);
        idx = sidx; // 更新空位的位置
    }

    // 将最后一个元素填补到空位
    h->val[idx] = h->val[h->size];
    while(idx > 0) {
        int pidx;
        GetParent(idx, &pidx); // 赋值 pidx
        if(h->val[pidx] > h->val[idx]) {
            swap_int(h->val+pidx, h->val+idx);
            idx = pidx;
        }
        else return ;
    }
}

int main() {
    struct heap pqueue;
    InitHeap(&pqueue);
    while(1) {
        printf("输入 -1 以退出程序.\n");
        printf("输入 0  以添加元素.\n");
        printf("输入 1  以减少元素.\n");
        int c;
        scanf("%d", &c);
        if(c == -1) break;
        if(c == 0) {
            printf("请输入你要添加的元素：");
            ElemType elem;
            scanf("%d", &elem);
            AddElem(&pqueue, elem);
        }
        else {
            ElemType elem;
            deheap(&pqueue, &elem);
            if(elem != ERROR_PROMPT) {
                printf("出队的元素是：%d\n", elem);
            }
        }
    }
    return 0;
}