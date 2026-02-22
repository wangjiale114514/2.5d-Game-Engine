//绘制墙(3D渲染)
#include "ray.c"         //射线检测
#include "pixel_draw.c"    //像素绘制

#define FOV 60          //视野角度(Field of View，玩家能看到的角度范围)
#define player_HEIGHT 500      //身高    后续实现跳跃可能要用 (眼睛高度)

//传送门渲染逻辑
int draw_door(float x, float y, float angle, float dist, int ray_in){    //长度，角度，距离，位置,所属射线
    float rad = angle; //* M_PI / 180.0;        //角度转弧度
    float ray_x = x + cos(rad) * dist;   //x增量
    float ray_y = y + sin(rad) * dist;   //y增量
    int map_in = quadtree_query(ray_x, ray_y);    //检测传送门所在扇区
    int map_on = quadtree_query(x, y);            //本扇区高度
    if (map_in != -1){                            //如果检测到传送门
        //计算传送门后地板和天花板高度
        float wall_foolt = ((player_HEIGHT - dim[map_in].floor)/dist/2) + 200;                   //计算地板高
        float wall_ceil = 0 - ((dim[map_in].ceil - player_HEIGHT)/dist/2) + 200;    //计算天花板高

        //计算遮掩情况
        float my_dist = ray(angle, x, y, map_on);    //计算本扇区边界
        float in_wall_foolt = ((player_HEIGHT - dim[map_on].floor)/my_dist/2) + 200;    //计算本扇区高度供遮掩计算
        float in_wall_ceil = 0 - ((dim[map_on].ceil - player_HEIGHT)/my_dist/2) + 200;
        if (dim[map_in].floor < dim[map_on].floor) wall_foolt = in_wall_foolt; 
        if (dim[map_in].ceil > dim[map_on].ceil) wall_ceil = in_wall_ceil;
        //if (dim[map_in].floor < dim[map_on].floor) wall_foolt = ((player_HEIGHT - dim[map_on].floor)/dist/2) + 200;    //如果传送门后地板高度低于本区块地板高度，则不遮掩地板
        //if (dim[map_in].ceil > dim[map_on].ceil) wall_ceil = 0 - ((dim[map_on].ceil - player_HEIGHT)/dist/2) + 200;    //如果传送门后天花板高度高于本区块天花板高度，则不遮掩天花板
        //开传送门
        for (int d = wall_foolt; d >= wall_ceil; d--) {    //绘制地板和天花板
            DrawPixel(ray_in, d, 0);    //绘制黑色像素
        }

    }
    return map_in;    //返回传送门所属扇区，同时检测是否有传送门
}

float draw_door_wall(float x, float y, float angle, float dist, int ray_in){
    float rad = angle; // * M_PI / 180.0;        //角度转弧度
    float ray_x = x + cos(rad) * dist;   //x增量
    float ray_y = y + sin(rad) * dist;   //y增量
    int map_in = quadtree_query(ray_x, ray_y);    //检测传送门所在扇区
    int map_on = quadtree_query(x, y);            //本扇区编号
    if (map_in != -1){
        dist += ray(angle, ray_x, ray_y, map_in); //计算下个墙壁位置
        float WALL_HEIGHT = dim[map_in].ceil - dim[map_in].floor;    //计算墙高
        //计算墙高
        float wall_foolt = ((player_HEIGHT - dim[map_in].floor)/dist/2) + 200;      //计算地板高
        float wall_ceil = 0 - ((dim[map_in].ceil - player_HEIGHT)/dist/2) + 200;    //计算天花板高
        //计算遮掩情况
        float my_dist = ray(angle, x, y, map_on);    //计算本扇区边界
        float in_wall_foolt = ((player_HEIGHT - dim[map_on].floor)/my_dist/2) + 200;    //计算本扇区高度供遮掩计算
        float in_wall_ceil = 0 - ((dim[map_on].ceil - player_HEIGHT)/my_dist/2) + 200;
        if (in_wall_foolt < wall_foolt) wall_foolt = in_wall_foolt; //如果传送门后地板高度低于本区块地板高度，遮掩地板
        if (in_wall_ceil > wall_ceil) wall_ceil = in_wall_ceil;     //如果传送门后天花板高度高于本区块天花板高度，掩天花板

        //开传送门后面绘制的墙
        //计算迷雾效果
        int color = 255 - (int)(dist * 3);   //距离乘以3来控制渐变速度
        if (color < 30) color = 30;   //最暗不小于30(太暗看不见)
        if (color > 255) color = 255;   //最亮不大于255

        for (int d = wall_foolt; d >= wall_ceil; d--) {    //绘制地板和天花板
            DrawPixel(ray_in, d, color);    //绘制黑色像素
        }
    }

    return dist;    //返回距离
}

//绘制3D场景
void Draw3D(float player_angle, float player_x, float player_y) {
    float start_angle = player_angle - FOV / 2;   //左边界角度(玩家角度减去一半FOV)
    float step_angle = (float)FOV / WIDTH;       //每条射线对应的角度增量(60度/640像素)

    //获取玩家所属空间
    int map_in = quadtree_query(player_x, player_y);    //检测玩家所在扇区

    //int map_in = 0;    //玩家所属空间
    //for (int i = 0; i <= MAP_math; i++) {
    //    if ((dim[i].x <= player_x && player_x <= dim[i].w) && (dim[i].y <= player_y && player_y <= dim[i].h)){
    //        map_in = i;
    //        break;
    //    }
    //}

    //遍历屏幕宽度上的每一条垂直线
    for (int x = 0; x < WIDTH; x++) {
        float ray_angle = start_angle + x * step_angle;   //当前射线的角度
        float rad = ray_angle * M_PI / 180.0;        //角度转弧度

        //发射射线，获取碰到墙壁的距离
        float dist = ray(rad, player_x, player_y, map_in);           //ray函数返回距离
        float WALL_HEIGHT = dim[map_in].ceil - dim[map_in].floor;    //计算墙高

        //鱼眼矫正：边缘射线的距离会因为角度而被拉长，需要压缩
        float angle_diff = (ray_angle - player_angle) * M_PI / 180.0;   //射线与中心线的角度差
        dist = dist * cos(angle_diff);   //乘以cos值矫正，距离变小

        //距离保护：防止距离太小导致墙壁过高
        if (dist < 0.5) dist = 0.5;   //最小距离为0.5，避免墙壁占据整个屏幕

        //距离转屏幕高度：距离越近，墙越高
        float wall_foolt = ((player_HEIGHT - dim[map_in].floor)/dist/2) + 200;                   //计算地板高
        float wall_ceil = 0 - ((dim[map_in].ceil - player_HEIGHT)/dist/2) + 200;    //计算天花板高

        //距离转颜色：距离越近越亮，越远越暗(迷雾效果)
        int color = 255 - (int)(dist * 3);   //距离乘以3来控制渐变速度
        if (color < 30) color = 30;   //最暗不小于30(太暗看不见)
        if (color > 255) color = 255;   //最亮不大于255

        //绘制垂直线：在屏幕x位置画一条高为wall_height的线
        for (int y = wall_foolt; y >= wall_ceil; y--) {    //绘制地板和天花板
            DrawPixel(x, y, color);
        }

        //检测扇区
        int look = draw_door(player_x, player_y, rad, dist, x);
        if (look != -1){
            for (int j = 0; j < 20; j++){
                look = draw_door(player_x, player_y, rad, dist, x);         //检测传送门
                dist = draw_door_wall(player_x, player_y, rad, dist, x);    //绘制传送门后墙
                if (look == -1) break;    //如果没有传送门，则跳出循环
            }
        }
    }
}