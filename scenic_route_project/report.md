# 景区路径规划系统期末大作业报告

---

## 一、项目需求

### 1.1 项目背景

随着旅游业的快速发展，景区游客数量持续增长，游客在游览过程中面临着路径选择困难、游览效率低下等问题。传统的纸质地图已无法满足游客对智能导航、最优路径规划的需求。因此，开发一套**景区路径规划系统**具有重要的现实意义，能够帮助游客快速找到最优游览路线，提升游览体验。

### 1.2 业务需求

- **景点管理**：景区管理方需要能够录入、修改景点信息及景点间路径距离
- **路线规划**：游客需要获取从起点到终点的最优路径
- **导游服务**：提供深度优先遍历的导游路线，帮助游客全面游览景区
- **回路检测**：检测导游路线中是否存在重复访问的景点，避免走回头路
- **权限管理**：区分游客和管理方的操作权限，保障系统安全

### 1.3 功能需求

| 功能模块 | 功能描述 | 优先级 |
|----------|----------|--------|
| 创建景区景点分布图 | 录入景点数量、名称、景点间路径距离，采用邻接矩阵存储 | 高 |
| 展示邻接矩阵分布图 | 完整打印邻接矩阵，无连通填32767，自身距离为0 | 高 |
| DFS深度优先遍历导游路线 | 输入起点，遍历全部可达景点，输出完整游览路线 | 高 |
| 回路检测功能 | 遍历导游路线，查找重复访问景点，输出回路段落 | 高 |
| Dijkstra最短路径算法 | 输入起点、终点，输出最短路径及总距离 | 高 |
| 双角色权限区分 | 游客仅查询，管理方可维护图结构 | 高 |
| 自定义扩展功能接口 | 预留扩展功能区域 | 中 |

### 1.4 用户角色需求

**景区游客**：
- 查看景点分布图
- 获取DFS导游路线
- 回路检测
- 最短路径查询
- 访问扩展功能

**景区管理方**：
- 查看景点分布图
- 获取DFS导游路线
- 回路检测
- 最短路径查询
- 添加景点
- 添加/修改路径
- 访问扩展功能

---

## 二、项目设计

### 2.1 整体实现思路

本系统采用**模块化设计**思想，将系统分为数据层、逻辑层和交互层三个层次：

- **数据层**：负责图结构的存储与管理，采用邻接矩阵表示景点间的连通关系
- **逻辑层**：实现图的核心算法，包括DFS遍历、回路检测、Dijkstra最短路径等
- **交互层**：提供用户界面，处理用户输入和输出，实现权限区分

### 2.2 系统分层框架

```
┌─────────────────────────────────────────────────────────────┐
│                      交互层 (main.c)                        │
│  ┌──────────────────┐    ┌──────────────────┐              │
│  │    游客菜单       │    │    管理方菜单     │              │
│  │  - 查看分布图    │    │  - 添加景点      │              │
│  │  - DFS导游路线   │    │  - 添加路径      │              │
│  │  - 回路检测      │    │  - 修改路径      │              │
│  │  - 最短路径查询  │    │  - 查询功能      │              │
│  └──────────────────┘    └──────────────────┘              │
├─────────────────────────────────────────────────────────────┤
│                      逻辑层 (graph.c)                        │
│  ┌──────────────────┐    ┌──────────────────┐              │
│  │    图操作模块    │    │    算法模块       │              │
│  │  - InitGraph     │    │  - DFS遍历       │              │
│  │  - AddVertex     │    │  - 回路检测      │              │
│  │  - AddEdge       │    │  - Dijkstra      │              │
│  │  - PrintMatrix   │    │                  │              │
│  └──────────────────┘    └──────────────────┘              │
├─────────────────────────────────────────────────────────────┤
│                      数据层 (graph.h)                        │
│  ┌───────────────────────────────────────────────────────┐  │
│  │                    Graph结构体                         │  │
│  │  - Vertex vertex[MAX_VERTEX]  // 景点数组             │  │
│  │  - int matrix[MAX_VERTEX][MAX_VERTEX]  // 邻接矩阵    │  │
│  │  - int vertex_count  // 景点数量                      │  │
│  └───────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
```

