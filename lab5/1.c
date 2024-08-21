#include <stdio.h>
#include <stdlib.h>


typedef struct {
    int id;
    int value;
} BElemType;

typedef struct Tnode {
    BElemType data;
    struct Tnode * lchild;
    struct Tnode * rchild;
} BiTNode, * BiTree;

typedef BiTree * QElemType;

typedef struct queue {
    QElemType head;
    QElemType tail; // 目前这一层包含的结点的第一个和最后一个的下一个
} Que;

BiTree NewNode(BElemType);
BiTree InitBiTNode();
BiTree CreateTree(int *, int);
BiTree FindValue(int, BiTree);
void showTree(int, BiTree);
void bfs(Que *); // 广搜来展示树
void Deletex(BiTree);
void RemoveXroot(BiTree, BiTree);
void GetPath(int, BiTree, Que *, Que *);
void GetIdPath(int id, BiTree p, Que * que, Que * q);
void pretra(BiTree T) {
    if(!T) return ;
    printf("(id:%d, value:%d), ", T->data.id, T->data.value);
    if(T->lchild) pretra(T->lchild);
    if(T->rchild) pretra(T->rchild);
}
void midtra(BiTree T) {
    if(!T) return ;
    if(T->lchild) midtra(T->lchild);
    printf("(id:%d, value:%d), ", T->data.id, T->data.value);
    if(T->rchild) midtra(T->rchild);
}
void lastra(BiTree T) {
    if(!T) return ;
    if(T->lchild) lastra(T->lchild);
    if(T->rchild) lastra(T->rchild);
    printf("(id:%d, value:%d), ", T->data.id, T->data.value);
}

int main() {

    // 创建树
    printf("请输入数据，第一个数据 n 表示元素个数，后面 n 个数据为要创建的树的值 value.\n");
    int n;
    scanf("%d", &n);
    int * value = (int *)malloc(sizeof(int) * n);
    for(int i = 0; i < n; i++) {
        scanf("%d", value + i);
    }
    BiTree root = CreateTree(value, n);
    showTree(n+1, root);
    printf("先序遍历: ");
    pretra(root);
    printf("\n中序遍历: ");
    midtra(root);
    printf("\n后序遍历: ");
    lastra(root);
    printf("\n");

    // 删除元素
    int x;
    printf("请输入你要删除的元素：");
    scanf("%d", &x);
    BiTree xroot = FindValue(x, root);
    while(xroot) {
        RemoveXroot(root, xroot);
        Deletex(xroot);
        xroot = FindValue(x, root);
    }
    printf("删除完毕\n");
    showTree(n+1, root);

    
    // 找到路径
    int path_value;
    printf("请输入你想得到的 path 的 value：");
    scanf("%d", &path_value);
    Que que, que1;
    que.head = (QElemType)malloc(sizeof(BiTree) * (n+1));
    que.tail = que.head;
    que1.head = (QElemType)malloc(sizeof(BiTree) * (n+1));
    que1.tail = que1.head;
    *que1.tail++ = root;
    GetPath(path_value, root, &que1, &que);
    QElemType next = que.head + 1;
    while(next < que.tail) {
        if(*next == (*(next-1))->lchild) {
            printf("左");
        } else {
            printf("右");
        }
        next++;
    }
    printf("\n");


    // 找最近共同祖先
    int id1, id2, idx;
    printf("请输入你要找祖先的两个id:");
    scanf("%d%d", &id1, &id2);
    Que que2;
    que2.head = (QElemType)malloc(sizeof(BiTree) * (n+1));
    que2.tail = que2.head;
    que1.tail = que1.head; 
    que.tail = que.head;
    
    *que.tail++ = root;
    GetIdPath(id1, root, &que, &que1);
    
    que.tail = que.head;
    *que.tail++ = root;
    GetIdPath(id2, root, &que, &que2);
    
    QElemType q1 = que1.head+1, q2 = que2.head+1;
    printf("id1: ");
    while(q1 < que1.tail) {
        if(*q1 == (*(q1-1))->lchild) {
            printf("左");
        } else {
            printf("右");
        }
        q1++;
    }

    printf("\nid2: ");
    while(q2 < que2.tail) {
        if(*q2 == (*(q2-1))->lchild) {
            printf("左");
        } else {
            printf("右");
        }
        q2++;
    }

    printf("\n");
    q1 = que1.head; q2 = que2.head;
    while(q1 < que1.tail && q2 < que2.tail) {
        if(*q1 == *q2) {
            idx = (*q1)->data.id;
            q1++;
            q2++;
        } else {
            break;
        }
    }
    printf("离 id1 和 id2 最近结点的id：%d\n", idx);
    
    return 0;
}

