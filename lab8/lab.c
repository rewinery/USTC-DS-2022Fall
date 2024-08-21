#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define MAXVERTEX 100
#define MAXEDGE 20000

// * ����ṹ��
typedef struct Vertex {
	int id;                 //������
	int w;                  //����Ȩֵ
	int degree;             //����ĳ���
	long edges[MAXVERTEX];  //�ߵı��
} Vertex;

// * �߽ṹ��
typedef struct Edge {
	long id;    //�ߵı��
	int h;      //��ͷ������
	int t;      //��β������
	int w;      //Ȩֵ
} Edge;

// * ͼ�ṹ��
typedef struct Graph {
	Vertex v[MAXVERTEX];    //��������
	Edge e[MAXEDGE];        //������
	bool dirctional;        //t:����ͼ��f:����ͼ
	bool weighted;          //t:��Ȩͼ��f:��Ȩͼ
	unsigned nv;            //������
	unsigned long ne;       //����
} Graph;

unsigned long getEdgeIdx(Graph* g, long id);

// * ����һ�����ͼ������ n �����㣬ÿ������������������ߵĸ���Ϊ p
void randgengraph(int n, float p, bool directional, bool weighted, const char* filename, int seed) {
	srand(seed /*(unsigned)time(NULL)*/);
	char * content = (char *)malloc(5000000);
	if (!content) {
		printf("����洢�ռ�ʧ�ܣ�\n");
		exit(0);
	}
	FILE * fp = fopen(filename, "wb");
	if (!fp) {
		printf("����ͼ�ļ� %s ʱʧ�ܣ�\n",filename);
		exit(0);
	}
	long offset = 0;//content��ǰ��д��λ��
	int ne = 0;//���ɱ���
	for (int i = 0; i < n; i++)
		offset += sprintf(content + offset, "%3d %5d\n", i, rand() % 100);//д�붥��ı�źͶ������Ȩֵ
	int pn = (int)(10000 * p); // p �� 1w ��
	if (directional) { // ����ͼ
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				if (i != j && rand() % 10000 < pn ) {
					offset += sprintf(content + offset, "%3d %3d %4d\n", i, j, weighted ? rand() % 100 : 1);
					ne++;
				}
	}
	else {//����ͼ
		for(int i = 0; i < n; i++)
			for(int j = i+1; j < n; j++) // ֻ��Ҫ�� i+1 ��ʼ
				if (rand() % 10000 < pn) {
					offset += sprintf(content + offset, "%3d %3d %4d\n", i, j, weighted ? rand() % 100 : 1);
					ne++;
				}
	}
	char ch[40];
	int sz = sprintf(ch, "%d %7d %5s %5s\n", n, ne, weighted ? "true" : "flase", directional ? "true" : "flase");
	fwrite(&ch, sz, 1, fp);//д��ͼ�Ķ������ͱ���
	fwrite(content, offset, 1, fp);//д��ͼ�Ķ���ͱߵ���Ϣ
	fclose(fp);
	free(content);
    printf("�ɹ����������һ��ͼ...\n");
}

//���ļ��ж���ͼ�����ڴ�������ͼ�ı�ʾ
void initgraph(Graph * g, const char * gfile) {
	FILE * fp = fopen(gfile, "r");
	if (!fp) {
		printf("��ȡͼ�����ļ�����\n");
		exit(0);
	}
	char bv[10];
	//��ȡͼ�Ļ�����Ϣ
	fscanf(fp, "%u%lu%s", &(g->nv), &(g->ne), bv);
	//printf("%d %d %s", g->ne, g->nv, bv);
	g->weighted = strcmp(bv, "true") == 0 ? true : false;
	fscanf(fp, "%s", bv); // �Ƿ��Ǽ�Ȩͼ
	g->dirctional = strcmp(bv, "true") == 0 ? true : false; // �Ƿ�������ͼ
	//��ȡ������Ϣ
	for (int i = 0; i < g->nv; i++) {
		fscanf(fp, "%d%d", &(g->v[i].id), &(g->v[i].w));
		g->v[i].degree = 0;
		//printf("����%d-Ȩ%d\n", g->v[i].id, g->v[i].w);
	}
	//��ȡ����Ϣ
	for (unsigned long i = 0; i < g->ne; i++) {
		fscanf(fp, "%u%u%d", &(g->e[i].h), &(g->e[i].t), &(g->e[i].w));
		//printf("��:%u-%u=>%d\n", g->e[i].h, g->e[i].t, g->e[i].w);
		g->e[i].id = i;
		unsigned h, t;
		h = g->e[i].h;
		t = g->e[i].t;
		g->v[h].edges[g->v[h].degree++] = i; // ����ͼֻ��¼�ڽӱ�
		if(g->dirctional==false) // ����ͼ
			g->v[t].edges[g->v[t].degree++] = i;
	}
	fclose(fp);
    printf("ͼ�Ĵ洢�Ѿ��������...\n");
}

