#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define MAXVERTEX 100
#define MAXEDGE 20000

//顶点结构体
typedef struct Vertex {
	int id;                 //顶点编号
	int w;                  //顶点权值
	int degree;             //顶点的出度
	long edges[MAXVERTEX];  //边的编号
} Vertex;

// * 边结构体
typedef struct Edge {
	long id;    //边的编号
	int h;      //边头顶点编号
	int t;      //边尾顶点编号
	int w;      //权值
} Edge;

// * 图结构体
typedef struct Graph {
	Vertex v[MAXVERTEX];    //顶点数组
	Edge e[MAXEDGE];        //边数组
	bool dirctional;        //t:有向图，f:无向图
	bool weighted;          //t:带权图，f:等权图
	unsigned nv;            //顶点数
	unsigned long ne;       //边数
} Graph;

unsigned long getEdgeIdx(Graph* g, long id);

// * 生成一个随机图，包括 n 个顶点，每个顶点和其它顶点连边的概率为 p
void randgengraph(int n, float p, bool directional, bool weighted, const char* filename, int seed) {
	srand(seed /*(unsigned)time(NULL)*/);
	char * content = (char *)malloc(5000000);
	if (!content) {
		printf("分配存储空间失败！\n");
		exit(0);
	}
	FILE * fp = fopen(filename, "wb");
	if (!fp) {
		printf("生成图文件 %s 时失败！\n",filename);
		exit(0);
	}
	long offset = 0;//content当前的写入位置
	int ne = 0;//生成边数
	for (int i = 0; i < n; i++)
		offset += sprintf(content + offset, "%3d %5d\n", i, rand() % 100);//写入顶点的编号和顶点随机权值
	int pn = (int)(10000 * p); // p 的 1w 倍
	if (directional) { // 有向图
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				if (i != j && rand() % 10000 < pn ) {
					offset += sprintf(content + offset, "%3d %3d %4d\n", i, j, weighted ? rand() % 100 : 1);
					ne++;
				}
	}
	else {//无向图
		for(int i = 0; i < n; i++)
			for(int j = i+1; j < n; j++) // 只需要从 i+1 开始
				if (rand() % 10000 < pn) {
					offset += sprintf(content + offset, "%3d %3d %4d\n", i, j, weighted ? rand() % 100 : 1);
					ne++;
				}
	}
	char ch[40];
	int sz = sprintf(ch, "%d %7d %5s %5s\n", n, ne, weighted ? "true" : "flase", directional ? "true" : "flase");
	fwrite(&ch, sz, 1, fp);//写入图的顶点数和边数
	fwrite(content, offset, 1, fp);//写入图的顶点和边的信息
	fclose(fp);
	free(content);
    printf("成功随机生成了一个图...\n");
}

//从文件中读入图，在内存中生成图的表示
void initgraph(Graph * g, const char * gfile) {
	FILE * fp = fopen(gfile, "r");
	if (!fp) {
		printf("读取图数据文件出错！\n");
		exit(0);
	}
	char bv[10];
	//读取图的基本信息
	fscanf(fp, "%u%lu%s", &(g->nv), &(g->ne), bv);
	//printf("%d %d %s", g->ne, g->nv, bv);
	g->weighted = strcmp(bv, "true") == 0 ? 1 : 0;
	fscanf(fp, "%s", bv); // 是否是加权图
	g->dirctional = strcmp(bv, "true") == 0 ? 1 : 0; // 是否是有向图
	//读取顶点信息
	for (int i = 0; i < g->nv; i++) {
		fscanf(fp, "%d%d", &(g->v[i].id), &(g->v[i].w));
		g->v[i].degree = 0;
		//printf("顶点%d-权%d\n", g->v[i].id, g->v[i].w);
	}
	//读取边信息
	for (unsigned long i = 0; i < g->ne; i++) {
		fscanf(fp, "%u%u%d", &(g->e[i].h), &(g->e[i].t), &(g->e[i].w));
		//printf("边:%u-%u=>%d\n", g->e[i].h, g->e[i].t, g->e[i].w);
		g->e[i].id = i;
		unsigned h, t;
		h = g->e[i].h;
		t = g->e[i].t;
		g->v[h].edges[g->v[h].degree++] = i; // 有向图只记录邻接表
		if(g->dirctional==0) // 无向图
			g->v[t].edges[g->v[t].degree++] = i;
	}
	fclose(fp);
    printf("图的存储已经创建完毕...\n");
}

