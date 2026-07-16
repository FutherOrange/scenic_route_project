#include "graph.h"
#include <stdio.h>
#include <string.h>

void InitGraph(Graph *g) {
    int i, j;
    g->vertex_count = 0;
    for (i = 0; i < MAX_VERTEX; i++) {
        for (j = 0; j < MAX_VERTEX; j++) {
            g->matrix[i][j] = INF;
        }
        g->matrix[i][i] = 0;
    }
}

void AddVertex(Graph *g, char *name) {
    strcpy(g->vertex[g->vertex_count].name, name);
    g->vertex_count++;
}

void AddEdge(Graph *g, int from, int to, int weight) {
    g->matrix[from][to] = weight;
    g->matrix[to][from] = weight;
}

void PrintMatrix(Graph *g) {
    int i, j;
    printf("邻接矩阵分布图:\n");
    printf("%-12s", "景点");
    for (i = 0; i < g->vertex_count; i++) {
        printf("%-12s", g->vertex[i].name);
    }
    printf("\n");
    for (i = 0; i < g->vertex_count; i++) {
        printf("%-12s", g->vertex[i].name);
        for (j = 0; j < g->vertex_count; j++) {
            if (g->matrix[i][j] == INF) {
                printf("%-12d", INF);
            } else {
                printf("%-12d", g->matrix[i][j]);
            }
        }
        printf("\n");
    }
}

static int visited[MAX_VERTEX];
static int path[MAX_VERTEX];
static int path_len;

void DFSHelper(Graph *g, int current) {
    int i;
    visited[current] = VISITED;
    path[path_len++] = current;
    for (i = 0; i < g->vertex_count; i++) {
        if (g->matrix[current][i] != INF && g->matrix[current][i] != 0 && visited[i] == UNVISITED) {
            DFSHelper(g, i);
        }
    }
}

void DFS(Graph *g, int start) {
    int i;
    for (i = 0; i < MAX_VERTEX; i++) {
        visited[i] = UNVISITED;
    }
    path_len = 0;
    DFSHelper(g, start);
    printf("DFS导游路线: ");
    for (i = 0; i < path_len; i++) {
        printf("%s", g->vertex[path[i]].name);
        if (i < path_len - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
}

void DetectCycle(Graph *g) {
    int i, j;
    for (i = 0; i < path_len; i++) {
        for (j = i + 1; j < path_len; j++) {
            if (path[i] == path[j]) {
                printf("检测到回路: ");
                for (int k = i; k <= j; k++) {
                    printf("%s", g->vertex[path[k]].name);
                    if (k < j) {
                        printf(" -> ");
                    }
                }
                printf("\n");
                return;
            }
        }
    }
    printf("未检测到回路\n");
}

void Dijkstra(Graph *g, int start, int end) {
    int dist[MAX_VERTEX], prev[MAX_VERTEX], visited_dijk[MAX_VERTEX];
    int i, j, min_dist, u;
    
    for (i = 0; i < g->vertex_count; i++) {
        dist[i] = g->matrix[start][i];
        if (dist[i] != INF && i != start) {
            prev[i] = start;
        } else {
            prev[i] = -1;
        }
        visited_dijk[i] = UNVISITED;
    }
    visited_dijk[start] = VISITED;
    
    for (i = 1; i < g->vertex_count; i++) {
        min_dist = INF;
        u = -1;
        for (j = 0; j < g->vertex_count; j++) {
            if (visited_dijk[j] == UNVISITED && dist[j] < min_dist) {
                min_dist = dist[j];
                u = j;
            }
        }
        if (u == -1) break;
        visited_dijk[u] = VISITED;
        
        for (j = 0; j < g->vertex_count; j++) {
            if (visited_dijk[j] == UNVISITED && g->matrix[u][j] != INF && dist[u] + g->matrix[u][j] < dist[j]) {
                dist[j] = dist[u] + g->matrix[u][j];
                prev[j] = u;
            }
        }
    }
    
    if (dist[end] == INF) {
        printf("无法从%s到达%s\n", g->vertex[start].name, g->vertex[end].name);
        return;
    }
    
    int path_result[MAX_VERTEX], path_index = 0;
    int temp = end;
    while (temp != -1) {
        path_result[path_index++] = temp;
        temp = prev[temp];
    }
    
    printf("最短路径: ");
    for (i = path_index - 1; i >= 0; i--) {
        printf("%s", g->vertex[path_result[i]].name);
        if (i > 0) {
            printf(" -> ");
        }
    }
    printf("\n最短距离: %d\n", dist[end]);
}

void LoadSampleData(Graph *g) {
    InitGraph(g);
    AddVertex(g, "大门");
    AddVertex(g, "碧玉潭");
    AddVertex(g, "红叶林");
    AddVertex(g, "赏花园");
    AddVertex(g, "飞云石");
    AddVertex(g, "九曲桥");
    AddVertex(g, "白虎岩");
    AddVertex(g, "观月阁");
    
    AddEdge(g, 0, 1, 50);
    AddEdge(g, 0, 2, 80);
    AddEdge(g, 1, 3, 60);
    AddEdge(g, 1, 4, 40);
    AddEdge(g, 2, 5, 70);
    AddEdge(g, 3, 6, 90);
    AddEdge(g, 4, 6, 30);
    AddEdge(g, 5, 7, 50);
    AddEdge(g, 6, 7, 45);
}
