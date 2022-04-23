#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H


#include "component.h"
#include "gameobject.h"
#include "physics.h"
#include "imagetransform.h"
#include <QList>
class UserController : public Component
{
public:
    UserController (int speed, float range, int bomb_num, Qt::Key key_up, Qt::Key key_down, Qt::Key key_left, Qt::Key key_right, Qt::Key key_bomb, QString up, QString down, QString left, QString right);
    //对应到泡泡堂游戏中，你可以将上下左右移动的四个键值通过构造函数传入，即可以利用其来操作两个玩家

void onAttach () override;
void onUpdate( float deltaTime ) override;

private:
    int speed;
    int range;//这个表示炸弹的范围
    int bomb_num = 1;
    float tool_speed = 0;
    float tool_bomb_num = 0;//道具的时限
    float tool_range = 0;
    int score = 0;
    float limit = -1;//用来控制按下空格键多久才能继续按下(先设置为1.5s)
    //利用构造函数，分别表示两个玩家
    Qt::Key key_up;
    Qt::Key key_down;
    Qt::Key key_left;
    Qt::Key key_right;
    Qt::Key key_bomb;

    QString up_image;
    QString down_image;
    QString left_image;
    QString right_image;


private:
    Physics *physics;
    Transform *transform;
    ImageTransform * imageTransform;
    QList<GameObject*> bomb_list;//里面装的是指针

public :
    //速度
    void set_speed(int s);
    void set_tool_speed(int t);//设置道具时间
    //范围
    void set_tool_range(int t);
    void set_range(int r);
    //数量
    void set_bomb_num();
    void set_tool_bomb_num(int t);

    bool judge_walk(float vx, float vy, int dir);
    int get_score();
    void add_score(int s);
    void judge_tool(int x, int y);

};

#endif // USERCONTROLLER_H
