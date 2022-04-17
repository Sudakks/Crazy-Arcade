#include "usercontroller.h"
#include "player.h"
#include "imagetransform.h"
#include "transformbuilder.h"
UserController::UserController () {}

void UserController::onAttach () {
    physics = this->gameObject->getComponent<Physics>();
    assert ( physics != nullptr );
    imageTransform= this->gameObject->getComponent<ImageTransform>();
    Q_ASSERT ( imageTransform != nullptr );
    transform = this->gameObject->getComponent<Transform>();
}

void UserController::onUpdate( float deltaTime ) {
    float vx = 0 , vy = 0;
    if (getKey(Qt::Key_A) )
    {
       imageTransform->setImage("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Player1/p1_up");
        vx -= 30;
    }
    if (getKey(Qt::Key_D) )
    {
        vx += 30;
    }
    if (getKey(Qt::Key_W) )
    {
        vy -= 30;
    }
    if (getKey(Qt::Key_S) )
    {
        vy += 30;
    }
    physics->setVelocity(vx, vy);
}
