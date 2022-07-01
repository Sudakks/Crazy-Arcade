#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "component.h"
#include "gameobject.h"
#include "physics.h"
#include "imagetransform.h"

class AIcontroller: public Component
{
public:
    AIcontroller(QString up1, QString up2, QString down1, QString down2, QString left1, QString left2, QString right1, QString right2);
    //这一个是两个机器人的属性，即一直在update然后定时在放炸弹
    //构造函数（包括up,down,left,right的图片，以及死掉了的图片）
void onAttach () override;
void onUpdate( float deltaTime ) override;
bool out_of_range(double vx, double vy, int dir);
bool judge(float vx, float vy, int dir);
bool distance(double vx, double vy);
bool now_at();
void free_walk();
void bomb_walk();
void change_image(int dir);
void judge_tool(int x, int y);
int find_tool(int step, int X, int Y, int offsetX, int offsetY);
void tool_walk();
int judge1(int vx, int vy, int dir);

private:
    Physics *physics;
    Transform *transform;
    ImageTransform * imageTransform;

    int interval = 7 * 60;//表示至少每一次隔7s才放炸弹
    int tool_interval = 6 * 60;//相当于每隔4s去找一次道具
    int bomb_time = 0;//表示炸弹爆炸的整个时长，即4s

    int range = 1;//机器人的炸弹范围
    int speed = 1;
    int bomb_num = 1;//这个是机器人拥有的炸弹数量
    float tool_bomb_num = 0;//其实不捡这个道具，因为机器人就是按照一定频率放💣
    float tool_speed = 0;
    float tool_range = 0;
    float tool_move = 0;

    double bomb_x = 0;//这个是机器人放炸弹的坐标
    double bomb_y = 0;
    //因为机器人是横平竖直地走，所以只用考虑一个方向上它是否变小了
    QString up1, up2;
    QString down1, down2;
    QString left1, left2;
    QString right1, right2;

    GameObject* bomb;
    //int bombX, bombY;//表示它放的炸弹的位置
    bool whether_bomb = 0;//这个用来表示是否放了炸弹
    int last_dir = 3;//这个表示上一次的方向（如果能接着走，就继续接着这个方向前进，默认为1， 即默认向下走）
    //逃离炸弹的逻辑是：判断两个，只要有一个满足即可
    //一个是离炸弹中心的位置越来越远， 一个是没有跑到炸弹的爆炸范围里
    //第一个主要是针对刚开始逃离炸弹，第二个主要是后期不跑到炸弹里面
    QList<int> pathX;//这个用来存机器人要走的路线的坐标位置
    QList<int> pathY;//相互对应的是X和Y的坐标，表示他们每一步要走的路线

    int down_num = 0;
    int right_num = 0;
    int up_num = 0;
    int left_num = 0;
};

#endif // AICONTROLLER_H
