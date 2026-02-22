//计算坐标和面向角度(玩家控制)
#include <math.h>
#include <windows.h>

//玩家位置(浮点数，支持平滑移动)
float player_x = 2.0;       //玩家x坐标
float player_y = 2.0;       //玩家y坐标
float player_angle = 0.0;   //玩家面向角度(度数，0=右，90=下)

//移动速度(单位/秒)
#define MOVE_SPEED 3.0f
//旋转速度(度/秒)
#define TURN_SPEED 90.0f
#define PLAYER_RADIUS 0.2   //玩家碰撞半径

//上一帧时间
static float last_time = 0.0f;

//获取时间(秒)
static float get_time() {
    static LARGE_INTEGER frequency = {0};
    static LARGE_INTEGER start = {0};

    if (frequency.QuadPart == 0) {
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&start);
    }

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (float)(now.QuadPart - start.QuadPart) / frequency.QuadPart;
}

//初始化时间系统
void init_time() {
    last_time = get_time();
}

//获取增量时间(秒)
float get_delta_time() {
    float current_time = get_time();
    float delta = current_time - last_time;
    last_time = current_time;

    //限制最大帧时间，防止卡顿后突然跳跃
    if (delta > 0.1f) delta = 0.1f;
    return delta;
}

//检查玩家是否碰到墙（房间边界）
int check_collision(float x, float y) {
    for (int i = 0; i < MAP_math; i++) {
        float room_x = dim[i].x;
        float room_y = dim[i].y;
        float room_w = dim[i].w;
        float room_h = dim[i].h;

        //检查玩家是否在这个房间内（边界处允许重叠，方便扇区切换）
        if (x >= room_x - PLAYER_RADIUS && x <= room_w + PLAYER_RADIUS &&
            y >= room_y - PLAYER_RADIUS && y <= room_h + PLAYER_RADIUS) {
            return 0;  //在房间内，没有碰到墙
        }
    }
    return 1;  //碰到墙了
}

//向前移动(增量移动)
void move_forward(float delta_time) {
    //把角度转成弧度，用于计算x和y分量
    float rad = player_angle * M_PI / 180.0;

    //计算移动距离：速度 * 时间增量
    float move_dist = MOVE_SPEED * delta_time;

    //计算新位置：沿当前角度方向移动
    float new_x = player_x + cos(rad) * move_dist;   //x增量
    float new_y = player_y + sin(rad) * move_dist;   //y增量

    //碰撞检测：检查新位置是否碰到墙
    if (!check_collision(new_x, new_y)) {
        player_x = new_x;   //更新玩家x坐标
        player_y = new_y;   //更新玩家y坐标
    }
}

//向后移动(增量移动)
void move_backward(float delta_time) {
    //把角度转成弧度
    float rad = player_angle * M_PI / 180.0;

    //计算移动距离：速度 * 时间增量
    float move_dist = MOVE_SPEED * delta_time;

    //计算新位置：沿当前角度反方向移动
    float new_x = player_x - cos(rad) * move_dist;   //x反向增量
    float new_y = player_y - sin(rad) * move_dist;   //y反向增量

    //碰撞检测：检查新位置是否碰到墙
    if (!check_collision(new_x, new_y)) {
        player_x = new_x;   //更新玩家坐标
        player_y = new_y;
    }
}

//左转(增量转动)
void turn_left(float delta_time) {
    //角度增量：旋转速度 * 时间增量
    player_angle -= TURN_SPEED * delta_time;

    //角度处理，保持在0-360范围内
    while (player_angle < 0) {
        player_angle += 360;
    }
    while (player_angle >= 360) {
        player_angle -= 360;
    }
}

//右转(增量转动)
void turn_right(float delta_time) {
    //角度增量：旋转速度 * 时间增量
    player_angle += TURN_SPEED * delta_time;

    //角度处理，保持在0-360范围内
    while (player_angle < 0) {
        player_angle += 360;
    }
    while (player_angle >= 360) {
        player_angle -= 360;
    }
}