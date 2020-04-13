#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
using namespace std;
#define VertexNum 100  //改动VertexNum的值来改变顶点数目
#define suiji1 40  //产生随机数，用于生成随机边。当顶点为100和1000时，值为40；当顶点为10000时，值为450,可满足题目要求
#define suiji2 1000  //产生的随机数，用于给权重随机赋值
#define INF 0x3f3f3f3f
typedef struct Node
{
    int adjvex;  //邻接顶点所在顺序表中下标索引
    int weight;  //边权重
    struct Node *next;  //依附同一顶点的下一条边
}Node;
typedef struct VertexRecord
{
    struct Node *first;  //指向依附该顶点的第一条边的指针
}VertexRecord;
typedef struct GraphRecord
{
    struct VertexRecord *vertices;  //数组
}GraphRecord,*Graph;
int M;  //无向图边的数目，用于求最小生成树
int matrix[VertexNum + 1][VertexNum + 1];  //随机生成的矩阵
int matrix2[VertexNum + 1] = {0};  //一维数组，判断随机生成的图是否连通
ndouble random(double start, double end)  //产生start~end之间的随机数
{
    return start + (end - start)*rand() / (RAND_MAX + 1.0);
}
void RandomUndirectGraph()  //随机产生邻接矩阵,并将结果写入文件
{
    FILE *fp;
    fp = fopen("F:图矩阵.txt", "w");
    srand(unsigned(time(0)));
    int x;  //用于产生随机数
    char st = ' ';
    for (int i = 0; i < VertexNum; i++)
    {
        for (int j = 0; j < VertexNum; j++)  //二维数组初始化
        {
            matrix[i][j] = 0;
        }
    }
    for (int i = 0; i < VertexNum; i++)
    {
        for (int j = 0; j < VertexNum; j++)
        {
            x = (int)random(0, suiji1);  //产生随机数，可改动
            if(matrix[i][j] == 1)
            {
                continue;
            }
            if ((i != j) && (x < 5))
            {
                matrix[i][j] = 1;
                matrix[j][i] = 1;
                matrix2[i] = 1;
                matrix2[j] = 1;
            }
        }
    }
    for (int i = 0; i <= VertexNum; i++)  //判断随机产生的无向图是否连通,不连通的加边
    {
        if ((matrix2[i] == 0) && (i != VertexNum))
        {
            matrix[i][i+1] = 1;
            matrix2[i] = 1;
        }
        if ((matrix2[i] == 0) && (i == VertexNum))
        {
            matrix[i-1][i] = 1;
            matrix2[i] = 1;
        }
    }
    for (int i = 0; i < VertexNum; i++)
    {
        for (int j = 0; j < VertexNum; j++)
        {
            fprintf(fp,"%d ", matrix[i][j]);
        }
        fprintf(fp,"%c\n", st);  //控制文件的格式
    }
    fclose(fp);
}
Graph CreateUndirectGraph()  //根据随机产生的图矩阵生成无向图的链表表示
{
    FILE *fp;
    fp = fopen("F:图矩阵.txt", "r");

    //图的初始化
    Graph G;
    struct Node *ptr;
    G = (struct GraphRecord*)malloc(sizeof(struct GraphRecord));
    G->vertices = (struct VertexRecord*)malloc(sizeof(struct VertexRecord)*VertexNum);
    for (int i = 0; i < VertexNum; i++)
    {
        G->vertices[i].first = NULL;
    }

    int flag;
    int Edgeweight;
    srand(unsigned(time(0)));

    //图的构建
    for (int i = 0; i < VertexNum; i++)
    {
        for (int j = i + 1; j < VertexNum; j++)
        {
            fscanf(fp,"%d", &flag);
            if (flag)
            {
                Edgeweight = (int)random(suiji2, suiji2*10);  //随机产生边的权值
                ptr = (struct Node*)malloc(sizeof(struct Node));
                ptr->adjvex = j;
                ptr->weight = Edgeweight;
                ptr->next = G->vertices[i].first;
                G->vertices[i].first = ptr;
            }
        }
    }
    fclose(fp);
    return G;
}
void print_EdgeAndWeight(Graph G)  //将无向图的边、权值信息写入文件
{
    struct Node *ptr;
    FILE *fp;
    fp = fopen("F:随机生成的边及权重.txt", "w");
    int count = 0;  //统计边数
    for (int i = 0; i < VertexNum; i++)
    {
        ptr = G->vertices[i].first;
        while (ptr != NULL)
        {
            fprintf(fp,"%d %d %d\n", i+1,ptr->adjvex+1, ptr->weight);
            ptr = ptr->next;
            count++;
        }
    }
    fclose(fp);
    M=count;
}
//prim求最小生成树
#define MAX 10001
int graph[MAX][MAX];

int Prim(int graph[][MAX], int n)
{
    FILE *fq;
    fq = fopen("F:最终结果.txt", "w");
    fprintf(fq,"\n起点城 - 终点城 : 权重(元)\n");
    int lowcost[MAX], st[MAX];
    int i, j, min, mini, sum = 0;
    for (i = 2; i <= n; i++)
    {
        lowcost[i] = graph[1][i];
        st[i] = 1;
    }
    st[1] = 0;
    for (i = 2; i <= n; i++)
    {
        min = INF;
        mini = 0;
        for (j = 2; j <= n; j++)
        {
            if ((lowcost[j] < min) && (lowcost[j] != 0))
            {
                min = lowcost[j];
                mini = j;
            }
        }
        fprintf(fq,"%6d   %6d    %6d\n", st[mini], mini, min);
        //printf("%5d  - %5d  : %5d \n", st[mini], mini, min);
        sum += min;
        lowcost[mini] = 0;
        for (j = 2; j <= n; j++)
        {
            if (graph[mini][j] < lowcost[j])
            {
                lowcost[j] = graph[mini][j];
                st[j] = mini;
            }
        }
    }
    fprintf(fq,"\n  城市总数: %d个\n  随机生成边数: %d条\n  铺设管道总数: %d条\n  最低总花费: %d元", n, M, n-1, sum);
    fclose(fq);
    return sum;
}
int main()
{
    RandomUndirectGraph();
    Graph G = CreateUndirectGraph();
    print_EdgeAndWeight(G);
    FILE *fp;
    fp = fopen("F:随机生成的边及权重.txt", "r");
    int n = VertexNum, m = M, weight, cost=0;
    int chx, chy, i, j;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            graph[i][j] = INF;
        }
    }
    for (int k = 0; k < m; k++)
    {
        fscanf(fp,"%d %d %d", &chx, &chy, &weight);
        i = chx;
        j = chy;
        graph[i][j] = weight;
        graph[j][i] = weight;
    }
    printf("\n*************************  课设程序：城市管道铺设中最小成本计算问题 ************************ \n");
    printf("城市总数: %d个\n", VertexNum);
    printf("下面随机生成无向图边及权重，");
    system("pause");
    printf("\n随机生成边的总数: %d条\n", m);
    printf("下面求最小生成树和最低费用，");
    system("pause");
    //printf("起点城 - 终点城 : 权重(元)\n");
    cost = Prim(graph, n);
    printf("\n最佳铺设管道条数: %d条\n", n-1);
    printf("最小费用总计: %d元\n", cost);
    return 0;
}
