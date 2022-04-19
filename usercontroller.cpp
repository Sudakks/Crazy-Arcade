#include "usercontroller.h"
#include "imagetransform.h"
#include "transformbuilder.h"
#include "transform.h"
#include <QGraphicsItem>
#include "mainwindow.h"
#include "map.h"
#include "common.h"
#include <QDebug>
#include "shooter.h"
#include "component.h"
#include "ammo.h"
#include "gameobject.h"

UserController::UserController (int speed, float range) {
    this->speed = speed;
    this->range = range;
}

void UserController::onAttach () {
    physics = this->gameObject->getComponent<Physics>();
    assert ( physics != nullptr );
    imageTransform = this->gameObject->getComponent<ImageTransform>();
    Q_ASSERT ( imageTransform != nullptr );
    transform = this->gameObject->getComponent<Transform>();
    Q_ASSERT ( transform != nullptr );
}

void UserController::onUpdate( float deltaTime ) {
    float vx = 0 , vy = 0;
    if(getKey(Qt::Key_Space))
    {
        float x = this->transform->pos().x();
        float y = this->transform->pos().y();
        //此时新建一个炸弹的对象
        auto shooter = new GameObject();
        if(!tool_bomb)
        {
            //表示按下了空格键,此时要新建一个炸弹进行处理
            ImageTransformBuilder()
                    .setPos(QPointF(x+20, y+20))
                    .setImage(":/bomb1/image/Bomb1/1.png")
                    .setAlignment(Qt::AlignCenter)
                    .addToGameObject(shooter);
        }
        else
        {
            ImageTransformBuilder()
                    .setPos(QPointF(x+20, y+20))
                    .setImage(":/bomb1/image/Bomb1/11.png")
                    .setAlignment(Qt::AlignCenter)
                    .addToGameObject(shooter);
        }
        shooter->addComponent(new Component);
        shooter->addComponent(new ImageTransform);
        shooter->addComponent(new Transform);
        shooter->addComponent(new Shooter(range, 0));//这个是调用userController的数据
        this->attachGameObject(shooter);
        //这一步相当于把shoooter放到了gameScene上面
    }
    else
    {
        if (getKey(Qt::Key_A) )
        {
           imageTransform->setImage(":/player1/image/Player1/p1_left.png");
           if(judge_walk(-30 * speed, 0, 1))
               vx -= 30 * speed;
        }
        if (getKey(Qt::Key_D) )
        {
            imageTransform->setImage(":/player1/image/Player1/p1_right.png");
            if(judge_walk(30 * speed, 0, 2))
                vx += 30 * speed;
        }
        if (getKey(Qt::Key_W) )
        {
            imageTransform->setImage(":/player1/image/Player1/p1_up.png");
            if(judge_walk(0, -30 * speed, 3))
                vy -= 30 * speed;
        }
        if (getKey(Qt::Key_S) )
        {
            imageTransform->setImage(":/player1/image/Player1/p1_down.png");
            if(judge_walk(0, 30 * speed, 4))
                vy += 30 * speed;
        }
            physics->setVelocity(vx, vy);
    }
}

void UserController::set_speed(int s)
{
    this->speed = s;
}

void UserController::set_tool_speed(int t)
{
    this->tool_speed = t;
}

void UserController::set_tool_bomb(int t)
{
    this->tool_bomb = t;
}

float UserController::get_tool_speed()
{
    return this->tool_speed;
}

float UserController::get_tool_bomb()
{
    return this->tool_bomb;
}

bool UserController::judge_walk(float vx, float vy, int dir)
{
    float x, y;
    //
    const float wall_h = 40;
    const float wall_w = 40;
    const float offset = 20;
    const float offset1 = 37;
    const float offset2 = 10;
    //
    float left_x = this->transform->pos().x();
    float up_y = this->transform->pos().y();
    float right_x = left_x + wall_w;
    float down_y = up_y + wall_h;
    float half_y = (up_y + down_y) / 2;
    //
    int up_Y = up_y / wall_h;
    int half_Y = half_y / wall_h;
    if(dir == 1)
    {
        //left
        x = left_x;
        if(up_Y == half_Y)
        {
            y = up_y;
            y += offset1;
        }

        else
        {
            y = down_y;
            y -= offset;
        }
    }
    else if(dir == 2)
    {
        //right
        x = right_x;
        if(up_Y == half_Y)
        {
            y = up_y;
            y += offset1;
        }

        else
        {
            y = down_y;
            y -= offset;
        }
    }
    else if(dir == 3)
    {
        //up
        y = up_y;
        if(My_map.get_map(y / wall_h, (left_x + offset2) / wall_w) || My_map.get_map(y / wall_h, (right_x - offset2) / wall_w))
            return false;
        else
            return true;
    }
    else
    {
        //down
        y = down_y;
        if(My_map.get_map(y / wall_h, (left_x + offset2) / wall_w) || My_map.get_map(y / wall_h, (right_x - offset2) / wall_w))
            return false;
        else
            return true;
    }
    float nowX = x + vx * 0.0166;
    float nowY = y + vy * 0.0166;
    int mapX = nowX / wall_w;
    int mapY = nowY / wall_h;
    //
    if(My_map.get_map(mapY, mapX))
    {
        return false;
    }
    return true;
    //似乎好像改对了吧www太难了www
}
