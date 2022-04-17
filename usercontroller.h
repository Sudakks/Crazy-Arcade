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
    Physics *physics;
    Transform *transform;
    ImageTransform * imageTransform;
};

#endif // USERCONTROLLER_H