//打印邻接表的信息
void printgraph(Graph* g) {
    printf("正在展示图...\n");
    
	printf("\033[46m图的基本信息:顶点数(%u)-边数(%lu)-%s-%s\033[0m\n", g->nv, g->ne, g->weighted ? "加权图" : "等权图", g->dirctional ? "有向图" : "无向图");

	for (int i = 0; i < g->nv; i++) {
		printf("\033[36mID(%u)-度(%d)-权(%d)-边表 t(eid|w):%u\033[0m", g->v[i].id, g->v[i].degree, g->v[i].w, g->v[i].id);
		for (int j = 0; j < g->v[i].degree; j++) {
			unsigned long e = getEdgeIdx(g, g->v[i].edges[j]); // 找到对应边的数组下标
			if (g->v[i].id == g->e[e].h) // 如果是出发顶点
				printf("\033[34m->%u(%lu|%d)\033[0m", g->e[e].t, g->e[e].id, g->e[e].w);
			else // 是终止顶点, 只针对无向图
				printf("\033[34m->%u(%lu|%d)\033[0m", g->e[e].h, g->e[e].id,  g->e[e].w);
		}
		printf("\n");
	}
    printf("\n");
}

//查找给定编号的顶点，返回其顶点数组下标
int getVexIdx(Graph* g, int id) {
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id)
			return i;
    return -1;
}

//查找给定编号的顶点，返回其权值
int getVexW(Graph* g, int id) {
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id)
			return g->v[i].w;
	printf("input wrong vertex id in getVexW()!\n");
	exit(0);
}

//查找给定编号的顶点，返回其度
int getVexDegree(Graph* g, int id) {
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id)
			return g->v[i].degree;
	printf("input wrong vertex id in getVexDegree()!\n");
	exit(0);
}

//查找给定编号的顶点,返回其所有边
long* getEdgesByNode(Graph* g, int id, int* ne) {
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id) {
			*ne = g->v[i].degree;
			return g->v[i].edges;
		}
	printf("input wrong vertex id in getEdgesByNode()!\n");
	exit(0);
}

//查找给定编号的边，返回其边数组下标
unsigned long getEdgeIdx(Graph* g, long id) {
	for (unsigned long i = 0; i < g->ne; i++)
		if (g->e[i].id == id)
			return i;
	printf("input wrong edge id in getEdgeIdx()!\n");
	exit(0);
}

//查找给定编号的边，返回其权值
int getEdgeW(Graph* g, long id) {
	for (long i = 0; i < g->ne; i++)
		if (g->e[i].id == id)
			return g->e[i].w;
	printf("input wrong edge id in getEdgeW()!\n");
	exit(0);
}

//查找给定编号的边，返回其两个顶点
void getVexByEdge(Graph* g, long id, int* h, int* t) {
	for (long i = 0; i < g->ne; i++)
		if (g->e[i].id == id) {
			*h = g->e[i].h;
			*t = g->e[i].t;
            return ;
		}		
	printf("input wrong edge id in getVexByEdge()!\n");
	exit(0);
}

//通过权值查找顶点编号
int locateVex(Graph* g, int w) {
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].w == w)
			return g->v[i].id;
	return -1;
}

//通过权值查找边编号
int locateEdge(Graph* g, int w) {
	for (int i = 0; i < g->ne; i++)
		if (g->e[i].w == w)
			return g->e[i].id;
	return -1;
}

//设置顶点权值
void setVexW(Graph* g, int id, int w) {
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id)
			g->v[i].w = w;
	printf("input wrong vertex id in setVexW()!\n");
	exit(0);
}

//设置边权值
void setEdgeW(Graph* g, int id, int w) {
	for (long i = 0; i < g->ne; i++)
		if (g->e[i].id == id)
			g->e[i].w = w;
	printf("input wrong edge id in setEdgeW()!\n");
	exit(0);
}

//删除边
void deleteEdge(Graph* g, int id) {
    printf("正在删除 id = %d 的边...\n", id);
    int nid = 0;
	for(int i = 0; i < g->ne - nid; i++) {
        if(g->e[i].id == id) {
            int v1 = getVexIdx(g, g->e[i].h); // 要删除的边的起始点
            nid++;
            if(v1 != -1) {
                g->v[v1].degree--;
                int nide = 0;
                for(int j = 0; j < g->v[v1].degree; j++) {
                    if(g->v[v1].edges[j] == id) {
                        nide++;
                    }
                    g->v[v1].edges[j] = g->v[v1].edges[j+nide];
                }
            }
            if(!g->dirctional) { // 无向图
                int v2 = getVexIdx(g, g->e[i].t); // 要删除的边的终止点        
                if(v2 != -1) {
                    g->v[v2].degree--;
                    int nide = 0;
                    for(int j = 0; j < g->v[v2].degree; j++) {
                        if(g->v[v2].edges[j] == id) {
                            nide++;
                        }
                        g->v[v2].edges[j] = g->v[v2].edges[j+nide];
                    }
                }
            }
        }
        if(nid)
            g->e[i] = g->e[i+nid]; // 找到编号为 id 的边
    }
    g->ne -= nid;
    // 如果 nid = 0，即没有编号为 id 的边，将不发生改变
    if(!nid)
        printf("没有找到要删除的边!\n");
    else {
        printf("id = %d 的边已经成功删除...\n", id);
    }
}


