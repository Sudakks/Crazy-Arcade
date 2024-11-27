#include "health.h"
#include "gameobject.h"
#include "transform.h"
#include "common.h"
#include "imagetransform.h"
#include "transformbuilder.h"
#include "gamescene.h"
#include <QDebug>
#include "usercontroller.h"
Health::Health(int health) : health(health) {}

void Health::change_health()
{
    health--;
}

void Health::beHit() {

    auto transform = this->gameObject->getComponent<Transform>();
    auto imageTransform = gameObject->getComponent<ImageTransform>();
    if(transform->type() == 1)
    {
        imageTransform->setImage(":/player1/image/Player1/p1_die.png");
        live1->setText(QString::number(this->health));
    }
    else
    {
        live2->setText(QString::number(this->health));
        imageTransform->setImage(":/player2/image/Player2/p2_die.png");
    }
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

