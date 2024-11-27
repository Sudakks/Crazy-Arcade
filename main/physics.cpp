#include "physics.h"

Physics::Physics () {}
void Physics::onAttach () {
transform = this->gameObject->getComponent<Transform>();
assert ( transform != nullptr ) ;
}
void Physics::onUpdate( float deltaTime ) {
transform->moveBy(deltaTime *vx , deltaTime *vy);
//moveBy是现成函数
}

void Physics::setVelocity ( float vx , float vy) {
    this->vx = vx;
    this->vy = vy;
}
