#ifndef _MYHUFFMANTREE_CPP_
#define _MYHUFFMANTREE_CPP_
//哈夫曼树 ：分配连续存储空间存储树 

#include <stdio.h>
#include <stdlib.h>

#define TElemType long

//编码的最大长度 
#define MCL 30 
//定义叶节点数,8bit的unsigned char 
#define n 256
//定义结点总数，2n-1 
#define m 511 

//定义树结点，包括三个指针父节点，左右孩子 
typedef struct HTNode{
	int id;
	TElemType w; 
	struct HTNode * parent, * lchild, * rchild;
} HTNode;

typedef struct {
	unsigned code; // 前面都是 0，最后 len 位是编码
	int len; // 编码长度
} HCode;

HTNode * CreateNode(TElemType w) {
	HTNode * node = (HTNode *)malloc(sizeof(HTNode));
	if(!node) {
		printf("内存不足，分配空间失败。\n");
		exit(0);
	}
	node->lchild = NULL;
	node->parent = NULL;
	node->rchild = NULL;
	node->w = w;
	return node;
}

int CreateHTree(HTNode * Tree[], TElemType * w) {
	long allw = 0; // 字符总数
	int numZero = 0; // 没出现的字符的个数
	for(int i = 0; i < m; i++) {
		Tree[i] = CreateNode(w[i]);
		Tree[i]->id = i;
		if(i < n) {
			if(!w[i]) numZero++; // 统计没出现的字符的个数
			allw += w[i]; 
		}
	} // 初始化 Huffman Tree，每个结点构成一棵树🌲
	
	int i = n, j; // 从 n 开始
	TElemType min1, min2;
	while(i < m - numZero) { // i < m - numZero
		min1 = min2 = allw + 10; // 设置最大的 w 值
		unsigned pos1, pos2;
		for(j = 0; j < i; ++j) {
			if(!Tree[j]->w)
				continue; // 忽略权值为 0 的顶点
			if(!Tree[j]->parent) { // 没有父节点
				if(Tree[j]->w < min1) {
					pos2 = pos1;
					min2 = min1;
					pos1 = j;
					min1 = Tree[j]->w;
				} // min1 <= min2, 找到比 min1 更小的Tree[j].w, 那么min1 替代min2，Tree[j].w 代替 min1.
				else {
					if(Tree[j]->w < min2) { 
						// min1 <= ht[j].w < min2
						pos2 = j;
						min2 = Tree[j]->w;
					}
				}
			}
		} // 内层 for 循环结束，找到 pos1，pos2 和最小的两个权值，准备构造非叶结点
		Tree[i]->lchild = Tree[pos1];
		Tree[i]->rchild = Tree[pos2];
		Tree[i]->w = Tree[pos1]->w + Tree[pos2]->w;
		Tree[pos1]->parent = Tree[pos2]->parent = Tree[i];
		i++;
	}
	return m - numZero - 1;
}

void PrintHTree(HTNode * root) {
	if(!root) return ;
	if(root->w)
		printf("(%ld), ", root->w);
	if(root->lchild) PrintHTree(root->lchild);
	if(root->rchild) PrintHTree(root->rchild);
}

unsigned char * parseFile(const char filename[], long * w, long * fsize) {
	FILE * fp = fopen(filename , "rb");
	if(fp == NULL) {
		printf("无法打开文件，请检查是否有相应文件\n");
		exit(0);
	}
	fseek(fp, 0, SEEK_END);
	*fsize = ftell(fp);
	rewind(fp);
	unsigned char * dataArray = (unsigned char *)malloc(sizeof(unsigned char) * (*fsize));
	if(!dataArray) {
		printf("文件太大， 内存不够，读入错误！\n");
		exit(0);
	}
	fread(dataArray, sizeof(unsigned char), *fsize, fp);
	fclose(fp);

	for(int i = 0; i < n; i++) {
		w[i] = 0; // 权值清空
	}
	for(int i = 0; i < *fsize; i++) {
		w[dataArray[i]]++; // 若对应字符出现，权值+1
	}

	return dataArray;
}

