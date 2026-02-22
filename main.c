//主函数(程序入口)
#include "map.c"        //地图初始化
#include "player.c"      //玩家控制
#include "draw_wall.c"   //3D渲染

int main() {    //主函数
    Init();    //初始化窗口和缓冲区
    init_time();    //初始化时间系统

    map_init();

    //主循环：持续运行直到窗口关闭
    while (!IsClosed()) {   //IsClosed返回0表示窗口还开着
        Clear();   //清空屏幕(填充黑色)

        //获取帧时间增量
        float delta_time = get_delta_time();

        //键盘输入处理(检测按键是否按下)
        if (GetAsyncKeyState(0x57) & 0x8000) move_forward(delta_time);  //W键：向前移动
        if (GetAsyncKeyState(0x53) & 0x8000) move_backward(delta_time); //S键：向后移动
        if (GetAsyncKeyState(0x41) & 0x8000) turn_left(delta_time);      //A键：左转
        if (GetAsyncKeyState(0x44) & 0x8000) turn_right(delta_time);     //D键：右转
        //0x8000是最高位，表示按键当前处于按下状态

        //渲染3D场景
        Draw3D(player_angle, player_x, player_y);   //传入玩家角度和坐标

        //更新屏幕显示
        Update();   //把内存缓冲区复制到窗口，处理Windows消息
    }

    return 0;   //程序正常退出
}
