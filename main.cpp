#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
using namespace std;
#define VertexNum 100  //�Ķ�VertexNum��ֵ���ı䶥����Ŀ
#define suiji1 40  //�����������������������ߡ�������Ϊ100��1000ʱ��ֵΪ40��������Ϊ10000ʱ��ֵΪ450,��������ĿҪ��
#define suiji2 1000  //����������������ڸ�Ȩ�������ֵ
#define INF 0x3f3f3f3f
typedef struct Node
{
    int adjvex;  //�ڽӶ�������˳������±�����
    int weight;  //��Ȩ��
    struct Node *next;  //����ͬһ�������һ����
}Node;
typedef struct VertexRecord
{
    struct Node *first;  //ָ�������ö���ĵ�һ���ߵ�ָ��
}VertexRecord;
typedef struct GraphRecord
{
    struct VertexRecord *vertices;  //����
}GraphRecord,*Graph;
int M;  //����ͼ�ߵ���Ŀ����������С������
int matrix[VertexNum + 1][VertexNum + 1];  //������ɵľ���
int matrix2[VertexNum + 1] = {0};  //һά���飬�ж�������ɵ�ͼ�Ƿ���ͨ
ndouble random(double start, double end)  //����start~end֮��������
{
    return start + (end - start)*rand() / (RAND_MAX + 1.0);
}
void RandomUndirectGraph()  //��������ڽӾ���,�������д���ļ�
{
    FILE *fp;
    fp = fopen("F:ͼ����.txt", "w");
    srand(unsigned(time(0)));
    int x;  //���ڲ��������
    char st = ' ';
    for (int i = 0; i < VertexNum; i++)
    {
        for (int j = 0; j < VertexNum; j++)  //��ά�����ʼ��
        {
            matrix[i][j] = 0;
        }
    }
    for (int i = 0; i < VertexNum; i++)
    {
        for (int j = 0; j < VertexNum; j++)
        {
            x = (int)random(0, suiji1);  //������������ɸĶ�
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
    for (int i = 0; i <= VertexNum; i++)  //�ж��������������ͼ�Ƿ���ͨ,����ͨ�ļӱ�
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
        fprintf(fp,"%c\n", st);  //�����ļ��ĸ�ʽ
    }
    fclose(fp);
}
Graph CreateUndirectGraph()  //�������������ͼ������������ͼ�������ʾ
{
    FILE *fp;
    fp = fopen("F:ͼ����.txt", "r");

    //ͼ�ĳ�ʼ��
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

    //ͼ�Ĺ���
    for (int i = 0; i < VertexNum; i++)
    {
        for (int j = i + 1; j < VertexNum; j++)
        {
            fscanf(fp,"%d", &flag);
            if (flag)
            {
                Edgeweight = (int)random(suiji2, suiji2*10);  //��������ߵ�Ȩֵ
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
void print_EdgeAndWeight(Graph G)  //������ͼ�ıߡ�Ȩֵ��Ϣд���ļ�
{
    struct Node *ptr;
    FILE *fp;
    fp = fopen("F:������ɵı߼�Ȩ��.txt", "w");
    int count = 0;  //ͳ�Ʊ���
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
//prim����С������
#define MAX 10001
int graph[MAX][MAX];

int Prim(int graph[][MAX], int n)
{
    FILE *fq;
    fq = fopen("F:���ս��.txt", "w");
    fprintf(fq,"\n���� - �յ�� : Ȩ��(Ԫ)\n");
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
    fprintf(fq,"\n  ��������: %d��\n  ������ɱ���: %d��\n  ����ܵ�����: %d��\n  ����ܻ���: %dԪ", n, M, n-1, sum);
    fclose(fq);
    return sum;
}
int main()
{
    RandomUndirectGraph();
    Graph G = CreateUndirectGraph();
    print_EdgeAndWeight(G);
    FILE *fp;
    fp = fopen("F:������ɵı߼�Ȩ��.txt", "r");
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
    printf("\n*************************  ������򣺳��йܵ���������С�ɱ��������� ************************ \n");
    printf("��������: %d��\n", VertexNum);
    printf("���������������ͼ�߼�Ȩ�أ�");
    system("pause");
    printf("\n������ɱߵ�����: %d��\n", m);
    printf("��������С����������ͷ��ã�");
    system("pause");
    //printf("���� - �յ�� : Ȩ��(Ԫ)\n");
    cost = Prim(graph, n);
    printf("\n�������ܵ�����: %d��\n", n-1);
    printf("��С�����ܼ�: %dԪ\n", cost);
    return 0;
}
