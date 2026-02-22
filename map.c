//地图
#include <stdbool.h>
#include "quadtree.h"

#define MAP_math 10 //地图空间数量

//map 包含：
//起始坐标x，起始坐标y，终点坐标x，终点坐标y，地板高度， 天花板高度，地板纹理编号，天花板纹理编号，北墙纹理编号，南墙纹理编号，东墙纹理编号，西墙纹理编号

typedef struct {     //地图结构
    float x;         //起始坐标x
    float y;         //起始坐标y
    float w;         //终点坐标x
    float h;         //终点坐标y
    float floor;     //地板高度
    float ceil;      //天花板高度
    float floor_tex; //地板纹理编号
    float ceil_tex;  //天花板纹理编号
    float north_tex; //北墙纹理编号
    float south_tex; //南墙纹理编号
    float east_tex;  //东墙纹理编号
    float west_tex;  //西墙纹理编号
} map;

map dim [MAP_math];


int map_init() {    //初始化地图
    //扇区0：第一个房间 (0,0) 到 (10,10)
    dim[0].x = -10;
    dim[0].y = 0;
    dim[0].w = 10;
    dim[0].h = 10;
    dim[0].floor = 0;
    dim[0].ceil = 1000;
    dim[0].floor_tex = 0;
    dim[0].ceil_tex = 0;
    dim[0].north_tex = 0;
    dim[0].south_tex = 0;
    dim[0].east_tex = 0;
    dim[0].west_tex = 0;

    //扇区1：第二个房间 (10,0) 到 (20,10) - 在扇区0的右侧
    dim[1].x = 10;
    dim[1].y = 0;
    dim[1].w = 20;
    dim[1].h = 10;
    dim[1].floor = 200;
    dim[1].ceil = 800;
    dim[1].floor_tex = 0;
    dim[1].ceil_tex = 0;
    dim[1].north_tex = 0;
    dim[1].south_tex = 0;
    dim[1].east_tex = 0;
    dim[1].west_tex = 0;

    //扇区2：第三个房间 (0,10) 到 (10,20) - 在扇区0的下方
    dim[2].x = 0;
    dim[2].y = 10;
    dim[2].w = 10;
    dim[2].h = 20;
    dim[2].floor = 0;
    dim[2].ceil = 1000;
    dim[2].floor_tex = 0;
    dim[2].ceil_tex = 0;
    dim[2].north_tex = 0;
    dim[2].south_tex = 0;
    dim[2].east_tex = 0;
    dim[2].west_tex = 0;

    //构建四叉树加速空间查询
    quadtree_build();

    return 0;
}

