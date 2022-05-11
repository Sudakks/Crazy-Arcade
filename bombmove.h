#ifndef BOMBMOVE_H
#define BOMBMOVE_H
#include "imagetransform.h"
#include "transform.h"
#include "transformbuilder.h"
#include "shooter.h"
#include <QDebug>
#include "component.h"
#include "gameobject.h"
#include "physics.h"

class bombMove: public Component
{
public:
    bombMove(int dir);
    //这个是专门用来移动炸弹的
    void onAttach () override;
    void onUpdate( float deltaTime ) override;
    bool judge();

private:
private:

    Transform *transform;
    ImageTransform * imageTransform;


};

#endif // BOMBMOVE_H