// 输入任何字符 c，得到它的哈夫曼编码, 被genCodes()调用 
void getCode(HTNode ** Tree, HTNode * c, unsigned * code1, int * clen1) {
	int clen = 0; // 编码长度
	unsigned code = 0; // 编码

	int size = sizeof(code); // 编码最大 bits 数
	unsigned mask = 1 << (size * 8 - 1); // 最高位为 1，其他为 0
	HTNode * parent = c->parent;
	while(parent != NULL) {
		code = code >> 1;
		parent->lchild == c ? code : (code = (code | mask));
		// 如果是右孩子，那么最高位设置为1，否则就是0 
		clen++;
		c = parent; // 更新 c
		parent = c->parent; // 更新 parent
	}
	// code的前面clen位是哈夫曼编码，将其移动到最后
	code = code >> (size * 8 - clen);
	// printf("0x%x-%d\n",code,clen);
	*code1 = code; // 返回值
	*clen1 = clen; // 返回值
}

// 从哈夫曼树ht生成完整的编码表hc，输出hc为encode()的输入参数 
void genHCodes(HCode hc[], HTNode ** Tree) {
	for (int i = 0; i < n; ++i) {
		if (Tree[i]->w > 0) {
			getCode(Tree, Tree[i], &(hc[i].code), &(hc[i].len));
		}
	}
}

/*** 以下为编解码实现 ***/
// 编码,原长olen的内容orgi，压缩编码后为长nlen的内容newc，需要利用编码表hc 
void encode(unsigned char * orgi, long olen, unsigned char * newc, long * nlen, HCode hc[]) {
	int sizec = sizeof(unsigned int); // 要确保字符编码的最大长度要小于sizec*8-8 
	long i = 0; // orgi的下标 
	long j = 0; // newc的下标 
	newc[j] = 0;
	unsigned code;
	int len;
	int usedbits = 0; // newc[j]中被占用的bits数 
	while(i < olen) { // 对原始数据逐个处理
		int idx = orgi[i];
		code = hc[idx].code;
		len = hc[idx].len;
		while(len + usedbits >= 8) { // code 需要左移 usedbits 长的一段，然后和 newc[j] 位或操作
			unsigned code1 = code >> (len + usedbits - 8);
			newc[j] = newc[j] | code1; // 填满前一个编码字符
			len = len + usedbits - 8; // code 现在的有效长度
			usedbits = 0;
			newc[++j] = 0;
			// printf("\n编码的第 %d 个字符为：#%2x\n",j-1,newc[j-1]);
		} // 循环，使得 len + usedbits < 8
		//将code的前面各位清0,因为有效位是len
		code = code & ((1 << len) - 1); // code 和后面 k 个 1，其它全为 0 的数据进行与，保留后面 len 位 
		code = code << (8 - len - usedbits); // 向前移动若干位 
		newc[j] = newc[j] | code;
		usedbits = len + usedbits;
		i++;
	}
	*nlen = j+1;
}


typedef struct {
	TElemType w;
	int parent;
	int lchild;
	int rchild;
} IDX; // 在压缩时用来存储父子关系


void zip(char fin[], char fout[]) {
	HTNode * root; // 表示 Huffman 树
	HTNode * Tree[m]; 
	IDX Tidx[m];
	HCode hc[n]; // 保存 Huffman 编码表: code, len, 用于编码文件
	long wDist[512]; // 保存字符的分布（字符在文件中出现的次数）
	for(int i = 0; i < 512; i++) {
		wDist[i] = 0;
	}
	long fsize; // 文件长度
	// 获取文件内容，分析待压缩文件，返回权值向量 wDist 文件，文件内容 content 和长度 fsize
	unsigned char * content = parseFile(fin, wDist, &fsize);
	
	// 生成 Huffman 树
	int Hroot = CreateHTree(Tree, wDist); // Hroot是哈夫曼树 Tree 的根结点的下标 
	for(int i = 0; i <= Hroot; i++) {
		Tidx[i].w = Tree[i]->w;
		if(!Tree[i]->parent) Tidx[i].parent = -1;
		else Tidx[i].parent = Tree[i]->parent->id;
		if(!Tree[i]->lchild) Tidx[i].lchild = -1;
		else Tidx[i].lchild = Tree[i]->lchild->id;
		if(!Tree[i]->rchild) Tidx[i].rchild = -1;
		else Tidx[i].rchild = Tree[i]->rchild->id;
	}
	genHCodes(hc, Tree);
	root = Tree[Hroot];
	// PrintHTree(root);
	unsigned char * zipContent; // 编码后的内容
	long zipsize; // 压缩之后文件的大小
	zipContent = (unsigned char *)malloc(sizeof(unsigned char) * (fsize + 10000));
	if(!zipContent) {
		printf("文件太大，内存不够，读入错误\n");
		exit(0);
	}
	encode(content, fsize, zipContent, &zipsize, hc); // 编码后返回长度zipsize的内容zipContent 

	FILE * fp = fopen(fout, "wb");
	if(fp == NULL) {
		printf("写入文件不存在，请检查文件名是否正确\n");
		exit(0);
	}
	long ht_size = sizeof(IDX) * m;   //哈夫曼编码表的大小 
	fwrite(&ht_size, sizeof(ht_size), 1, fp); //保存哈夫曼树的大小
	fwrite(&zipsize, sizeof(zipsize), 1, fp); //保存编码内容的大小 
	fwrite(&fsize, sizeof(fsize), 1, fp);     //保存原始内容的大小 
	fwrite(&Hroot, sizeof(Hroot), 1, fp);     //保存哈夫曼树根节点
	fwrite(Tidx, sizeof(IDX), m, fp);      //保存哈夫曼树 ，解码要用到 
	// for(int i = 0; i <= Hroot; i++) {
		// fwrite(&(Tree[i]->w), sizeof(TElemType), 1, fp);
		// fwrite(Tidx+i, sizeof(IDX), 1, fp);
		// printf("w = %-4ld, parent = %-4d, lchild = %-4d, rchild = %-4d\n", Tidx[i].w, Tidx[i].parent, Tidx[i].lchild, Tidx[i].rchild);
	// }

	fwrite(zipContent, sizeof(unsigned char), zipsize, fp);//保存编码后的内容 
	fclose(fp);

	free(zipContent);
	free(content);
	// printf("Hroot = %d\n", Hroot);
	// printf("树的大小为：%ld\n", ht_size);
	// printf("被压缩后内容大小为：%ld\n", zipsize);
	// printf("被压缩内容的大小：%ld\n", fsize);
	printf("压缩文件 %s 已经生成！\n", fout);
}

