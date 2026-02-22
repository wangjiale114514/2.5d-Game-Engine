//地图头文件
#ifndef MAP_H
#define MAP_H

#define MAP_math 10 //地图空间数量

typedef struct {
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

extern map dim[MAP_math];
extern int map_init();

#endif
