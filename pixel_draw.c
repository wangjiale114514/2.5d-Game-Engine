//绘制像素
#include <windows.h>

#define WIDTH 640    //窗口宽度(像素)
#define HEIGHT 400   //窗口高度(像素)

HWND hWnd;          //窗口句柄(Windows API窗口标识)
HDC hdc, memDC;     //设备上下文(用于绘制)
HBITMAP hBitmap;    //位图句柄(屏幕缓冲区)
BYTE* pixels;       //像素数据指针(指向位图内存)
int window_closed = 0;  //窗口关闭标志(0=打开，1=关闭)

//窗口过程函数：处理Windows消息
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    //检查是否是关闭消息
    if (msg == WM_DESTROY || msg == WM_CLOSE) {
        window_closed = 1;   //设置关闭标志
        PostQuitMessage(0);  //退出消息循环
        return 0;
    }
    //其他消息交给默认处理
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

//初始化窗口和缓冲区
int Init() {
    //设置窗口类属性
    WNDCLASSW wc = {0};  //初始化窗口类结构体
    wc.lpfnWndProc = WindowProc;   //设置窗口过程函数
    wc.hInstance = GetModuleHandle(NULL);  //获取当前程序实例
    wc.lpszClassName = L"Game";   //设置窗口类名
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);   //背景黑色
    RegisterClassW(&wc);   //注册窗口类

    //创建窗口
    hWnd = CreateWindowW(L"Game", L"Raycasting", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL, NULL, GetModuleHandle(NULL), NULL);
        //窗口标题"Raycasting"，大小640x400

    //获取设备上下文
    hdc = GetDC(hWnd);          //获取屏幕DC
    memDC = CreateCompatibleDC(hdc);   //创建内存DC(用于离屏绘制)

    //创建位图(32位RGBA格式)
    BITMAPINFO bmi = {0};  //位图信息结构体
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = WIDTH;       //宽度
    bmi.bmiHeader.biHeight = -HEIGHT;     //负数表示从上到下
    bmi.bmiHeader.biPlanes = 1;        //位面数(必须是1)
    bmi.bmiHeader.biBitCount = 32;       //每像素32位
    bmi.bmiHeader.biCompression = BI_RGB;   //无压缩
    //创建DIB位图，pixels指针指向像素数据
    hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);

    SelectObject(memDC, hBitmap);   //把位图选入内存DC
    memset(pixels, 0, WIDTH * HEIGHT * 4);   //填充黑色(清空屏幕)
    ShowWindow(hWnd, SW_SHOW);   //显示窗口
    return 1;   //初始化成功
}

//绘制单个像素
void DrawPixel(int x, int y, int color) {
    //边界检查：防止越界
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;

    //计算像素在数组中的索引：y * 每行宽度 + x
    int idx = (y * WIDTH + x) * 4;   //乘以4因为是RGBA(4个字节)

    //设置像素颜色(灰度模式：R=G=B=color)
    pixels[idx] = color;         //B(蓝色)分量
    pixels[idx + 1] = color;     //G(绿色)分量
    pixels[idx + 2] = color;     //R(红色)分量
    pixels[idx + 3] = 255;       //A(透明度)分量
}

//绘制垂直线(用于墙壁)
void DrawLine(int x, int y, int h, int color) {
    int start_y = HEIGHT / 2 - h / 2;   //线的起点(屏幕中心-半高)
    int end_y = HEIGHT / 2 + h / 2;     //线的终点(屏幕中心+半高)

    //从起点到终点逐个像素绘制
    for (int py = start_y; py < end_y && py < HEIGHT; py++) {
        if (py >= 0) {   //防止y为负
            DrawPixel(x, py, color);   //绘制像素
        }
    }
}

//更新显示(把内存位图复制到屏幕)
void Update() {
    BitBlt(hdc, 0, 0, WIDTH, HEIGHT, memDC, 0, 0, SRCCOPY);   //复制到窗口

    //处理Windows消息队列(避免卡死)
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {   //取出消息
        TranslateMessage(&msg);   //转换键盘消息
        DispatchMessage(&msg);    //分发给窗口过程
    }
}

//清空屏幕(填充黑色)
void Clear() {
    memset(pixels, 0, WIDTH * HEIGHT * 4);   //把所有像素设为0(黑色)
}

//检查窗口是否已关闭
int IsClosed() {
    return window_closed;   //返回关闭标志
}
