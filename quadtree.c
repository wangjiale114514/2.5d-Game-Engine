//四叉树空间索引优化
#include <stdlib.h>
#include "map.h"

#define MAX_MAPS_PER_NODE 4  //每个节点最多包含的地图格子数
#define MAX_DEPTH 8          //最大递归深度

typedef struct QuadTreeNode {
    float x, y, w, h;        //节点边界
    int map_indices[MAX_MAPS_PER_NODE];  //节点包含的地图格子索引
    int map_count;           //当前包含的格子数
    struct QuadTreeNode *nw, *ne, *sw, *se;  //四个子节点
    int depth;               //当前深度
} QuadTreeNode;

QuadTreeNode *root = NULL;

//函数声明
static void quadtree_cleanup_recursive(QuadTreeNode *node);
extern void quadtree_cleanup();

//创建新节点
QuadTreeNode* create_node(float x, float y, float w, float h, int depth) {
    QuadTreeNode *node = (QuadTreeNode*)malloc(sizeof(QuadTreeNode));
    node->x = x;
    node->y = y;
    node->w = w;
    node->h = h;
    node->map_count = 0;
    node->nw = node->ne = node->sw = node->se = NULL;
    node->depth = depth;
    return node;
}

//判断两个矩形是否相交
int rectangles_intersect(float x1, float y1, float w1, float h1, 
                         float x2, float y2, float w2, float h2) {
    return !(w1 < x2 || w2 < x1 || h1 < y2 || h2 < y1);
}

//向节点插入地图格子
void insert_to_node(QuadTreeNode *node, int map_idx, float mx, float my, float mw, float mh) {
    //检查格子是否与节点相交
    if (!rectangles_intersect(node->x, node->y, node->w, node->h, mx, my, mw, mh)) {
        return;
    }
    
    //如果是叶子节点且未满，直接添加
    if (node->nw == NULL && node->map_count < MAX_MAPS_PER_NODE) {
        node->map_indices[node->map_count++] = map_idx;
        return;
    }
    
    //需要分裂或已有子节点
    if (node->nw == NULL) {
        //分裂节点
        float half_w = (node->w - node->x) / 2.0f;
        float half_h = (node->h - node->y) / 2.0f;
        float mid_x = node->x + half_w;
        float mid_y = node->y + half_h;
        
        if (node->depth < MAX_DEPTH) {
            node->nw = create_node(node->x, node->y, mid_x, mid_y, node->depth + 1);
            node->ne = create_node(mid_x, node->y, node->w, mid_y, node->depth + 1);
            node->sw = create_node(node->x, mid_y, mid_x, node->h, node->depth + 1);
            node->se = create_node(mid_x, mid_y, node->w, node->h, node->depth + 1);
            
            //将现有的格子分配到子节点
            for (int i = 0; i < node->map_count; i++) {
                int idx = node->map_indices[i];
                insert_to_node(node->nw, idx, dim[idx].x, dim[idx].y, dim[idx].w, dim[idx].h);
                insert_to_node(node->ne, idx, dim[idx].x, dim[idx].y, dim[idx].w, dim[idx].h);
                insert_to_node(node->sw, idx, dim[idx].x, dim[idx].y, dim[idx].w, dim[idx].h);
                insert_to_node(node->se, idx, dim[idx].x, dim[idx].y, dim[idx].w, dim[idx].h);
            }
            node->map_count = 0;
        }
    }
    
    //插入到子节点
    if (node->nw) {
        insert_to_node(node->nw, map_idx, mx, my, mw, mh);
        insert_to_node(node->ne, map_idx, mx, my, mw, mh);
        insert_to_node(node->sw, map_idx, mx, my, mw, mh);
        insert_to_node(node->se, map_idx, mx, my, mw, mh);
    }
}

//初始化四叉树
void quadtree_init(float min_x, float min_y, float max_x, float max_y) {
    if (root) quadtree_cleanup();
    root = create_node(min_x, min_y, max_x, max_y, 0);
}

//构建四叉树（调用此函数前需要先初始化地图数据）
void quadtree_build() {
    if (!root) {
        quadtree_init(0, 0, 100, 100);  //默认范围，可根据实际地图调整
    }
    for (int i = 0; i < MAP_math; i++) {
        insert_to_node(root, i, dim[i].x, dim[i].y, dim[i].w, dim[i].h);
    }
}

//查询坐标所在的地图格子索引，返回-1表示未找到
int quadtree_query(float x, float y) {
    if (!root) return -1;
    
    QuadTreeNode *node = root;
    while (node) {
        //检查当前节点的直接格子
        for (int i = 0; i < node->map_count; i++) {
            int idx = node->map_indices[i];
            if (dim[idx].x <= x && x <= dim[idx].w && dim[idx].y <= y && y <= dim[idx].h) {
                return idx;
            }
        }
        
        //如果没有子节点，返回未找到
        if (!node->nw) return -1;
        
        //确定进入哪个子节点
        float mid_x = node->x + (node->w - node->x) / 2.0f;
        float mid_y = node->y + (node->h - node->y) / 2.0f;
        
        if (x < mid_x) {
            node = (y < mid_y) ? node->nw : node->sw;
        } else {
            node = (y < mid_y) ? node->ne : node->se;
        }
    }
    return -1;
}

//释放四叉树内存
void quadtree_cleanup_recursive(QuadTreeNode *node) {
    if (!node) return;
    quadtree_cleanup_recursive(node->nw);
    quadtree_cleanup_recursive(node->ne);
    quadtree_cleanup_recursive(node->sw);
    quadtree_cleanup_recursive(node->se);
    free(node);
}

void quadtree_cleanup() {
    quadtree_cleanup_recursive(root);
    root = NULL;
}