// 读取压缩文件，解压
void unzip(char zfile[], char ofile[]) {
	IDX Tidx[m];
	FILE * fp = fopen(zfile, "rb");
	if(fp == NULL) {
		printf("无法打开压缩文件进行读取.\n");
		exit(0);
	}

	printf("成功打开压缩文件！\n");
	long ht_size1, zipsize1, fsize1;
	int Hroot;  // 下标
	fread(&ht_size1, sizeof(ht_size1), 1, fp); // Huffman 树大小（字节数）
	fread(&zipsize1, sizeof(zipsize1), 1, fp); // 压缩后内容的大小
	fread(&fsize1, sizeof(fsize1), 1, fp); 	   // 被压缩内容的大小
	// printf("树的大小为：%ld\n", ht_size1);
	// printf("被压缩后内容大小为：%ld\n", zipsize1);
	// printf("被压缩内容的大小：%ld\n", fsize1);
	fread(&Hroot, sizeof(Hroot), 1, fp); 	   // 根的下标
	// printf("成功读取下标, 下标为: ");
	// printf("Hroot = %d\n", Hroot);
	fflush(stdout);
	unsigned char * zcontent = (unsigned char *)malloc(zipsize1);
	unsigned char * ocontent = (unsigned char *)malloc(fsize1); // 解码后的数据
	HTNode * Tree[m];
	for(int i = 0; i < m; i++) {
		Tree[i] = (HTNode*)malloc(sizeof(HTNode));
		if(!Tree[i]) {
			printf("内存不足, 分配失败\n");
			exit(0);
		}
		Tree[i]->id = i;
		Tree[i]->w = 0;
		Tree[i]->lchild = Tree[i]->parent = Tree[i]->rchild = NULL;
	}

	// for(int i = 0; i <= Hroot; i++) {
		// fread(&(Tree[i]->w), sizeof(TElemType), 1, fp);
		// fread(Tidx+i, sizeof(IDX), 1, fp);
	// }
	fread(Tidx, ht_size1, 1, fp); // 读取 Huffman 🌲
	fread(zcontent, zipsize1, 1, fp); // 读取编码数据
	fclose(fp);

	printf("成功读取完毕\n");
	// for(int i = 0; i <= Hroot; i++) {
	// 	printf("w = %-4ld, parent = %-4d, lchild = %-4d, rchild = %-4d\n", Tidx[i].w, Tidx[i].parent, Tidx[i].lchild, Tidx[i].rchild);
	// fflush(stdout);
	// }
	for(int i = 0; i <= Hroot; i++) {
		Tree[i]->w = Tidx[i].w;
		if(Tidx[i].lchild != -1) Tree[i]->lchild = Tree[Tidx[i].lchild];
		if(Tidx[i].rchild != -1) Tree[i]->rchild = Tree[Tidx[i].rchild];
		if(Tidx[i].parent != -1) Tree[i]->parent = Tree[Tidx[i].parent];
		// printf("w = %-4ld, ", Tree[i]->w);
		// if(Tree[i]->parent) printf("parent = %-4d, ", Tree[i]->parent->id);
		// else printf("parent = %-4d, ", -1);
		// if(Tree[i]->lchild) printf("lchild = %-4d, ", Tree[i]->lchild->id);
		// else printf("lchild = %-4d, ", -1);
		// if(Tree[i]->rchild) printf("rchild = %-4d\n", Tree[i]->rchild->id);
		// else printf("rchild = %-4d\n", -1);
	}

	fp = fopen(ofile, "wb");
	if(fp == NULL) {
		printf("无法打开解压后文件进行解压\n");
		exit(0);
	}
	int idx = Hroot; // 从根开始
	int j, k = 0, i = 0; // zcontent 的下标 k，ocontent 的下标 i
	while(i < fsize1) { // 依次写入解压后数据的每个字节
		j = 128; // 10000000
		
		while(j > 0) {
			if((zcontent[k] & j) > 0) { // 向右走 1
				if(Tree[idx]->rchild == NULL) {
					ocontent[i++] = idx;
					idx = Hroot;
					j = j << 1;
				} 
				else {
					idx = Tree[idx]->rchild->id;
				}
			}
			else { // 向左走 0
				if(Tree[idx]->lchild == NULL) {
					ocontent[i++] = idx;
					idx = Hroot;
					j = j << 1;
				}
				else {
					idx = Tree[idx]->lchild->id;
				}
			}
			j = j >> 1;
		}
		k++;
	}
	fwrite(ocontent, fsize1, 1, fp);
	
	fclose(fp);
	free(ocontent);
	free(zcontent);
	printf("文件 %s 已经成功解压为 %s \n", zfile, ofile);
}


