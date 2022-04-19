#include "ammo.h"
#include <QDebug>
#include <QBrush>
#include <QGraphicsEllipseItem>

#include "hitable.h"

Ammo::Ammo(float range) : Component(){
    this->range = range;
}

void Ammo::onAttach() {
  this->transform = this->gameObject->getComponent<Transform>();
  Q_ASSERT(this->transform != nullptr);
  /*auto circle = new QGraphicsEllipseItem(this->transform);
  //auto:系统自己会帮助我们给出变量的声明
  circle->setRect(QRectF(-10, -10, 20, 20));
  circle->setBrush(QBrush(Qt::black));
  this->collider = circle;*/
}

void Ammo::onUpdate(float deltaTime) {
    timeToLive--;
  if (timeToLive <= 0) {
    destory(this->gameObject);
  }
  //this->transform->setPos(this->transform->pos() + velocity * deltaTime);
  for (auto item : this->collider->collidingItems()) {//与它碰撞的东西
    while (item->parentItem() != nullptr) item = item->parentItem();
    auto transform = dynamic_cast<Transform *>(item);
    if (transform == nullptr) continue;
    auto gameObject = transform->getParentGameObject();
    auto hitable = gameObject->getComponent<Hitable>();
    if (hitable == nullptr) continue;
    hitable->beHit();
    destory(this->gameObject);
    break;
  }
}

void Ammo::set_collider(QGraphicsItem *co)
{
    this->collider = co;
}

