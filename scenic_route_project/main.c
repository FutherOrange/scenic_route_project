#include "graph.h"
#include <stdio.h>
#include <string.h>

#define USER_VISITOR 1
#define USER_ADMIN 2

void VisitorMenu(Graph *g);
void AdminMenu(Graph *g);
void ExtensionInterface(Graph *g);
int GetVertexIndex(Graph *g, char *name);

int main() {
    Graph g;
    LoadSampleData(&g);
    
    int user_type, choice;
    
    printf("========== 景区路径规划系统 ==========\n");
    printf("请选择用户角色:\n");
    printf("1. 景区游客\n");
    printf("2. 景区管理方\n");
    printf("请输入选择: ");
    scanf("%d", &user_type);
    
    while (1) {
        if (user_type == USER_VISITOR) {
            VisitorMenu(&g);
        } else if (user_type == USER_ADMIN) {
            AdminMenu(&g);
        }
        
        printf("\n请选择操作:\n");
        printf("1. 返回菜单\n");
        printf("2. 退出系统\n");
        printf("请输入选择: ");
        scanf("%d", &choice);
        
        if (choice == 2) {
            printf("感谢使用景区路径规划系统!\n");
            break;
        }
    }
    
    return 0;
}

void VisitorMenu(Graph *g) {
    int choice, start, end;
    char start_name[20], end_name[20];
    
    printf("\n========== 游客菜单 ==========\n");
    printf("1. 查看景点分布图\n");
    printf("2. DFS导游路线\n");
    printf("3. 回路检测\n");
    printf("4. 最短路径查询\n");
    printf("5. 扩展功能\n");
    printf("请输入选择: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            PrintMatrix(g);
            break;
        case 2:
            printf("请输入起点景点名称: ");
            scanf("%s", start_name);
            start = GetVertexIndex(g, start_name);
            if (start == -1) {
                printf("景点不存在!\n");
                break;
            }
            DFS(g, start);
            break;
        case 3:
            printf("请输入起点景点名称: ");
            scanf("%s", start_name);
            start = GetVertexIndex(g, start_name);
            if (start == -1) {
                printf("景点不存在!\n");
                break;
            }
            DFS(g, start);
            DetectCycle(g);
            break;
        case 4:
            printf("请输入起点景点名称: ");
            scanf("%s", start_name);
            printf("请输入终点景点名称: ");
            scanf("%s", end_name);
            start = GetVertexIndex(g, start_name);
            end = GetVertexIndex(g, end_name);
            if (start == -1 || end == -1) {
                printf("景点不存在!\n");
                break;
            }
            Dijkstra(g, start, end);
            break;
        case 5:
            ExtensionInterface(g);
            break;
        default:
            printf("无效选择!\n");
    }
}

void AdminMenu(Graph *g) {
    int choice, from, to, weight;
    char name[20], from_name[20], to_name[20];
    
    printf("\n========== 管理方菜单 ==========\n");
    printf("1. 查看景点分布图\n");
    printf("2. DFS导游路线\n");
    printf("3. 回路检测\n");
    printf("4. 最短路径查询\n");
    printf("5. 添加景点\n");
    printf("6. 添加路径\n");
    printf("7. 修改路径距离\n");
    printf("8. 扩展功能\n");
    printf("请输入选择: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            PrintMatrix(g);
            break;
        case 2:
            printf("请输入起点景点名称: ");
            scanf("%s", name);
            from = GetVertexIndex(g, name);
            if (from == -1) {
                printf("景点不存在!\n");
                break;
            }
            DFS(g, from);
            break;
        case 3:
            printf("请输入起点景点名称: ");
            scanf("%s", name);
            from = GetVertexIndex(g, name);
            if (from == -1) {
                printf("景点不存在!\n");
                break;
            }
            DFS(g, from);
            DetectCycle(g);
            break;
        case 4:
            printf("请输入起点景点名称: ");
            scanf("%s", from_name);
            printf("请输入终点景点名称: ");
            scanf("%s", to_name);
            from = GetVertexIndex(g, from_name);
            to = GetVertexIndex(g, to_name);
            if (from == -1 || to == -1) {
                printf("景点不存在!\n");
                break;
            }
            Dijkstra(g, from, to);
            break;
        case 5:
            printf("请输入新景点名称: ");
            scanf("%s", name);
            AddVertex(g, name);
            printf("景点添加成功!\n");
            break;
        case 6:
            printf("请输入起点景点名称: ");
            scanf("%s", from_name);
            printf("请输入终点景点名称: ");
            scanf("%s", to_name);
            printf("请输入路径距离: ");
            scanf("%d", &weight);
            from = GetVertexIndex(g, from_name);
            to = GetVertexIndex(g, to_name);
            if (from == -1 || to == -1) {
                printf("景点不存在!\n");
                break;
            }
            AddEdge(g, from, to, weight);
            printf("路径添加成功!\n");
            break;
        case 7:
            printf("请输入起点景点名称: ");
            scanf("%s", from_name);
            printf("请输入终点景点名称: ");
            scanf("%s", to_name);
            printf("请输入新路径距离: ");
            scanf("%d", &weight);
            from = GetVertexIndex(g, from_name);
            to = GetVertexIndex(g, to_name);
            if (from == -1 || to == -1) {
                printf("景点不存在!\n");
                break;
            }
            AddEdge(g, from, to, weight);
            printf("路径距离修改成功!\n");
            break;
        case 8:
            ExtensionInterface(g);
            break;
        default:
            printf("无效选择!\n");
    }
}

void ExtensionInterface(Graph *g) {
    int choice;
    
    printf("\n========== 扩展功能接口 ==========\n");
    printf("预留扩展功能区域，可在此添加:\n");
    printf("1. 景点信息查询\n");
    printf("2. 路径信息统计\n");
    printf("3. 返回主菜单\n");
    printf("请输入选择: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            printf("景点数量: %d\n", g->vertex_count);
            printf("景点列表:\n");
            for (int i = 0; i < g->vertex_count; i++) {
                printf("%d. %s\n", i + 1, g->vertex[i].name);
            }
            break;
        case 2:
            printf("路径统计功能开发中...\n");
            break;
        case 3:
            break;
        default:
            printf("无效选择!\n");
    }
}

int GetVertexIndex(Graph *g, char *name) {
    for (int i = 0; i < g->vertex_count; i++) {
        if (strcmp(g->vertex[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}
