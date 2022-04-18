#include "usercontroller.h"
#include "player.h"
#include "imagetransform.h"
#include "transformbuilder.h"
#include "transform.h"
#include <QGraphicsItem>
#include "mainwindow.h"
#include "map.h"
#include "common.h"
#include <QDebug>
UserController::UserController () {

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
    if (getKey(Qt::Key_A) )
    {
       imageTransform->setImage(":/player1/image/Player1/p1_left.png");
       if(judge_walk(-30, 0, 1))
           vx -= 30;
    }
    if (getKey(Qt::Key_D) )
    {
        imageTransform->setImage(":/player1/image/Player1/p1_right.png");
        if(judge_walk(30, 0, 2))
            vx += 30;
    }
    if (getKey(Qt::Key_W) )
    {
        imageTransform->setImage(":/player1/image/Player1/p1_up.png");
        if(judge_walk(0, -30, 3))
            vy -= 30;
    }
    if (getKey(Qt::Key_S) )
    {
        imageTransform->setImage(":/player1/image/Player1/p1_down.png");
        if(judge_walk(0, 30, 4))
            vy += 30;
    }
        physics->setVelocity(vx, vy);

}

bool UserController::judge_walk(float vx, float vy, int dir)
{
    float x, y;
    //
    const float wall_h = 40;
    const float wall_w = 40;
    const float offset = 20;
    const float offset1 = 37;
    //
    float left_x = this->transform->pos().x();
    float up_y = this->transform->pos().y();
    float right_x = left_x + wall_w;
    float down_y = up_y + wall_h;
    float half_x = (left_x + right_x) / 2;
    float half_y = (up_y + down_y) / 2;
    //
    int up_Y = up_y / wall_h;
    int half_Y = half_y / wall_h;
    int left_X = left_x / wall_w;
    int half_X = half_x / wall_w;
    //图片进入一半后才算真正的进入(确定人物位置)
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
        if(left_X == half_X)
        {
            x = left_x;
            x += offset1;
        }
        else
        {
            x = right_x;
            x -= offset1;
        }
    }
    else
    {
        //down
        y = down_y;
        if(left_X == half_X)
        {
            x = left_x;
            x += offset1;
        }
        else
        {
            x = right_x;
            x -= offset1;
        }
    }
    //int oriX = x / wall_w;
    //int oriY = y / wall_h;
    //qDebug() << "人物的位置:   " << x << "  " << y;
    //qDebug() << "人物的格子:   " << oriX << "   "<< oriY;
    float nowX = x + vx * 0.0166;
    float nowY = y + vy * 0.0166;
    int mapX = nowX / wall_w;
    int mapY = nowY / wall_h;
    //
    //qDebug() << "地图X: " << nowX << "   "<< "地图Y:  " << nowY;
    //qDebug() << "格子数为" << mapX << "   "<< mapY << "此时的内容为" << My_map.get_map(mapX, mapY);
    if(My_map.get_map(mapY, mapX))
    {
        return false;
    }
    return true;
    //似乎好像改对了吧www太难了www
}