### 2.3 图邻接矩阵数据结构详细设计

**Vertex结构体**：存储单个景点信息

```c
typedef struct {
    char name[20];  // 景点名称
} Vertex;
```

**Graph结构体**：存储整个景区的图结构

```c
typedef struct {
    Vertex vertex[MAX_VERTEX];       // 景点数组，最多20个景点
    int matrix[MAX_VERTEX][MAX_VERTEX];  // 邻接矩阵，存储路径距离
    int vertex_count;                // 当前景点数量
} Graph;
```

**邻接矩阵设计说明**：

| 矩阵元素 | 含义 |
|----------|------|
| `matrix[i][i] = 0` | 景点自身到自身距离为0 |
| `matrix[i][j] = weight` | 景点i到景点j的距离为weight |
| `matrix[i][j] = INF(32767)` | 景点i和景点j之间无直接路径 |

### 2.4 五大功能模块拆分设计

**模块一：图的创建与初始化**
- 功能：初始化图结构、添加景点、添加路径
- 核心函数：`InitGraph()`、`AddVertex()`、`AddEdge()`

**模块二：邻接矩阵展示**
- 功能：打印完整的邻接矩阵
- 核心函数：`PrintMatrix()`

**模块三：DFS深度优先遍历**
- 功能：从起点出发遍历所有可达景点
- 核心函数：`DFS()`、`DFSHelper()`

**模块四：回路检测**
- 功能：检测遍历路径中是否存在重复访问的景点
- 核心函数：`DetectCycle()`

**模块五：Dijkstra最短路径**
- 功能：计算两点间的最短路径及距离
- 核心函数：`Dijkstra()`

### 2.5 技术决策对比分析

#### 2.5.1 图存储结构选择

| 方案 | 原理 | 生活类比 | 优缺点 |
|------|------|----------|--------|
| **邻接矩阵** | 用二维数组存储，`matrix[i][j]`表示景点i到景点j的距离 | **班级座位表**：一张表格，行和列都是同学，表格里填的是两人之间的距离（如隔几排） | ✅ 优点：查询两点是否连通、获取距离速度快（O(1)），结构简单直观<br>❌ 缺点：浪费空间，当景点很多但路径很少时，大部分格子都是"无穷大" |
| **邻接表** | 用链表存储，每个景点对应一个链表，记录它能到达的其他景点和距离 | **通讯录分组**：每个人有一个电话本，只记录自己认识的人的联系方式 | ✅ 优点：节省空间，适合稀疏图（路径少的情况）<br>❌ 缺点：查询两点是否连通需要遍历链表（O(n)），实现稍微复杂 |
| **边集数组** | 用数组存储所有边，每条边包含起点、终点、距离 | **快递路线单**：一张单子上列出所有快递路线，每条路线写清楚从哪到哪、距离多远 | ✅ 优点：适合需要频繁添加/删除边的场景<br>❌ 缺点：查询效率低，不适合路径规划算法 |

**推荐方案：邻接矩阵**

**推荐理由**：
1. 题目明确要求使用邻接矩阵存储图结构
2. 景区路径规划场景中，景点数量有限（示例只有8个），空间浪费可以忽略
3. Dijkstra算法和DFS遍历在邻接矩阵上实现更直观
4. 邻接矩阵的打印展示效果更好，便于检查和调试

#### 2.5.2 DFS遍历策略选择

| 方案 | 原理 | 生活类比 | 优缺点 |
|------|------|----------|--------|
| **递归DFS** | 利用函数调用栈，访问一个景点后立即深入访问其未访问的邻居 | **迷宫探险**：走到一个路口，随便选一条路一直走到头，走不通了再退回来选另一条 | ✅ 优点：代码简洁易理解，符合直觉<br>❌ 缺点：景点数量过多时可能栈溢出（本题只有8个景点，完全没问题） |
| **迭代DFS** | 手动维护一个栈数据结构，模拟递归过程 | **探险队记录**：用本子记录走过的路线，每次选一条新路，走不通就按本子记录返回 | ✅ 优点：避免栈溢出，更安全<br>❌ 缺点：代码稍微复杂，不如递归直观 |
| **回溯DFS** | 在递归基础上增加路径记录，到达终点后回溯尝试其他路径 | **寻宝游戏**：记录走过的每一步，找到宝藏后返回起点，尝试另一条路看是否有更好的宝藏 | ✅ 优点：可以记录完整路径，方便后续回路检测<br>❌ 缺点：需要额外的路径数组存储 |

