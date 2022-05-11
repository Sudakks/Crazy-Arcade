#include "bombmove.h"
#include "imagetransform.h"
#include "transform.h"
#include "transformbuilder.h"
#include "shooter.h"
#include <QDebug>
#include "component.h"
#include "gameobject.h"
#include "physics.h"
#include "common.h"
#include "map.h"

bombMove::bombMove(int dir)
{
    this->dir = dir;
}

void bombMove::onAttach()
{
    physics = this->gameObject->getComponent<Physics>();
    Q_ASSERT ( physics != nullptr );
    transform = this->gameObject->getComponent<Transform>();
    Q_ASSERT ( transform != nullptr );
    x = transform->pos().x();
    y = transform->pos().y();
    qDebug() << x << y;
}

void bombMove::onUpdate(float deltaTime)
{
    //这个就是用来判断炸弹是否需要停下来

}

bool bombMove::judge()
{

}

