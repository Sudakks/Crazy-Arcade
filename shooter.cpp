#include "shooter.h"
#include "ammo.h"
#include <gameobject.h>
#include <transform.h>
#include <QDebug>
#include "transformbuilder.h"
#include <QGraphicsRectItem>
#include <imagetransform.h>
#include "map.h"
#include "common.h"

Shooter::Shooter(float range, float wait_time) : Component() {
    this->range = range;
    this->wait_time = wait_time;
}

void Shooter::onAttach() {
    this->transform = this->gameObject->getComponent<Transform>();
    Q_ASSERT(this->transform != nullptr);
    this->imageTransform = this->gameObject->getComponent<ImageTransform>();
    Q_ASSERT(this->imageTransform != nullptr);
    /*auto rect = new QGraphicsRectItem(this->transform);
    rect->setRect(QRectF(-30, -30, 60, 60));*/
}

void Shooter::onUpdate(float deltaTime) {
   wait_time += 1;
   if(wait_time > 180)
   {    //QList<GameObject> ammo_list;
        //此时要爆炸了(差不多是3s的时间)
        //新建一个ammo对象
       float x = this->transform->pos().x() / 40;
       float y = this->transform->pos().y() / 40;
       for(int i = 1; i <= 4; i++)
       {          
           auto ammo = new GameObject();
           ammo->addComponent(new Transform);
           ammo->addComponent(new Ammo(range));
           auto amm = ammo->getComponent<Ammo>();
               //这时候应该往四个方位新建对象
               if(this->range == 1)
               {
                   if(i == 1)
                   {
                       //先判断是否能炸
                       if(My_map.get_map(y + 1, x) != 0 && My_map.get_map(y + 1, x) != 2)
                       {
                           continue;
                       }
                       auto flash = new QGraphicsPixmapItem(this->transform);
                       flash->setPixmap(QPixmap(":/flash_1/image/Flash/down/down1.png"));
                       flash->setPos(0, 0);//这个是以图片的中心点的位置发射
                       flash->setOffset(-20,20);
                       Q_ASSERT(flash != nullptr);
                       amm->set_collider(flash);
                   }
                   else if(i == 2)
                   {
                       if(My_map.get_map(y, x - 1) != 0 && My_map.get_map(y, x - 1) != 2)
                           continue;
                       auto flash = new QGraphicsPixmapItem(this->transform);
                       flash->setPixmap(QPixmap(":/flash_1/image/Flash/left/left1.png"));
                       flash->setPos(0,0);
                       flash->setOffset(-60, -20);
                       Q_ASSERT(flash != nullptr);
                       amm->set_collider(flash);
                   }
                   else if(i == 3)
                   {
                       if(My_map.get_map(y, x + 1) != 0 && My_map.get_map(y, x + 1) != 2)
                           continue;
                       auto flash = new QGraphicsPixmapItem(this->transform);
                       flash->setPixmap(QPixmap(":/flash_1/image/Flash/right/right1.png"));
                       flash->setPos(0,0);
                       flash->setOffset(20, -20);
                       Q_ASSERT(flash != nullptr);
                       amm->set_collider(flash);
                   }
                   else
                   {
                       if(My_map.get_map(y - 1, x) != 0 && My_map.get_map(y - 1, x) != 2)
                           continue;
                       auto flash = new QGraphicsPixmapItem(this->transform);
                       flash->setPixmap(QPixmap(":/flash_1/image/Flash/up/up1.png"));
                       flash->setPos(0,0);
                       flash->setOffset(-20, -60);
                       Q_ASSERT(flash != nullptr);
                       amm->set_collider(flash);
                   }
                          attachGameObject(ammo);
               }
               else
               {
                   //这个是威力加倍后的场景
               }

       }


   }
   else
   {
       //切换动画
       if(range == 1)
       {
           if(wait_time == 15 || wait_time == 60 || wait_time == 105|| wait_time == 150 || wait_time == 195)
           {
               imageTransform->setImage(":/bomb1/image/Bomb1/2.png");
           }
           else if (wait_time == 30 || wait_time == 75 || wait_time == 120 || wait_time == 165 || wait_time == 210)
           {
               imageTransform->setImage(":/bomb1/image/Bomb1/3.png");
           }
           else if(wait_time == 45 || wait_time == 90 || wait_time == 135 || wait_time == 180 || wait_time == 225)
           {
               imageTransform->setImage(":/bomb1/image/Bomb1/1.png");
           }
       }
       else
       {
           if(wait_time == 20 || wait_time == 80 || wait_time == 140)
           {
               imageTransform->setImage(":/bomb1/image/Bomb1/22.png");
           }
           else if (wait_time == 40 || wait_time == 100 || wait_time == 160)
           {
               imageTransform->setImage(":/bomb1/image/Bomb1/33.png");
           }
           else if(wait_time == 60 || wait_time == 120 || wait_time == 180)
           {
               imageTransform->setImage(":/bomb1/image/Bomb1/11.png");
           }
       }       
   }
  /*cooldown -= deltaTime;
  if (cooldown > 0) return;
  cooldown = interval;

  QPointF velocity;
  if (this->getKey(Qt::Key_W))
    velocity = QPointF(0, -100);
  else if (this->getKey(Qt::Key_A))
    velocity = QPointF(-100, 0);
  else if (this->getKey(Qt::Key_S))
    velocity = QPointF(0, 100);
  else
    velocity = QPointF(100, 0);
  auto ammo = new GameObject();
  ammo->addComponent(new Transform(this->transform->pos()));
  ammo->addComponent(new Ammo(velocity));
  attachGameObject(ammo);*/

}

void Shooter::set_wait_time(float t)
{
    this->wait_time = t;
}

float Shooter::get_wait_time()
{
    return this->wait_time;
}

void Shooter::set_range(int r)
{
    this->range = r;
}

float Shooter::get_range()
{
    return this->range;
}