**推荐方案：回溯DFS**

**推荐理由**：
1. 题目要求输出"完整游览导游路线"，需要记录经过的路径
2. 回溯DFS天然支持路径记录，方便后续的回路检测功能
3. 8个景点规模很小，递归深度最大为8，不会栈溢出
4. 代码实现直观，便于理解和调试

#### 2.5.3 回路检测方法选择

| 方案 | 原理 | 生活类比 | 优缺点 |
|------|------|----------|--------|
| **访问标记数组** | 在遍历过程中标记已访问的节点，遇到已标记的节点即为回路 | **参观博物馆盖章**：每个展厅门口盖章，发现某个展厅已经盖过章，说明走回头路了 | ✅ 优点：实现简单，遍历过程中就能检测<br>❌ 缺点：只能检测是否存在回路，无法输出具体回路段落 |
| **路径记录比对** | 记录完整路径，遍历完成后检查路径中是否有重复节点 | **查火车票**：检查整个行程的车票，看有没有重复到达同一个城市的记录 | ✅ 优点：可以输出完整的回路段落<br>❌ 缺点：需要存储完整路径，稍费空间 |
| **着色法** | 用三种颜色标记节点状态（白色=未访问，灰色=访问中，黑色=已完成），遇到灰色节点即为回路 | **交通信号灯**：绿灯=没去过，黄灯=正在去，红灯=已经去过；遇到黄灯说明在绕圈子 | ✅ 优点：可以检测出具体的回路段落<br>❌ 缺点：实现稍复杂 |

**推荐方案：路径记录比对**

**推荐理由**：
1. 题目要求输出"完整回路段落"，路径记录比对可以满足这个需求
2. 与回溯DFS配合，路径数组已经存在，无需额外空间
3. 实现直观，符合大学生作业水平

#### 2.5.4 最短路径算法选择

| 方案 | 原理 | 生活类比 | 优缺点 |
|------|------|----------|--------|
| **Dijkstra算法** | 贪心策略，每次选择距离起点最近的未访问节点进行松弛操作 | **找最近的加油站**：每次选最近的加油站加油，然后继续前进 | ✅ 优点：单源最短路径，时间复杂度O(n²)，适合稠密图<br>❌ 缺点：不能处理负权边 |
| **Floyd-Warshall算法** | 动态规划，通过中间节点逐步更新任意两点间的最短路径 | **找最优路线组合**：尝试经过每一个可能的中转站，找到最优路线 | ✅ 优点：全源最短路径，代码简洁<br>❌ 缺点：时间复杂度O(n³)，效率较低 |
| **Bellman-Ford算法** | 松弛所有边n-1次，可检测负权环 | **多次试走**：把每条路都走n-1次，总能找到最短路径 | ✅ 优点：能处理负权边，可检测负权环<br>❌ 缺点：时间复杂度O(nm)，效率较低 |

**推荐方案：Dijkstra算法**

**推荐理由**：
1. 题目明确要求使用Dijkstra算法
2. 景区路径距离都是非负数，不存在负权边
3. Dijkstra算法在邻接矩阵上实现直观，时间复杂度O(n²)对于8个景点完全足够
4. 单源最短路径符合用户"输入起点、终点"的查询需求

#### 2.5.5 权限管理设计选择

