#include "ammo.h"
#include <QDebug>
#include <QBrush>
#include <QGraphicsEllipseItem>
#include "hitable.h"
#include "imagetransform.h"
#include "component.h"
#include "common.h"

Ammo::Ammo(int type) : Component(){
    this->type = type;
}

void Ammo::onAttach() {
  this->transform = this->gameObject->getComponent<Transform>();
  Q_ASSERT(this->transform != nullptr);
}

void Ammo::onUpdate(float deltaTime) {
    timeToLive--;
  if (timeToLive <= 0) {
    destory(this->gameObject);
  }
  for (auto item : this->collider->collidingItems()) {//与它碰撞的东西
    while (item->parentItem() != nullptr) item = item->parentItem();
    auto transform = dynamic_cast<Transform *>(item);
    if (transform == nullptr) continue;
    auto gameObject = transform->getParentGameObject();
    auto hitable = gameObject->getComponent<Hitable>();
    if(transform->type() == 1)
    {
        //player1
        //imageTransform->setImage(":/player1/image/Player1/p1_die.png");
        if(type == 2)
        {
            //加分(就说明炸到的不是自己)
            p2_score += player_score;
        }
        else if(type == -1)
        {
            r1_score += player_score;
        }
        else if(type == -1)
        {
            r2_score += player_score;
        }
    }
    else if(transform->type() == 2)
    {
        //player2
        //imageTransform->setImage(":/player2/image/Player2/p2_die.png");
        if(type == 1)
            p1_score += player_score;
        else if(type == -1)
            r1_score += player_score;
        else if(type == -2)
            r2_score += player_score;
    }
    if (hitable == nullptr) continue;
    //这里加上对类型的判断
    judge_type(transform, gameObject);
    hitable->beHit();
    destory(this->gameObject);
    break;
  }
  score1->setText(QString::number(p1_score));
  score2->setText(QString::number(p2_score));
  score_r1->setText(QString::number(r1_score));
  score_r2->setText(QString::number(r2_score));
}

void Ammo::set_collider(QGraphicsItem *co)
{
    this->collider = co;
}

void Ammo::judge_type(Transform *transform, GameObject* gameObject)
{
    auto imageTransform = gameObject->getComponent<ImageTransform>();
    if(transform->type() == -1)
    {
        //robot1
        //imageTransform->setImage(":/robot1/image/Robot1/r1_die.png");
        if(type == 1)
            p1_score += robot_score;
        else if(type == 2)
            p2_score += robot_score;
        else if(type == -2)
            r2_score += robot_score;
    }
    else if(transform->type() == -2)
    {
        //robot2
        //imageTransform->setImage(":/robot2/image/Robot2/r2_die.png");
        if(type == 1)
            p1_score += robot_score;
        else if(type == 2)
            p2_score += robot_score;
        else if(type == -1)
            r1_score += robot_score;
    }
    else if(transform->type() == 0)
    {
        //wall
        if(type == 1)
            p1_score += soft_score;
        else if(type == 2)
            p2_score += soft_score;
        else if(type == -1)
            r1_score += soft_score;
        else if(type == -2)
            r2_score += soft_score;
        //通过这里改变地图的表示
        float X = transform->pos().x();
        float Y = transform->pos().y();
        //注意X和Y是反过来的
        My_map.set_map(Y / 40, X / 40, 0);
    }
}
