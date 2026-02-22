//四叉树头文件
#ifndef QUADTREE_H
#define QUADTREE_H

extern void quadtree_init(float min_x, float min_y, float max_x, float max_y);
extern void quadtree_build();
extern int quadtree_query(float x, float y);
extern void quadtree_cleanup();

#endif