| 方案 | 原理 | 生活类比 | 优缺点 |
|------|------|----------|--------|
| **菜单区分** | 根据用户角色显示不同的菜单选项 | **景区门票**：普通门票只能进常规景点，VIP门票可以进所有区域 | ✅ 优点：实现简单，用户体验直观<br>❌ 缺点：权限控制不够细粒度 |
| **功能权限表** | 使用权限矩阵控制每个用户角色对每个功能的访问权限 | **公司门禁卡**：每张卡有不同的开门权限，系统检查权限表决定是否允许进入 | ✅ 优点：权限控制精细，扩展性强<br>❌ 缺点：实现复杂，需要额外的数据结构 |
| **角色继承** | 定义基础角色，高级角色继承基础角色的所有权限 | **会员等级**：普通会员有基础权限，VIP会员在普通会员基础上增加更多权限 | ✅ 优点：权限管理灵活，易于扩展<br>❌ 缺点：实现稍复杂 |

**推荐方案：菜单区分**

**推荐理由**：
1. 系统只有两个角色（游客、管理方），菜单区分足够
2. 实现简单，符合大学生作业水平
3. 用户界面清晰，游客和管理方看到不同的菜单

#### 2.5.6 扩展接口设计选择

| 方案 | 原理 | 生活类比 | 优缺点 |
|------|------|----------|--------|
| **预留菜单入口** | 在菜单中预留扩展功能区域，用户可选择进入 | **商场预留铺位**：商场预留一些铺位，未来可以出租给新商家 | ✅ 优点：实现简单，用户可直接访问扩展功能<br>❌ 缺点：扩展功能需要修改主程序代码 |
| **插件机制** | 设计插件接口，通过动态加载实现功能扩展 | **手机应用商店**：通过安装APP扩展手机功能，无需修改系统 | ✅ 优点：扩展性强，无需修改主程序<br>❌ 缺点：实现复杂，C语言支持有限 |
| **配置文件** | 通过配置文件定义扩展功能，程序读取配置动态加载 | **电视节目单**：通过节目单决定播放什么节目，无需修改电视硬件 | ✅ 优点：灵活性高，无需重新编译<br>❌ 缺点：实现复杂，需要解析配置文件 |

**推荐方案：预留菜单入口**

**推荐理由**：
1. 实现简单，符合大学生作业水平
2. 对于期末作业来说，预留接口即可满足要求
3. 扩展功能可以直接在预留区域添加，无需复杂的插件机制

---

## 三、项目实现过程

### 3.1 C语言工程创建流程

**文件分工说明**：

| 文件名称 | 文件类型 | 职责描述 |
|----------|----------|----------|
| `graph.h` | 头文件 | 定义Graph、Vertex结构体，声明所有函数接口 |
| `graph.c` | 源文件 | 实现图的操作函数和算法函数 |
| `main.c` | 源文件 | 主程序入口，实现菜单界面和权限控制 |

**工程创建步骤**：

1. 创建项目目录 `scenic_route_project`
2. 创建头文件 `graph.h`，定义数据结构和函数声明
3. 创建源文件 `graph.c`，实现图操作和算法
4. 创建源文件 `main.c`，实现用户交互界面
5. 使用GCC编译：`gcc main.c graph.c -o scenic_route.exe`

### 3.2 邻接矩阵数据结构完整代码实现

**头文件 graph.h**：

```c
#ifndef GRAPH_H
#define GRAPH_H

#define MAX_VERTEX 20   // 最大景点数量
#define INF 32767       // 无穷大，表示无路径
#define VISITED 1       // 已访问标记
#define UNVISITED 0     // 未访问标记

// 景点结构体
typedef struct {
    char name[20];  // 景点名称
} Vertex;

// 图结构体
typedef struct {
    Vertex vertex[MAX_VERTEX];       // 景点数组
    int matrix[MAX_VERTEX][MAX_VERTEX];  // 邻接矩阵
    int vertex_count;                // 当前景点数量
} Graph;

// 函数声明
void InitGraph(Graph *g);           // 初始化图
void AddVertex(Graph *g, char *name);  // 添加景点
void AddEdge(Graph *g, int from, int to, int weight);  // 添加路径
void PrintMatrix(Graph *g);         // 打印邻接矩阵
void DFS(Graph *g, int start);      // DFS遍历
void DetectCycle(Graph *g);          // 回路检测
void Dijkstra(Graph *g, int start, int end);  // Dijkstra最短路径
void LoadSampleData(Graph *g);      // 加载示例数据

#endif
```

