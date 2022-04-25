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
#include <QList>
#include <QRandomGenerator>

Shooter::Shooter(float range, float wait_time, Transform* userTransform) : Component() {
    this->range = range;
    this->wait_time = wait_time;
    this->userTransform = userTransform;//这个获取到了放炸弹的人的transform
    //所以我可以把道具的图片放到这个transform上面
}

void Shooter::onAttach() {
    this->transform = this->gameObject->getComponent<Transform>();
    Q_ASSERT(this->transform != nullptr);
    this->imageTransform = this->gameObject->getComponent<ImageTransform>();
    Q_ASSERT(this->imageTransform != nullptr);
}

void Shooter::onUpdate(float deltaTime) {
   wait_time += 1;
   if(wait_time == 180)
   {
       //此时要爆炸了(差不多是3s的时间)
       //新建一个ammo对象
       //随机数生成道具
       float x = this->transform->pos().x() / 40;
       float y = this->transform->pos().y() / 40;
       for(int i = 1; i <= range; i++)
       {
           //四个方向(down)
           if(My_map.get_map(y + i, x) != 0 && My_map.get_map(y + i, x) != 2)
               break;
            develop_bomb(-20 + 5, 20 + 40 * (i - 1));
           if(My_map.get_map(y + i, x) == 2)
           {
               //score += soft_score;//得分准备
               //说明是软墙，此时要进行准备摆放道具图片，并更改二维数组的内容
                change_map(y + i, x);
           }
       }
       for(int i = 1; i <= range; i++)
       {
           //left
           if(My_map.get_map(y, x - i) != 0 && My_map.get_map(y, x - i) != 2)
               break;
           develop_bomb(-20 + (-40) * i + 5, -20);
           if(My_map.get_map(y, x - i) == 2)
           {
               //score += soft_score;
               change_map(y, x - i);
           }
       }
       for(int i = 1; i <= range; i++)
       {
           //right
           if(My_map.get_map(y, x + i) != 0 && My_map.get_map(y, x + i) != 2)
               break;
            develop_bomb(5 + 20 + 40 * (i - 1), -20);
           if(My_map.get_map(y, x + i) == 2)
           {
               //score += soft_score;
               change_map(y, x + i);
           }
       }
       for(int i = 1; i <= range; i++)
       {
            //up
           if(My_map.get_map(y - i, x) != 0 && My_map.get_map(y - i, x) != 2)
               break;
            develop_bomb(-20 + 5, -20 + (-40) * i);
           if(My_map.get_map(y - i, x) == 2)
           {
               //score += soft_score;
               change_map(y - i, x);
           }
       }
   }
   else if(wait_time > 180)
   {
       //这个是用来变换照片的
       if(wait_time == 195)
       {
            for(int i = 0; i < flash_list.size(); i++)
            {
                auto fla = flash_list.at(i);
                fla->setPixmap(QPixmap(":/flash/image/Flash/2.png"));
            }
       }
       else if(wait_time == 210)
       {
           for(int i = 0; i < flash_list.size(); i++)
           {
               auto fla = flash_list.at(i);
               fla->setPixmap(QPixmap(":/flash/image/Flash/1.png"));
           }
       }
       else if(wait_time == 225)
       {
           for(int i = 0; i < flash_list.size(); i++)
           {
               auto fla = flash_list.at(i);
               fla->setPixmap(QPixmap(":/flash/image/Flash/3.png"));
           }
       }
   }
   else
   {
       //切换动画
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

int Shooter::get_score()
{
    return this->score;
}

int Shooter::random_tool()
{
    quint32 pr = (QRandomGenerator::global()->generate()) % 7;
    return -pr;
}

void Shooter::change_map(int x, int y)
{
    int random =  random_tool();
    if(random == speed_tool || random == range_tool || random == bomb_num_tool)
    {
        //新建一个gameObject然后把它挂到玩家的transform上面
        //或者是自己就是一个gameObject，然后自己也有transform，放进list里面，玩家捡到了就detach掉
        auto tool = new GameObject();

        if(random == speed_tool)
        {
            //获得加速道具
            //设置图片
            ImageTransformBuilder()
                    .setPos(QPointF(40 * y, 40 * x))
                    .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                    .setImage(":/tool/image/Tool/speed.png")
                    .addToGameObject(tool);
            //qDebug() << "1";
            My_map.set_map(x, y, speed_tool);
            //qDebug() << "实际位置是" << tr->pos().x() << "," << tr->pos().y();
            //qDebug() << "map[" << x << "][" << y << "] = " << My_map.get_map(x, y);
        }
        else if(random == range_tool)
        {
            //获得炸弹威力增加道具
            ImageTransformBuilder()
                    .setPos(QPointF(40 * y, 40 * x))
                    .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                    .setImage(":/tool/image/Tool/range.png")
                    .addToGameObject(tool);
            //qDebug() << "2";
            My_map.set_map(x, y, range_tool);
            //qDebug() << "实际位置是" << tr->pos().x() << "," << tr->pos().y();
            //qDebug() << "map[" << x << "][" << y << "] = " << My_map.get_map(x, y);
        }
        else
        {
            //获得炸弹拥有个数增加的道具
            ImageTransformBuilder()
                    .setPos(QPointF(40 * y, 40 * x))
                    .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                    .setImage(":/tool/image/Tool/num.png")
                    .addToGameObject(tool);
            //qDebug() << "3";
            My_map.set_map(x, y, bomb_num_tool);
            //qDebug() << "实际位置是" << tr->pos().x() << "," << tr->pos().y();
            //qDebug() << "map[" << x << "][" << y << "] = " << My_map.get_map(x, y);
        }
        attachGameObject(tool);
        tool->addComponent(new Transform);
        tool->addComponent(new ImageTransform);
        tool_list.emplace_back(tool);//并且把它加到道具队列中
    }
    else
    {
        My_map.set_map(x, y, 0);//如果没有抽到道具，还是相当于炸开
    }

}

void Shooter::develop_bomb(float offsetX, float offsetY)
{
    //利用这个添加光束火焰
    auto ammo = new GameObject();
    ammo->addComponent(new Transform);
    ammo->addComponent(new Ammo(this->transform->type()));
    auto amm = ammo->getComponent<Ammo>();
    auto flash = new QGraphicsPixmapItem(this->transform);
    flash->setPixmap(QPixmap(":/flash/image/Flash/1.png"));
    flash->setPos(0, 0);//这个是以图片的中心点的位置发射
    flash->setOffset(offsetX, offsetY);
    this->flash_list.emplace_back(flash);
    amm->set_collider(flash);
    attachGameObject(ammo);
}
