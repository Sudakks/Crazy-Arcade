#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H


#include "component.h"
#include "gameobject.h"
#include "physics.h"
#include "imagetransform.h"
class UserController : public Component
{
public:
    UserController ();
    //对应到泡泡堂游戏中，你可以将上下左右移动的四个键值通过构造函数传入，即可以利用其来操作两个玩家

void onAttach () override;
void onUpdate( float deltaTime ) override;

private:
    int speed;
    int tool_speed;
    int tool_bomb;
    //float vx;
    //float vy;

private:
    Physics *physics;
    Transform *transform;
    ImageTransform * imageTransform;

public :
    void set_speed(int s);
    void set_tool_speed(int t);
    void set_tool_bomb(int t);
    int get_tool_speed();
    int get_tool_bomb();
    bool judge_walk(float vx, float vy, int dir);
};

#endif // USERCONTROLLER_H