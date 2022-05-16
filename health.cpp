#include "health.h"
#include "gameobject.h"
#include "transform.h"
#include "common.h"
#include "imagetransform.h"
#include "transformbuilder.h"
#include "gamescene.h"
Health::Health(int health) : health(health) {}

void Health::beHit() {
    if(have_hit == false)
    {
        have_hit = true;
        health--;
    }
    auto transform = this->gameObject->getComponent<Transform>();
    if(transform->type() == 1)
        live1->setText(QString::number(this->health));
    else
        live2->setText(QString::number(this->health));
    if (health <= 0) {
    auto transform = this->gameObject->getComponent<Transform>();
    if(transform->type() == 1)
    {
        //表明玩家2获胜
        auto win = new GameObject;
        ImageTransformBuilder()
                .setPos(QPointF(0, 0))
                .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                .setImage(":/surface/image/surface/p2_win2.png")
                .addToGameObject(win);
        attachGameObject(win);
    }
    else
    {
        auto win = new GameObject;
        ImageTransformBuilder()
                .setPos(QPointF(0, 0))
                .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                .setImage(":/surface/image/surface/p1_win2.png")
                .addToGameObject(win);
        attachGameObject(win);
    }
    this->gameObject->get_scene()->get_updateTimer()->stop();
    destory(this->gameObject);
  }
}

void Health::set_hit(int val)
{
    this->have_hit = val;
}