int check(char file1[], char file2[]){	// 返回1说明文件一样，返回0说明文件不一样
	FILE *fp1 = fopen(file1,"r"), *fp2 = fopen(file2,"r");
	unsigned fsize1,fsize2;
	fseek(fp1, 0, SEEK_END);	fseek(fp2, 0, SEEK_END);
	fsize1 = ftell(fp1);		fsize2 = ftell(fp2);
	rewind(fp1);				rewind(fp2);
	if(fsize1 != fsize2) return 0;
	char c1,c2;
	for(unsigned i = 0; i < fsize1; ++i){
		fread(&c1,1,1,fp1);		fread(&c2,1,1,fp2);
		if(c1 != c2) return printf("at fsize = %d, c1 is %d, c2 is %d\n",i,c1,c2);
	}
	fclose(fp1);				fclose(fp2);
	return 1;
}

/// @brief 
/// @return 
int main() {
	char infile[256] = "lab6.pdf"; 		// 被压缩文件名
	char zfile[256] = "lab6pdf.myzip"; // 压缩文件名
	char ofile[256] = "myout_lab6.pdf";	// 解压文件名
	printf("\n压缩一个PDF:%s ...\n", infile);
	zip(infile, zfile);	 // 压缩
	printf("压缩完成...");
	printf("正在解压压缩的PDF...\n");
	unzip(zfile, ofile); // 解压
	// 压缩一个图片
	
	if(check(infile, ofile)) {
		printf("压缩前和解压后一样...\n");
	}
	else {
		printf("压缩前和解压后不一样...\n");
	}

	char infile1[256] = "lab6.c";
	char zfile1[256] = "lab6c.myzip";
	char ofile1[256] = "myout_lab6.c";

	printf("\n压缩一个 C 文档:%s ...\n", infile1);
	zip(infile1, zfile1);	 // 压缩
	printf("压缩完成...");
	printf("正在解压压缩的 C 文档...\n");
	unzip(zfile1, ofile1); // 解压

	if(check(infile1, ofile1)) {
		printf("压缩前和解压后一样...\n");
	}
	else {
		printf("压缩前和解压后不一样...\n");
	}

	char infile2[256] = "pic.png";
	char zfile2[256] = "picpng.myzip";
	char ofile2[256] = "myout_pic.png";
	printf("\n压缩一张图片:%s ...\n", infile2);
	zip(infile2, zfile2);	 // 压缩
	printf("压缩完成...");
	printf("正在解压压缩的 C 文档...\n");
	unzip(zfile2, ofile2); // 解压

	if(check(infile1, ofile1)) {
		printf("压缩前和解压后一样...\n");
	}
	else {
		printf("压缩前和解压后不一样...\n");
	}
	return 0;
}
#endif