void GetPath(int value, BiTree p, Que * que, Que * q) {
    if(!p) return ; // 空树
    if(value == p->data.value) {
        QElemType q1 = que->head;
        while(q1 < que->tail) {
            *q->tail++ = *q1++;
        }
        return ;
    }
    if(p->lchild) {
        *que->tail++ = p->lchild; // 进队
        GetPath(value, p->lchild, que, q);
        que->tail--; // 出队
    }
    if(p->rchild) {
        *que->tail++ = p->rchild; // 进队
        GetPath(value, p->rchild, que, q);
        que->tail--; // 出队
    }
}

void GetIdPath(int id, BiTree p, Que * que, Que * q) {
    if(!p) return ; // 空树
    if(id == p->data.id) {
        QElemType q1 = que->head;
        while(q1 < que->tail) {
            *q->tail++ = *q1++;
        }
        return ;
    }
    if(p->lchild) {
        *que->tail++ = p->lchild; // 进队
        GetIdPath(id, p->lchild, que, q);
        que->tail--; // 出队
    }
    if(p->rchild) {
        *que->tail++ = p->rchild; // 进队
        GetIdPath(id, p->rchild, que, q);
        que->tail--; // 出队
    }
}

void RemoveXroot(BiTree root, BiTree xroot) { // 从树中移除 xroot.
    if(root->lchild) {
        if(root->lchild == xroot) {
            root->lchild = NULL;
            return ;
        }
        RemoveXroot(root->lchild, xroot);
    }
    if(root->rchild) {
        if(root->rchild == xroot) {
            root->rchild = NULL;
            return ;
        }
        RemoveXroot(root->rchild, xroot);
    }
}

void Deletex(BiTree xroot) { // 后续遍历删除结点及子树
    if(xroot->lchild) Deletex(xroot->lchild);
    if(xroot->rchild) Deletex(xroot->rchild);
    free(xroot);
    printf("Successfully delete.\n");
    fflush(stdout);
}

BiTree FindValue(int x, BiTree root) { // 查找 value 值为 x 的元素的位置
    if(!root) return NULL;
    else if(x == root->data.value) {
        printf("找到指定结点, 准备进行删除操作...\n");
        return root;
    }
    BiTree xroot = FindValue(x, root->lchild);
    if(xroot) // 找到对应结点。
        return xroot;
    return FindValue(x, root->rchild);
}

void showTree(int n, BiTree root) {
    if(!root) {
        printf("这是一棵空树.\n");
        return ;
    }
    printf("层序遍历展示树...\n");
    fflush(stdout);
    Que queue;
    queue.head = (BiTree *)malloc(sizeof(BiTree) * n);
    if(!queue.head) {
        printf("内存不足，分配空间失败...\n");
        exit(0);
    }
    QElemType header = queue.head;
    queue.tail = queue.head;
    *queue.tail++ = root;
    while(queue.tail > queue.head) {
        bfs(&queue);
    }
    free(header);
}

void bfs(Que * que) {
    QElemType rear = que->tail;
    while(rear > que->head) {
        if((*que->head)->lchild) {
            *que->tail++ = (*que->head)->lchild;
        } // 将当前结点的左子结点入队
        if((*que->head)->rchild) {
            *que->tail++ = (*que->head)->rchild; 
        } // 将当前结点的右子结点入队
        printf("(id:%d, value:%d)  ", (*que->head)->data.id, (*que->head)->data.value); // 遍历的同时展示数据，检查创建的树是否没问题
        que->head++; // 当前结点出队
    }
    printf("\n");
}

BiTree InitBiTNode() { // 初始化一个结点
    BiTree node = (BiTree)malloc(sizeof(BiTNode));
    if(!node) {
        printf("内存不足，分配空间失败.\n");
        exit(0);
    } else {
        node->lchild = NULL;
        node->rchild = NULL;
    }
    return node;
}

BiTree NewNode(BElemType data) { // 调用 InitBiTNode() 函数，创建新的结点
    BiTree node = InitBiTNode();
    node->data = data;
    return node;
}

BiTree CreateTree(int * value, int len) {
    printf("正在生成二叉排序树...\r");
    BElemType data;
    data.id = 0;
    data.value = value[0];
    BiTree root = NULL, now;
    root = NewNode(data);
    for(int i = 1; i < len; i++) {
        data.id = i;
        data.value = value[i];
        BiTree newnode = NewNode(data); // 创建新结点

        for(BiTree p = root; p; p = (p->data.value >= value[i]) ? p->lchild : p->rchild)
            now = p; // 找到要插入结点的父结点

        (now->data.value >= value[i]) ? (now->lchild = newnode) : (now->rchild = newnode);
    }
    printf("二叉排序树生成完毕...\n");
    return root;
}