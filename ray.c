//光线判断
#include <math.h>
#include "quadtree.h"

#define MAX_DIST 100.0     //射线最大距离

//光线判断函数，使用步进法发射一条射线，返回碰到墙壁的距离

float ray(float angle, float x, float y, int map_in) {    //角度，x坐标，y坐标，所在空间
    float dish = 0;    //距离计数器
    float fx = x;      //原点坐标
    float fy = y;
    
    //计算射线方向的x和y
    float cos_in = cos(angle);
    float sin_in = sin(angle);


    //第一步步进到附近边缘
    //计算距离边缘最近的x和y
    float step_x = fabs(dim[map_in].x - x - 0.0001);    //x边缘距离 0.0001避免减整数
    float step_y = fabs(dim[map_in].y - y - 0.0001);
    if (cos_in >= 0) step_x = fabs(dim[map_in].w - x + 0.0001);  //x方向步进到下一个格子边缘
    if (sin_in >= 0) step_y = fabs(dim[map_in].h - y + 0.0001);  //y方向步进到下一个格子边缘
    //计算第一步步进到边缘的距离
    if ((cos_in == 0)||(sin_in == 0)) {    //如果射线是垂直或水平线
        if (cos_in == 0) dish = step_x;
        if (sin_in == 0) dish = step_y;
    }else{
        if (step_x/fabs(cos_in) < step_y/fabs(sin_in)) dish = fabs(step_x/cos_in);  //x边缘更近
        if (step_y/fabs(sin_in) <= step_x/fabs(cos_in)) dish = fabs(step_y/sin_in); //y边缘更近
    }
    x = x + (cos_in * dish);
    y = y + (sin_in * dish);

    
//    for (int i = 0; i < MAX_DIST; i++) {
//        //计算当前位置的x和y(步进1)
//        int map_on = quadtree_query(x, y);
//        if (map_on == -1)    //检测是否碰到墙
//        {
//            break;    //碰到墙就停止
//        }else{
//            step_x = fabs(dim[map_on].x - x - 0.0001);    //x边缘距离 0.0001避免减整数
//            step_y = fabs(dim[map_on].y - y - 0.0001);
//            if (cos_in >= 0) step_x = fabs(dim[map_on].w - x + 0.0001);  //x方向步进到下一个格子边缘
//            if (sin_in >= 0) step_y = fabs(dim[map_on].h - y + 0.0001);  //y方向步进到下一个格子边缘
//            if ((cos_in == 0)||(sin_in == 0)) {
//                if (cos_in == 0) dish += step_x;
//                if (sin_in == 0) dish += step_y;
//            }else{
//                if (step_x/fabs(cos_in) < step_y/fabs(sin_in)) dish += step_x/fabs(cos_in);  //x边缘更近
//                if (step_y/fabs(sin_in) <= step_x/fabs(cos_in)) dish += step_y/fabs(sin_in); //y边缘更近
//            }
//            x = fx + (cos_in * dish);
//            y = fy + (sin_in * dish);
//        }
//    }

//step和三角函数要用绝对值去算，防止出现负数导致计算错误

    return dish;
}