//删除顶点
void deleteVex(Graph* g, int id) {
    printf("正在删除 id 为 %d 的顶点...\n", id);
	int nid = 0;
    for(int i = 0; i < g->nv - nid; i++) {
        if(g->v[i].id == id) { // 找到对应顶点
            printf("正在删除与 id = 5 的顶点关联的边...\n");
            int degree = g->v[i].degree;
            for(int j = 0; j < degree; j++) {
                deleteEdge(g, g->v[i].edges[0]);
            }
            nid++;
        }
        
        if(nid)
            g->v[i] = g->v[i+nid];
    }
    g->nv -= nid;
    if(!nid)
        printf("没有找到要删除的顶点!\n");
    else {
        for(int i = 0; i < g->nv; i++) {
             // 其他顶点，如果有与 id = 5 的顶点相邻的边，也需要删除
            for(int j = 0; j < g->v[i].degree; ) {
                int E_idx = getEdgeIdx(g, g->v[i].edges[j]); // 边的数组的下标
                int h, t;
                getVexByEdge(g, g->v[i].edges[j], &h, &t);
                if(t == id) {
                    deleteEdge(g, g->v[i].edges[j]);
                }
                else j++;
            }
        }
        printf("id = %d 的顶点已成功删除...\n", id);
    }
}


//增加顶点
void addVex(Graph* g, int w) {
    printf("要插入顶点的权重为 %d, 开始插入顶点...\n", w);
	g->v[g->nv].w = w; // 新建权重
    g->v[g->nv].id = g->v[g->nv-1].id+1; // 这个 id 一定没有出现过
    g->v[g->nv].degree = 0;
    g->nv++;
    printf("成功加入一个权重为 %d 的顶点:\n\n", w);
}             

//增加边
void addEdge(Graph* g, int w, int h, int t) {
    printf("要插入的边的权重为 %d, 开始插入边...\n", w);
	g->e[g->ne].w = w;
    g->e[g->ne].h = h;
    g->e[g->ne].t = t;
    g->e[g->ne].id = g->e[g->ne-1].id+1; // 新边的 id
    g->ne++;
    // 相应的边的权值发生改变
    int idh = getVexIdx(g, h); 
    int idt = getVexIdx(g, t); 
    g->v[idh].edges[g->v[idh].degree++] = g->e[g->ne-1].id;
    if(!g->dirctional)
        g->v[idt].edges[g->v[idt].degree++] = g->e[g->ne-1].id;
    printf("成功插入权重为 %d 的边, 他的起点编号为: %d, 终点编号为: %d\n\n", w, h, t);
}

bool book[MAXVERTEX] = {0};
#define _Infty 1919810
unsigned long min = 1919114514;

void dfs(Graph * g, int start_id, int desti_id, unsigned long * dist) {
    if(start_id == desti_id) {
        if(min > *dist){
            min = *dist;
        }
        return ;
    }
    int idx_start = getVexIdx(g, start_id); // 起始点的下标
    int idx_desti = getVexIdx(g, desti_id); // 终点的下标
    if(g->v[idx_start].degree == 0) return ; // 走到头了
    int idx_next_v; // 下一步的点的下标
    unsigned long idx_next_edge; // 下一步的边的下标
    int h;
    for(int i = 0; i < g->v[idx_start].degree; i++) {
        getVexByEdge(g, g->v[idx_start].edges[i], &h, &idx_next_v); // 收集到可以走的下一步的所有点的 id
        if(!book[idx_next_v]) {
            idx_next_edge = getEdgeIdx(g, g->v[idx_start].edges[i]); // 将要走的边的下标
            book[idx_next_v] = 1; // 标记这个点走过
            *dist += g->e[idx_next_edge].w;
            dfs(g, idx_next_v, desti_id, dist);
            *dist -= g->e[idx_next_edge].w;
            book[idx_next_v] = 0; // 取消这个点的标记
        }
    }
}


void ComplexOperation(Graph * g) {
    printf("To find the the Minimum distance between A and B\n");
    int start_id = 1, end_id = 3;
    book[start_id] = 1;
    unsigned long dist = 0;
    dfs(g, start_id, end_id, &dist);
    printf("从 id = %d 的点到 id = %d 的点的最小加权路径为 %lu\n", start_id, end_id, min);
}

