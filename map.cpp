#include "map.h"
#include "mainwindow.h"
#include <QtGlobal>
#include <QTime>
#include <QDebug>
#include <QRandomGenerator>
map::map()
{

}

int map::get_map(int x, int y)
{
    return Map[x][y];
}

void map::init_Map()
{
    //用0表示空地，1表示硬墙，2表示软墙，3表示其他(中间的装饰)
    const int soft_wall = 52;
    const int hard_wall = 37;
    for(int i = 0; i <= 14; i++)
    {
        Map[i][0] = 1;
        Map[i][19] = 1;
    }
    for(int i = 0; i <= 19; i++)
    {
        Map[0][i] = 1;
        Map[14][i] = 1;
    }
    //利用随机数生成地图
    for(int i = 1; i <= soft_wall;)
    {
        quint32 x = (QRandomGenerator::global()->generate())%14;
        quint32 y = (QRandomGenerator::global()->generate())%20;
        if(!Map[x][y])
        {
            Map[x][y] = 2;
            i++;
        }
    }
    for(int i = 1; i <= hard_wall;)
    {
        quint32 x = (QRandomGenerator::global()->generate())%14;
        quint32 y = (QRandomGenerator::global()->generate())%20;
        if(!Map[x][y])
        {
            Map[x][y] = 1;
            i++;
        }
    }
    for(int i = 6; i<=8; i++)
    {
        for(int j = 8; j<= 10; j++)
        {
            Map[i][j] = 3;
        }
    }
    Map[6][10] = 0;
}