### 3.3 五大核心功能函数逐段代码+注释讲解

#### 3.3.1 创建图 - InitGraph()

```c
void InitGraph(Graph *g) {
    int i, j;
    g->vertex_count = 0;  // 初始化景点数量为0
    // 将邻接矩阵所有元素设为INF（无路径）
    for (i = 0; i < MAX_VERTEX; i++) {
        for (j = 0; j < MAX_VERTEX; j++) {
            g->matrix[i][j] = INF;
        }
        g->matrix[i][i] = 0;  // 自身到自身距离为0
    }
}
```

**原理**：初始化时将邻接矩阵所有元素设为无穷大，表示初始状态下所有景点之间都没有路径。对角线元素设为0，表示景点到自身的距离为0。

#### 3.3.2 添加景点 - AddVertex()

```c
void AddVertex(Graph *g, char *name) {
    strcpy(g->vertex[g->vertex_count].name, name);  // 复制景点名称
    g->vertex_count++;  // 景点数量加1
}
```

**原理**：将景点名称存储到顶点数组中，同时更新景点计数器。

#### 3.3.3 添加路径 - AddEdge()

```c
void AddEdge(Graph *g, int from, int to, int weight) {
    g->matrix[from][to] = weight;  // 设置从from到to的距离
    g->matrix[to][from] = weight;  // 设置从to到from的距离（无向图）
}
```

**原理**：景区路径是双向的（无向图），因此需要同时设置两个方向的距离。

#### 3.3.4 打印矩阵 - PrintMatrix()

```c
void PrintMatrix(Graph *g) {
    int i, j;
    printf("邻接矩阵分布图:\n");
    printf("%-12s", "景点");  // 打印表头
    for (i = 0; i < g->vertex_count; i++) {
        printf("%-12s", g->vertex[i].name);  // 打印所有景点名称
    }
    printf("\n");
    // 按行打印邻接矩阵
    for (i = 0; i < g->vertex_count; i++) {
        printf("%-12s", g->vertex[i].name);  // 行名称
        for (j = 0; j < g->vertex_count; j++) {
            if (g->matrix[i][j] == INF) {
                printf("%-12d", INF);  // 无路径显示INF
            } else {
                printf("%-12d", g->matrix[i][j]);  // 有路径显示距离
            }
        }
        printf("\n");
    }
}
```

**原理**：逐行逐列打印邻接矩阵，第一行和第一列显示景点名称，矩阵元素显示路径距离。

#### 3.3.5 DFS深度优先遍历 - DFS() & DFSHelper()

```c
static int visited[MAX_VERTEX];  // 访问标记数组
static int path[MAX_VERTEX];     // 路径记录数组
static int path_len;             // 当前路径长度

void DFSHelper(Graph *g, int current) {
    int i;
    visited[current] = VISITED;  // 标记当前节点已访问
    path[path_len++] = current;  // 将当前节点加入路径
    // 遍历所有邻接节点
    for (i = 0; i < g->vertex_count; i++) {
        // 如果存在路径且未访问，则递归访问
        if (g->matrix[current][i] != INF && 
            g->matrix[current][i] != 0 && 
            visited[i] == UNVISITED) {
            DFSHelper(g, i);
        }
    }
}

void DFS(Graph *g, int start) {
    int i;
    // 初始化访问标记数组
    for (i = 0; i < MAX_VERTEX; i++) {
        visited[i] = UNVISITED;
    }
    path_len = 0;  // 初始化路径长度
    DFSHelper(g, start);  // 从起点开始DFS
    // 输出遍历路径
    printf("DFS导游路线: ");
    for (i = 0; i < path_len; i++) {
        printf("%s", g->vertex[path[i]].name);
        if (i < path_len - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
}
```

**原理**：DFS采用递归方式，访问一个节点后立即深入访问其未访问的邻接节点，直到无法继续深入，然后回溯到上一个节点尝试其他分支。

