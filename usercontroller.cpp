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
#include <QMovie>
UserController::UserController (int speed, float range, int bomb_num) {
    this->speed = speed;
    this->range = range;
    this->bomb_num = bomb_num;
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
    if(bomb_list.size())
    {
        //表示有炸弹需要处理
        for(int i = 0; i < bomb_list.size(); i++)
        {
            auto shooter = bomb_list.at(i);
            auto shoot = shooter->getComponent<Shooter>();
            if(shoot->get_wait_time() == 4 * 60)
            {
                //表明这个炸弹已经要被销毁
                score += shoot->get_score();
                bomb_list.pop_front();//先从队列中移走
                this->detachGameObject(shooter);
                bomb_num++;
            }
        }
    }
    if(getKey(Qt::Key_Space) && bomb_num > 0)
    {
        float x = this->transform->pos().x();
        float y = this->transform->pos().y();
        //此时新建一个炸弹的对象
        auto shooter = new GameObject();
        bomb_num--;
        qDebug() << "现在的炸弹数量为" << bomb_num;
        bomb_list.emplace_back(shooter);
        //表示按下了空格键,此时要新建一个炸弹进行处理
        ImageTransformBuilder()
                .setPos(QPointF(x+20, y+20))
                .setImage(":/bomb1/image/Bomb1/1.png")
                .setAlignment(Qt::AlignCenter)
                .addToGameObject(shooter);
        shooter->addComponent(new Component);
        shooter->addComponent(new ImageTransform);
        shooter->addComponent(new Transform);
        shooter->addComponent(new Shooter(range, 0, transform));//这个是调用userController的数据
        this->attachGameObject(shooter);
        //这一步相当于把shoooter放到了gameScene上面
    }
    else
    {
        if (getKey(Qt::Key_A) )
        {
           imageTransform->setImage(":/player1/image/Player1/p1_left.png");
           if(judge_walk(-35 * speed, 0, 1))
               vx -= 35 * speed;
        }
        if (getKey(Qt::Key_D) )
        {
            imageTransform->setImage(":/player1/image/Player1/p1_right.png");
            if(judge_walk(35 * speed, 0, 2))
                vx += 35 * speed;
        }
        if (getKey(Qt::Key_W) )
        {
            imageTransform->setImage(":/player1/image/Player1/p1_up.png");
            if(judge_walk(0, -35 * speed, 3))
                vy -= 35 * speed;
        }
        if (getKey(Qt::Key_S) )
        {
            imageTransform->setImage(":/player1/image/Player1/p1_down.png");
            if(judge_walk(0, 35 * speed, 4))
                vy += 35 * speed;
        }
            physics->setVelocity(vx, vy);
    }
}

void UserController::set_tool_speed(int t)
{
    this->tool_speed += t;
}

void UserController::set_tool_range(int t)
{
    this->tool_range += t;
}

void UserController::set_tool_bomb_num(int t)
{
    this->tool_bomb_num += t;
}
//以上是设置道具时长
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
        if(My_map.get_map(y / wall_h, (left_x + offset2) / wall_w) >= 1 || My_map.get_map(y / wall_h, (right_x - offset2) / wall_w) >= 1)
            return false;
        else
        {
            judge_tool(y / wall_h, (left_x + offset2) / wall_w);
            judge_tool(y / wall_h, (right_x - offset2) / wall_w);
            return true;
        }

    }
    else
    {
        //down
        y = down_y;
        if(My_map.get_map(y / wall_h, (left_x + offset2) / wall_w) >= 1 || My_map.get_map(y / wall_h, (right_x - offset2) / wall_w) >= 1)
            return false;
        else
        {
            judge_tool(y / wall_h, (left_x + offset2) / wall_w);
            judge_tool(y / wall_h, (right_x - offset2) / wall_w);
            return true;
        }
    }
    float nowX = x + vx * 0.0166;
    float nowY = y + vy * 0.0166;
    int mapX = nowX / wall_w;
    int mapY = nowY / wall_h;
    //
    if(My_map.get_map(mapY, mapX) >= 1)
    {
        return false;
    }
    else
    {
        judge_tool(mapX, mapY);
        return true;
    }

    //似乎好像改对了吧www太难了www
}

int UserController::get_score()
{
    return this->score;
}

void UserController::add_score(int s)
{
    this->score += s;
}


void UserController::judge_tool(int x, int y)
{
    int now = My_map.get_map(x, y);
    //qDebug() << x << "  "<< y;
    if(now == speed_tool || now == bomb_num_tool || now == range_tool)
    {
        //利用队列查找位置
        qDebug() << tool_list.size();
        auto begin = tool_list.begin();
        for(int i = 0; i < tool_list.size(); i++)
        {
            GameObject* tool = tool_list.at(i);
            Transform* pos = tool->getComponent<Transform>();
            //qDebug() << "道具的位置是" << pos->pos().x() / 40 << "  "<< pos->pos().y() / 40;
            if(pos->pos().x() / 40 == y && pos->pos().y() / 40 == x)
            {
                //说明找到了这一个道具
                //直接消除这一个gameObject
                tool_list.erase(begin + i);//先从队列中移走
                this->detachGameObject(tool);
                break;
            }

        }
        if(now == speed_tool)
        {
            //qDebug() << "-1";
            speed += 1;
        }
        else if(now == bomb_num_tool)
        {
            //qDebug() << "-2";
            bomb_num += 1;
            qDebug() << "加完道具后的炸弹数量为" << bomb_num;
        }
        else if(now == range_tool)
        {
            //qDebug() << "-3";
            range += 1;
        }
    }
    My_map.set_map(x, y, 0);
}