//��ӡ�ڽӱ����Ϣ
void printgraph(Graph* g) {
    printf("����չʾͼ...\n");
    
	printf("\033[46mͼ�Ļ�����Ϣ:������(%u)-����(%lu)-%s-%s\033[0m\n", g->nv, g->ne, g->weighted ? "��Ȩͼ" : "��Ȩͼ", g->dirctional ? "����ͼ" : "����ͼ");

	for (int i = 0; i < g->nv; i++) {
		printf("\033[36mID(%u)-��(%d)-Ȩ(%d)-�߱� t(eid|w):%u\033[0m", g->v[i].id, g->v[i].degree, g->v[i].w, g->v[i].id);
		for (int j = 0; j < g->v[i].degree; j++) {
			unsigned long e = getEdgeIdx(g, g->v[i].edges[j]); // �ҵ���Ӧ�ߵ������±�
			if (g->v[i].id == g->e[e].h) // ����ǳ�������
				printf("\033[34m->%u(%lu|%d)\033[0m", g->e[e].t, g->e[e].id, g->e[e].w);
			else // ����ֹ����, ֻ�������ͼ
				printf("\033[34m->%u(%lu|%d)\033[0m", g->e[e].h, g->e[e].id,  g->e[e].w);
		}
		printf("\n");
	}
    printf("\n");
}

//���Ҹ�����ŵĶ��㣬�����䶥�������±�
int getVexIdx(Graph* g, int id) {
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id)
			return i;
    return -1;
}

//���Ҹ�����ŵĶ��㣬������Ȩֵ
int getVexW(Graph* g, int id) {
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id)
			return g->v[i].w;
	printf("input wrong vertex id in getVexW()!\n");
	exit(0);
}

//���Ҹ�����ŵĶ��㣬�������
int getVexDegree(Graph* g, int id) {
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id)
			return g->v[i].degree;
	printf("input wrong vertex id in getVexDegree()!\n");
	exit(0);
}

//���Ҹ�����ŵĶ���,���������б�
long* getEdgesByNode(Graph* g, int id, int* ne) {
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id) {
			*ne = g->v[i].degree;
			return g->v[i].edges;
		}
	printf("input wrong vertex id in getEdgesByNode()!\n");
	exit(0);
}

//���Ҹ�����ŵıߣ�������������±�
unsigned long getEdgeIdx(Graph* g, long id) {
	for (unsigned long i = 0; i < g->ne; i++)
		if (g->e[i].id == id)
			return i;
	printf("input wrong edge id in getEdgeIdx()!\n");
	exit(0);
}

//���Ҹ�����ŵıߣ�������Ȩֵ
int getEdgeW(Graph* g, long id) {
	for (long i = 0; i < g->ne; i++)
		if (g->e[i].id == id)
			return g->e[i].w;
	printf("input wrong edge id in getEdgeW()!\n");
	exit(0);
}

//���Ҹ�����ŵıߣ���������������
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

//ͨ��Ȩֵ���Ҷ�����
int locateVex(Graph* g, int w) {
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].w == w)
			return g->v[i].id;
	return -1;
}

//ͨ��Ȩֵ���ұ߱��
int locateEdge(Graph* g, int w) {
	for (int i = 0; i < g->ne; i++)
		if (g->e[i].w == w)
			return g->e[i].id;
	return -1;
}

//���ö���Ȩֵ
void setVexW(Graph* g, int id, int w) {
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id)
			g->v[i].w = w;
	printf("input wrong vertex id in setVexW()!\n");
	exit(0);
}

//���ñ�Ȩֵ
void setEdgeW(Graph* g, int id, int w) {
	for (long i = 0; i < g->ne; i++)
		if (g->e[i].id == id)
			g->e[i].w = w;
	printf("input wrong edge id in setEdgeW()!\n");
	exit(0);
}