void Print_in_markdown(Graph * g) {
    printf("正在创建 markdown 文档...\n");
    FILE * fp = fopen("g.md", "w");
    if(!fp) {
        printf("无法打开文件\n");
        exit(0);
    }
    fprintf(fp, "mermaid 图：\n```mermaid\nflowchart LR\n");
	for (int i = 0; i < g->nv; i++) {
        if(!g->v[i].degree) fprintf(fp, "%d\n", g->v[i].id);
		for (int j = 0; j < g->v[i].degree; j++) {
			unsigned long e = getEdgeIdx(g, g->v[i].edges[j]); // 找到对应边的数组下标
			if (g->v[i].id == g->e[e].h) // 如果是出发顶点
				fprintf(fp, "%d -- %d ---> %d \n", g->v[i].id, g->e[e].w, g->e[e].t);
			else // 是终止顶点, 只针对无向图
				fprintf(fp, "%d -- %d --- %d \n", g->v[i].id, g->e[e].w, g->e[e].h);
		}
	}
    fprintf(fp, "```\n");
    fclose(fp);
    printf("markdown 文档创建完毕\n");
}

//保存图数据为html文件，供浏览器查看
void writeGraph(Graph *g,FILE *fp){
	for (int i = 0; i < g->nv; ++i) // 为了显示独立结点
		fprintf(fp, "{source: '%d', target: '%d', 'rela': '', type: 'resolved'},\n", g->v[i].id,g->v[i].id);
	if (g->weighted) 
		for (int i = 0; i < g->ne; ++i)	
			fprintf(fp, "{source: '%d', target: '%d', 'rela': '%d', type: 'resolved'},\n",g->e[i].h,g->e[i].t, g->e[i].w);
	else																	
		for (int i = 0; i < g->ne; ++i)
			fprintf(fp,"{source: '%d', target: '%d', 'rela': '', type: 'resolved'},\n",g->e[i].h,g->e[i].t);
}

void saveGraph(Graph *g,const char filename[]){
	FILE * fp = fopen(filename,"wb");
	if (!fp) {
		printf("打开写入文件出错！\n");
		exit(0);
	}
	//读取头部文件，二进制方式写入filename文件
	FILE * fh = fopen("head.txt","rb");
	if(!fh) {
		printf("文件打开失败\n");
		fflush(stdout);
		exit(0);
	}
	fseek(fh,0,SEEK_END); 
	fflush(stdout);
	unsigned long fsize = ftell(fh);
	rewind(fh);
	unsigned char *dataArray;
	dataArray = (unsigned char *)malloc(sizeof(unsigned char)*fsize);
	if (!dataArray) {
		printf("文件太大，内存不够，读入错误! %lu\n",fsize);
		exit(0);
	}
	fread(dataArray,sizeof(unsigned char),fsize,fh); //读取文件
	fwrite(dataArray, sizeof(unsigned char),fsize,fp);//写入文件
	free(dataArray);
	fclose(fh);

	//将图的信息写入filename文件
	fprintf(fp,"\n\nvar links = \n[\n");
	writeGraph(g,fp);
	fprintf(fp,"\n];");

	//读取尾部文件，二进制方式写入filename文件
	if (g->dirctional)//有向图
		fh = fopen("tail.txt","rb");
	else //无向图
		fh = fopen("tail1.txt","rb");
	fseek(fh,0,SEEK_END);
	fsize = ftell(fh);
	rewind(fh);
	dataArray = (unsigned char *)malloc(sizeof(unsigned char)*fsize);
	if (!dataArray) {
		printf("文件太大，内存不够，读入错误! %lu\n",fsize);
		exit(0);
	}
	fread(dataArray,sizeof(unsigned char),fsize,fh); //读取文件
	fwrite(dataArray, sizeof(unsigned char),fsize,fp);//写入文件
	free(dataArray);
	fclose(fh);
	fclose(fp);
	printf("写入完毕...\n");
}

int main() {
	Graph * g = (Graph *)malloc(sizeof(Graph));
	if (!g) {
		printf("error\n");
		exit(0);
    }
	randgengraph(10, 0.5, 1, 1, "graph.txt", 5);
	initgraph(g, "graph.txt");
	printgraph(g);
	saveGraph(g, "Graph1.html");
	
	int h;
	int t;
	addVex(g, 30);
    printgraph(g);
	// saveGraph(g, "Graph2.html");
    addEdge(g, 50, 1, 3);
    printgraph(g);
	// saveGraph(g, "Graph3.html");
	getVexByEdge(g, 10, &h, &t);
	printf("id 为 10 的边的头尾节点的id: %d %d\n", h, t);
	deleteEdge(g, 10);
    printgraph(g);
	// saveGraph(g, "Graph4.html");
	deleteVex(g, 5);
    printgraph(g);
	// saveGraph(g, "Graph5.html");
    ComplexOperation(g);
    Print_in_markdown(g);
    return 0;
}