#### 3.3.6 回路检测 - DetectCycle()

```c
void DetectCycle(Graph *g) {
    int i, j;
    // 双重循环检查路径中是否有重复节点
    for (i = 0; i < path_len; i++) {
        for (j = i + 1; j < path_len; j++) {
            if (path[i] == path[j]) {
                // 发现回路，输出回路段落
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
```

**原理**：遍历DFS生成的路径（存储在静态变量path数组中），检查是否存在重复访问的景点。如果存在相同的景点索引，则说明存在回路。使用静态变量path和path_len，避免了跨文件传递数组参数的问题。

#### 3.3.7 Dijkstra最短路径 - Dijkstra()

```c
void Dijkstra(Graph *g, int start, int end) {
    int dist[MAX_VERTEX], prev[MAX_VERTEX], visited_dijk[MAX_VERTEX];
    int i, j, min_dist, u;
    
    // 初始化距离数组、前驱数组、访问标记数组
    for (i = 0; i < g->vertex_count; i++) {
        dist[i] = g->matrix[start][i];  // 初始距离为起点到各节点的直接距离
        if (dist[i] != INF && i != start) {
            prev[i] = start;            // 直连节点的前驱设为起点
        } else {
            prev[i] = -1;               // 其他节点前驱初始化为-1
        }
        visited_dijk[i] = UNVISITED;    // 初始未访问
    }
    visited_dijk[start] = VISITED;  // 起点标记为已访问
    
    // Dijkstra算法主循环
    for (i = 1; i < g->vertex_count; i++) {
        // 找到未访问节点中距离最小的节点u
        min_dist = INF;
        u = -1;
        for (j = 0; j < g->vertex_count; j++) {
            if (visited_dijk[j] == UNVISITED && dist[j] < min_dist) {
                min_dist = dist[j];
                u = j;
            }
        }
        if (u == -1) break;  // 所有可达节点已处理
        visited_dijk[u] = VISITED;  // 标记u为已访问
        
        // 松弛操作：更新通过u到达其他节点的最短距离
        for (j = 0; j < g->vertex_count; j++) {
            if (visited_dijk[j] == UNVISITED && 
                g->matrix[u][j] != INF && 
                dist[u] + g->matrix[u][j] < dist[j]) {
                dist[j] = dist[u] + g->matrix[u][j];
                prev[j] = u;  // 更新前驱节点
            }
        }
    }
    
    // 判断是否可达
    if (dist[end] == INF) {
        printf("无法从%s到达%s\n", g->vertex[start].name, g->vertex[end].name);
        return;
    }
    
    // 回溯重建路径
    int path_result[MAX_VERTEX], path_index = 0;
    int temp = end;
    while (temp != -1) {
        path_result[path_index++] = temp;
        temp = prev[temp];
    }
    
    // 输出路径（逆序）
    printf("最短路径: ");
    for (i = path_index - 1; i >= 0; i--) {
        printf("%s", g->vertex[path_result[i]].name);
        if (i > 0) {
            printf(" -> ");
        }
    }
    printf("\n最短距离: %d\n", dist[end]);
}
```

**原理**：Dijkstra算法采用贪心策略，每次选择距离起点最近的未访问节点，然后更新通过该节点到达其他节点的最短距离，直到所有节点都被处理。

### 3.4 Git版本控制完整操作流程命令

