#ifndef GRAPH_H
#define GRAPH_H

#define MAX_VERTEX 20
#define INF 32767
#define VISITED 1
#define UNVISITED 0

typedef struct {
    char name[20];
} Vertex;

typedef struct {
    Vertex vertex[MAX_VERTEX];
    int matrix[MAX_VERTEX][MAX_VERTEX];
    int vertex_count;
} Graph;

void InitGraph(Graph *g);
void AddVertex(Graph *g, char *name);
void AddEdge(Graph *g, int from, int to, int weight);
void PrintMatrix(Graph *g);
void DFS(Graph *g, int start);
void DetectCycle(Graph *g);
void Dijkstra(Graph *g, int start, int end);
void LoadSampleData(Graph *g);

#endif
