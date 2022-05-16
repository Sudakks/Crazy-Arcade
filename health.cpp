#include "health.h"
#include "gameobject.h"
#include "transform.h"
#include "common.h"
Health::Health(int health) : health(health) {}

void Health::beHit() {
  health--;
  if (health <= 0) {
    auto transform = this->gameObject->getComponent<Transform>();
    updateTimer->stop();
    QGraphicsPixmapItem *pixmapItem0 = new QGraphicsPixmapItem(QPixmap(":/surface/image/surface/background.png"));
    //gameScene->addItem(pixmapItem0);
    if(transform->type() == 1)
    {
        //表明玩家1获胜

    }
    else
    {

    }
    destory(this->gameObject);
  }
}