//ɾ����
void deleteEdge(Graph* g, int id) {
    printf("����ɾ�� id = %d �ı�...\n", id);
    int nid = 0;
	for(int i = 0; i < g->ne - nid; i++) {
        if(g->e[i].id == id) {
            int v1 = getVexIdx(g, g->e[i].h); // Ҫɾ���ıߵ���ʼ��
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
            if(!g->dirctional) { // ����ͼ
                int v2 = getVexIdx(g, g->e[i].t); // Ҫɾ���ıߵ���ֹ��        
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
            g->e[i] = g->e[i+nid]; // �ҵ����Ϊ id �ı�
    }
    g->ne -= nid;
    // ��� nid = 0����û�б��Ϊ id �ıߣ����������ı�
    if(!nid)
        printf("û���ҵ�Ҫɾ���ı�!\n");
    else {
        printf("id = %d �ı��Ѿ��ɹ�ɾ��...\n", id);
    }
}


//ɾ������
void deleteVex(Graph* g, int id) {
    printf("����ɾ�� id Ϊ %d �Ķ���...\n", id);
	int nid = 0;
    for(int i = 0; i < g->nv - nid; i++) {
        if(g->v[i].id == id) { // �ҵ���Ӧ����
            printf("����ɾ���� id = 5 �Ķ�������ı�...\n");
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
        printf("û���ҵ�Ҫɾ���Ķ���!\n");
    else {
        for(int i = 0; i < g->nv; i++) {
             // �������㣬������� id = 5 �Ķ������ڵıߣ�Ҳ��Ҫɾ��
            for(int j = 0; j < g->v[i].degree; ) {
                int E_idx = getEdgeIdx(g, g->v[i].edges[j]); // �ߵ�������±�
                int h, t;
                getVexByEdge(g, g->v[i].edges[j], &h, &t);
                if(t == id) {
                    deleteEdge(g, g->v[i].edges[j]);
                }
                else j++;
            }
        }
        printf("id = %d �Ķ����ѳɹ�ɾ��...\n", id);
    }
}


//���Ӷ���
void addVex(Graph* g, int w) {
    printf("Ҫ���붥���Ȩ��Ϊ %d, ��ʼ���붥��...\n", w);
	g->v[g->nv].w = w; // �½�Ȩ��
    g->v[g->nv].id = g->v[g->nv-1].id+1; // ��� id һ��û�г��ֹ�
    g->v[g->nv].degree = 0;
    g->nv++;
    printf("�ɹ�����һ��Ȩ��Ϊ %d �Ķ���:\n\n", w);
}             

//���ӱ�
void addEdge(Graph* g, int w, int h, int t) {
    printf("Ҫ����ıߵ�Ȩ��Ϊ %d, ��ʼ�����...\n", w);
	g->e[g->ne].w = w;
    g->e[g->ne].h = h;
    g->e[g->ne].t = t;
    g->e[g->ne].id = g->e[g->ne-1].id+1; // �±ߵ� id
    g->ne++;
    // ��Ӧ�ıߵ�Ȩֵ�����ı�
    int idh = getVexIdx(g, h); 
    int idt = getVexIdx(g, t); 
    g->v[idh].edges[g->v[idh].degree++] = g->e[g->ne-1].id;
    if(!g->dirctional)
        g->v[idt].edges[g->v[idt].degree++] = g->e[g->ne-1].id;
    printf("�ɹ�����Ȩ��Ϊ %d �ı�, ���������Ϊ: %d, �յ���Ϊ: %d\n\n", w, h, t);
}

bool book[MAXVERTEX] = {0};
#define _Infty 1919810
unsigned long min = 1919114514;

void dfs(Graph * g, int start_id, int desti_id, unsigned long * dist) {
	printf("%d", start_id); // To print the path of dfs.
    if(start_id == desti_id) {
        if(min > *dist){
            min = *dist;
        }
        return ;
    }
    int idx_start = getVexIdx(g, start_id); // ��ʼ����±�
    int idx_desti = getVexIdx(g, desti_id); // �յ���±�
    if(g->v[idx_start].degree == 0) return ; // �ߵ�ͷ��
    int id_next_v; // ��һ���ĵ�� id
    unsigned long idx_next_edge; // ��һ���ıߵ��±�
    int h;
    for(int i = 0; i < g->v[idx_start].degree; i++) {
        getVexByEdge(g, g->v[idx_start].edges[i], &h, &id_next_v); // �ռ��������ߵ���һ�������е�� id
		if(id_next_v == start_id) {
			id_next_v = h;
		} // �������ͼ
        if(!book[id_next_v]) {
            idx_next_edge = getEdgeIdx(g, g->v[idx_start].edges[i]); // ��Ҫ�ߵıߵ��±�
            book[id_next_v] = 1; // ���������߹�
            *dist += g->e[idx_next_edge].w;
            printf("->"); // To print the path of the dfs
            dfs(g, id_next_v, desti_id, dist);
            *dist -= g->e[idx_next_edge].w;
            book[id_next_v] = 0; // ȡ�������ı��
        }
    }
}


int bfs(Graph * g, int start_id, int desti_id) {
	int pace = 0;
	for(int i = 0; i < MAXVERTEX; i++) {
		book[i] = 0;
	}
	book[start_id] = 1; // deal with 'book' array.
	printf("Begin bfs operation...\n");
	printf("The path of bfs:\n");
	int * fringe = (int *) malloc(sizeof(int) * MAXVERTEX); // �洢 id �Ķ���
	int * fringe_tail = fringe;
	*fringe_tail++ = start_id; // To push in the queue
	int idx = 0, h, idx_next_v;
    int idx_start = getVexIdx(g, start_id); // ��ʼ����±�
    int idx_desti = getVexIdx(g, desti_id); // �յ���±�
	int i, * tail;
	while(fringe < fringe_tail) {
		tail = fringe_tail;
		while(fringe < tail) {
			printf("->%d", *fringe);
			if(*fringe == desti_id) {
				return pace;
			}
			for(i = 0; i < g->v->degree; i++) {
				getVexByEdge(g, g->v[getVexIdx(g, *fringe)].edges[i], &h, &idx_next_v); // �ռ��������ߵ���һ�������е�� id
				if(!book[idx_next_v]) {
					book[idx_next_v] = 1;
					*fringe_tail++ = g->v[idx_next_v].id; // ���
				}
			}
			fringe++; // ����
		}
		pace++;
	}
	return pace;
}

void Dij(Graph * g, int start_id) {
	// ��ʼ�� book ����
	for(int i = 0; i < MAXVERTEX; i++) {
		book[i] = 0;
	}
	book[start_id] = 1;
	// ��ʼ�����뺯��
	int map[MAXVERTEX][MAXVERTEX];
	for(int i = 0; i < g->nv; i++) {
		for(int j = 0; j < g->nv; j++) {
			map[i][j] = _Infty;
		}
	}
	for(int i = 0; i < g->ne; i++) {
		map[g->e[i].h][g->e[i].t] = g->e[i].w;
		if(!g->dirctional) {
			map[g->e[i].t][g->e[i].h] = g->e[i].w;
		}
	}
	for(int i = 0; i < g->nv; i++) {
		map[i][i] = 0;
	}
	for(int i = 0; i < g->nv; i++) {
		for(int j = 0; j < g->nv; j++) {
			printf("%-3d", map[i][j] != _Infty ? map[i][j] : 0);
		}
		printf("\n");
	}

	// ��ʼ�� dist ����, ���ڼ�¼�� start �����������̾���
	int dist[MAXVERTEX];
	for(int i = 0; i < g->nv; i++) {
		dist[i] = map[start_id][i];
	}

	// dijstra
	int idx, Min;
	for(int j = 1; j < g->nv; j++) {
		Min = _Infty;
		for(int i = 0; i < g->nv; i++) {
			if(!book[i]) {
				if(Min > dist[i]) {
					Min = dist[i];
					idx = i;
				} // ��ȡ��ǰ�� start ����ĵ�
			}
		}
		if(Min == _Infty)
			break; // ��ʱ�Ѿ���չ�굱ǰ��ͨƬ������е�
		book[idx] = 1;
		for(int i = 0; i < g->nv; i++) {
			if(map[idx][i] < _Infty)
				if(dist[i] > dist[idx] + map[idx][i]) {
					dist[i] = dist[idx] + map[idx][i]; // ���� dist ����
				}
		}
	}

	printf("Start from %d\n", start_id);
	for(int i = 0; i < g->nv; i++) {
		if(dist[i] < _Infty) 
			printf("->%d, distance = %d\n", i, dist[i]);
		else
			printf("%d ���ɴ� %d\n", start_id, i);
	}
}

void dfs_for_num(Graph * g, int start) {
	printf("%d ", start);
    int idx_start = getVexIdx(g, start); // ��ʼ����±�
    if(g->v[idx_start].degree == 0) return ; // �ߵ�ͷ��
    int id_next_v; // ��һ���ĵ�� id
    int h;
    for(int i = 0; i < g->v[idx_start].degree; i++) {
        getVexByEdge(g, g->v[idx_start].edges[i], &h, &id_next_v); // �ռ��������ߵ���һ�������е�� id
		if(id_next_v == start) { // ������ͼ����
			id_next_v = h;
		}
        if(!book[id_next_v]) {
            book[id_next_v] = 1; // ���������߹�
            dfs_for_num(g, id_next_v);
        }
    }
}

bool judge(Graph * g) {// �ж��ǲ������е㶼��ǹ���
	for(int i = 0; i < g->nv; i++)
		if(!book[g->v[i].id])
			return true;
	return false;
}

int num(Graph * g) {
	printf("��ʼ����ͨƬ����...\n");
	fflush(stdout);
	for(int i = 0; i < g->nv; i++) {
		book[i] = 0;
	}
	// for(int i = 0; i < MAXVERTEX; ++i) {
	// 	printf("%d ", book[i]);
	// }
	int start;
	int count = 0;
	while(judge(g)) {
		for(int i = 0; i < g->nv; i++) {
			if(!book[g->v[i].id]) {
				start = g->v[i].id;
				printf("\nstart id = %d\n", start);
				fflush(stdout);
				break;
			}
		}
		book[start] = 1;
		dfs_for_num(g, start);
		count++;
	}
	return count;
}

// int * fringe;

// void dfs1(Graph * g, int start, int desti) {
// 	if(start == desti) {
// 		return ;
// 	}
// 	for(int i = 0; i < MAXVERTEX; i++) {
// 		book[i] = 0;
// 	}
// 	book[start] = 1;
// 	int * tail = fringe;
// 	*tail++ = start;
// 	int idx;
// 	int id_next_v, h;
// 	while(tail >  start) {
// 		idx = getVexIdx(g, *tail);
// 		for(int i = 0; i < g->v[idx].degree; i++) {
//         	getVexByEdge(g, g->v[idx].edges[i], &h, &id_next_v); // �ռ��������ߵ���һ�������е�� id
// 			if(id_next_v == idx) {
// 				id_next_v = h;
// 			} // �������ͼ
// 			if(!book[id_next_v]) {

// 			}
// 		}
// 	}




	
}

void ComplexOperation(Graph * g) {
	// DFS
	// printf("Begin dfs operation...\n");
    int start_id = 1, end_id = 5; // start from 1, end with 3. Find 
	fringe = (int *)malloc(sizeof(int) * MAXVERTEX);
	for(int i = 0; i < MAXVERTEX; i++) {
		*(fringe+i) = 0;
	}
	dfs1(g, start_id, end_id);
	
	



	// the nearest path between 2 points
    // book[start_id] = 1;
    // unsigned long dist = 0;
    // printf("The path of dfs:\n");
    // dfs(g, start_id, end_id, &dist);
    // printf("\n�� id = %d �ĵ㵽 id = %d �ĵ����С��Ȩ·��Ϊ %lu\n", start_id, end_id, min);
    
    // // BFS
    // int pace = bfs(g, start_id, end_id);
	// printf("\n�� %d �� %d ��Ҫ %d ��\n",  start_id, end_id, pace);

	// Dijstra
	Dij(g, start_id);

	// ��ͨƬ����
	if(!g->dirctional)
		printf("��ͨƬ����Ϊ��%d\n", num(g));
}


int main() {
	Graph * g = (Graph *)malloc(sizeof(Graph));
	if (!g) {
		printf("Error\n");
		exit(0);
    }
	randgengraph(10, 0.1, 0, 1, "graph.txt", 5);
	initgraph(g, "graph.txt");
	// printgraph(g);
	// saveGraph(g, "Graph1.html");
	
	// int h;
	// int t;
	addVex(g, 30);
    // printgraph(g);
	// saveGraph(g, "Graph2.html");
    addEdge(g, 50, 1, 3);
    // printgraph(g);
	// saveGraph(g, "Graph3.html");
	// getVexByEdge(g, 10, &h, &t);
	// printf("id Ϊ 10 �ıߵ�ͷβ�ڵ��id: %d %d\n", h, t);
	// deleteEdge(g, 10);
    // printgraph(g);
	// saveGraph(g, "Graph4.html");
	// deleteVex(g, 5);
    printgraph(g);
	// saveGraph(g, "Graph5.html");
    ComplexOperation(g);
    // Print_in_markdown(g);
    return 0;
}