```bash
# 1. 初始化Git仓库
git init

# 2. 添加所有文件
git add .

# 3. 第一次提交
git commit -m "Initial commit: 景区路径规划系统基础框架"

# 4. 创建功能分支
git branch feature/graph-structure
git checkout feature/graph-structure

# 5. 实现图结构后提交
git add graph.h graph.c
git commit -m "Implement: 图邻接矩阵数据结构"

# 6. 创建DFS分支
git checkout -b feature/dfs-traversal

# 7. 实现DFS遍历后提交
git add graph.c
git commit -m "Implement: DFS深度优先遍历功能"

# 8. 合并分支到主分支
git checkout main
git merge feature/graph-structure
git merge feature/dfs-traversal

# 9. 推送到远程仓库
git remote add origin https://github.com/yourusername/scenic-route.git
git push -u origin main

# 10. 创建Dijkstra分支
git checkout -b feature/dijkstra

# 11. 实现最短路径后提交
git add graph.c
git commit -m "Implement: Dijkstra最短路径算法"

# 12. 创建权限管理分支
git checkout -b feature/permission

# 13. 实现权限控制后提交
git add main.c
git commit -m "Implement: 双角色权限区分功能"

# 14. 合并所有分支
git checkout main
git merge feature/dijkstra
git merge feature/permission
git push origin main

# 15. 查看提交历史
git log --oneline
```

### 3.5 Trae编辑器开发交互操作步骤说明

**步骤1：创建项目**
- 打开Trae编辑器
- 点击"新建项目"，选择C语言项目模板
- 输入项目名称"scenic_route_project"

**步骤2：创建文件**
- 在项目目录中创建`graph.h`头文件
- 创建`graph.c`源文件
- 创建`main.c`源文件

**步骤3：编写代码**
- 在`graph.h`中定义结构体和函数声明
- 在`graph.c`中实现图操作和算法函数
- 在`main.c`中实现菜单界面和权限控制

**步骤4：编译运行**
- 打开终端，输入编译命令：`gcc main.c graph.c -o scenic_route.exe`
- 运行程序：`./scenic_route.exe`

**步骤5：调试测试**
- 使用Trae的调试功能设置断点
- 逐步执行代码，观察变量值变化
- 验证各功能模块的正确性

---

## 四、项目运行效果

### 4.1 内置示例测试数据

**8个景点**：
1. 大门 (索引0)
2. 碧玉潭 (索引1)
3. 红叶林 (索引2)
4. 赏花园 (索引3)
5. 飞云石 (索引4)
6. 九曲桥 (索引5)
7. 白虎岩 (索引6)
8. 观月阁 (索引7)

**9条连通边**：
| 起点 | 终点 | 距离 |
|------|------|------|
| 大门 | 碧玉潭 | 50 |
| 大门 | 红叶林 | 80 |
| 碧玉潭 | 赏花园 | 60 |
| 碧玉潭 | 飞云石 | 40 |
| 红叶林 | 九曲桥 | 70 |
| 赏花园 | 白虎岩 | 90 |
| 飞云石 | 白虎岩 | 30 |
| 九曲桥 | 观月阁 | 50 |
| 白虎岩 | 观月阁 | 45 |

### 4.2 分模块运行效果说明

**模块一：展示邻接矩阵分布图**

```
邻接矩阵分布图:
景点         大门         碧玉潭       红叶林       赏花园       飞云石       九曲桥       白虎岩       观月阁
大门         0           50          80          32767       32767       32767       32767       32767
碧玉潭       50          0           32767       60          40          32767       32767       32767
红叶林       80          32767       0           32767       32767       70          32767       32767
赏花园       32767       60          32767       0           32767       32767       90          32767
飞云石       32767       40          32767       32767       0           32767       30          32767
九曲桥       32767       32767       70          32767       32767       0           32767       50
白虎岩       32767       32767       32767       90          30          32767       0           45
观月阁       32767       32767       32767       32767       32767       50          45          0
```

**模块二：DFS深度优先遍历导游路线**

```
输入: 大门
输出: DFS导游路线: 大门 -> 碧玉潭 -> 赏花园 -> 白虎岩 -> 飞云石 -> 观月阁 -> 九曲桥 -> 红叶林
```

**模块三：回路检测功能**

```
输入: 大门
输出: DFS导游路线: 大门 -> 碧玉潭 -> 赏花园 -> 白虎岩 -> 飞云石 -> 观月阁 -> 九曲桥 -> 红叶林
      未检测到回路
```

**模块四：Dijkstra最短路径算法**

```
输入: 起点=大门, 终点=观月阁
输出: 最短路径: 大门 -> 碧玉潭 -> 飞云石 -> 白虎岩 -> 观月阁
      最短距离: 165
```

