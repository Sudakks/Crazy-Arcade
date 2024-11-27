#ifndef PHYSICS_H
#define PHYSICS_H

#include "component.h"
#include "gameobject.h"
#include "transform.h"

class Physics : public Component
{
public :
    Physics ();
    void onAttach () override;
    void onUpdate( float deltaTime ) override;
    //要增加碰撞测试（使得人物在碰到物体时停下来，或者是推动的炸弹碰到的时候停下来）

void setVelocity ( float vx , float vy) ;
void setImage();

private :
   Transform *transform;
   //需要经常使用到游戏对象的 Transform 组件以更改位置，因此在类中缓存了一个Transform 组件的指针
   //随后在游戏每一次更新时，根据经过的时间更新游戏对象的位置
   float vx = 0 , vy = 0;
};

#endif // PHYSICS_H
