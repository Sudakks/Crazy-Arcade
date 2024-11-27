#ifndef MAP_H
#define MAP_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

class map
{
public:
    map();
    int get_map(int x, int y);//得到地图类型
    void init_Map();//初始化二维数组
    void set_map(int x, int y, int content);
private:
    int Map[15][20]={0};
};

#endif // MAP_H
