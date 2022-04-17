#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
//玩家有的属性：道具的时长，速度
class player
{
public:
    player();
private:
    int speed = 0;
    int tool_speed = 0;
    int tool_bomb = 0;
};

#endif // PLAYER_H