**模块五：双角色权限区分**

```
游客菜单:
1. 查看景点分布图
2. DFS导游路线
3. 回路检测
4. 最短路径查询
5. 扩展功能

管理方菜单:
1. 查看景点分布图
2. DFS导游路线
3. 回路检测
4. 最短路径查询
5. 添加景点
6. 添加路径
7. 修改路径距离
8. 扩展功能
```

### 4.3 测试结果解析

**邻接矩阵测试**：矩阵正确反映了景点间的连通关系，对角线为0，无路径位置为32767，有路径位置为实际距离。

**DFS遍历测试**：从大门出发，DFS正确遍历了所有8个景点，路线完整且无遗漏。

**回路检测测试**：测试路线中无重复访问的景点，回路检测正确返回"未检测到回路"。

**Dijkstra测试**：计算出的最短路径为165，路径为"大门→碧玉潭→飞云石→白虎岩→观月阁"，验证正确。

**权限测试**：游客只能查询，管理方可进行维护操作，权限区分正确。

---

## 五、项目总结

### 5.1 项目完成情况

本项目已完成全部硬性功能要求：

- ✅ 创建景区景点分布图，采用邻接矩阵存储
- ✅ 展示邻接矩阵分布图，无连通填32767，自身距离为0
- ✅ DFS深度优先遍历导游路线，输入起点遍历全部可达景点
- ✅ 回路检测功能，查找重复访问景点并输出回路段落
- ✅ Dijkstra最短路径算法，输出途经景点序列和最短距离
- ✅ 双角色权限区分，游客仅查询，管理方可维护
- ✅ 预留自定义扩展功能接口

### 5.2 开发难点

1. **邻接矩阵初始化**：需要正确处理对角线元素和无穷大的表示
2. **DFS递归边界**：需要正确设置递归终止条件，避免死循环
3. **Dijkstra路径重建**：需要通过前驱数组正确回溯路径，注意路径顺序
4. **权限控制设计**：需要设计合理的菜单结构，区分不同角色的操作权限

### 5.3 收获

1. 掌握了**图的邻接矩阵存储结构**及其基本操作
2. 深入理解了**DFS深度优先遍历算法**的原理和实现
3. 掌握了**Dijkstra最短路径算法**的贪心策略和实现细节
4. 学会了**模块化编程**思想，将大型程序拆分为多个模块
5. 理解了**权限管理**的基本设计思路

### 5.4 优化改进思路

1. **优化空间复杂度**：对于稀疏图，可以考虑使用邻接表存储，减少空间浪费
2. **增加Floyd-Warshall算法**：实现全源最短路径查询，支持任意两点间的最短路径
3. **添加路径可视化**：使用图形化界面展示景区地图和路径
4. **增加景点信息管理**：存储景点的详细信息（描述、图片、开放时间等）
5. **实现动态图结构**：支持删除景点和路径的功能
6. **添加文件读写功能**：将图数据保存到文件，实现数据持久化

---

## 六、附录

### 6.1 参考文献

[1] 严蔚敏, 吴伟民. 数据结构(C语言版)[M]. 清华大学出版社, 2011.

[2] 谭浩强. C程序设计(第四版)[M]. 清华大学出版社, 2010.

[3] 屈婉玲, 刘田, 张立昂. 算法设计与分析[M]. 清华大学出版社, 2015.

[4] 陈越, 何钦铭. 数据结构学习与实验指导[M]. 高等教育出版社, 2012.

### 6.2 完整全部源代码

**graph.h**：

```c
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
```

**graph.c**：

```c
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
```

**main.c**：

```c
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
```

---

**编译运行说明**：

```bash
# 编译命令
gcc main.c graph.c -o scenic_route.exe

# 运行命令
./scenic_route.exe
```

---

**项目文件清单**：

| 文件 | 说明 |
|------|------|
| `graph.h` | 头文件，定义数据结构和函数声明 |
| `graph.c` | 源文件，实现图操作和算法 |
| `main.c` | 源文件，主程序入口和用户界面 |
| `report.md` | 期末作业报告 |
