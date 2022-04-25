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

UserController::UserController (int speed, float range, int bomb_num, Qt::Key key_up, Qt::Key key_down, Qt::Key key_left, Qt::Key key_right, Qt::Key key_bomb, QString up, QString down, QString left, QString right)
{
    this->speed = speed;
    this->range = range;
    this->bomb_num = bomb_num;

    this->key_up = key_up;
    this->key_down = key_down;
    this->key_left = key_left;
    this->key_right = key_right;
    this->key_bomb = key_bomb;

    this->up_image = up;
    this->down_image = down;
    this->left_image = left;
    this->right_image = right;
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
    if(tool_speed > 0)
        tool_speed -= deltaTime;
    if(tool_bomb_num > 0)
        tool_bomb_num -= deltaTime;
    if(tool_range > 0)
        tool_range -= deltaTime;
    if(tool_speed <= 0 && speed > 1)
        speed -= 1;
    if(tool_bomb_num <= 0 && bomb_num > 1)
        bomb_num -= 1;
    if(tool_range <= 0 && range > 1)
        range -= 1;
    float vx = 0 , vy = 0;
    if(limit > 0)
        limit -= deltaTime;
    if(bomb_list.size())
    {
        //表示有炸弹需要处理
        for(int i = 0; i < bomb_list.size(); i++)
        {
            auto shooter = bomb_list.at(i);
            auto shoot = shooter->getComponent<Shooter>();
            auto trans = shooter->getComponent<Transform>();
            if(shoot->get_wait_time() >= 4 * 60)
            {
                //表明这个炸弹已经要被销毁
                if(trans->type() == this->transform->type())
                {
                    //score += shoot->get_score();
                    bomb_num++;
                    //说明是自己的炸弹
                    bomb_list.pop_front();//先从队列中移走
                    this->detachGameObject(shooter);
                }
                else if(trans->type() < 0)
                {
                    //第二种情况是机器人放的炸弹
                    bomb_list.pop_front();//先从队列中移走
                    this->detachGameObject(shooter);
                }
            }
        }
        //qDebug() << p1_score << p2_score;
    }
    if(getKey(key_bomb) && bomb_num > 0 && limit <= 0)
    {
        limit = deltaTime * 60;
        //因为一次按键它会读入很多下，所以限制按键的读入(即隔多少秒之后才能继续读入)
        float x = this->transform->pos().x();
        float y = this->transform->pos().y();
        //此时新建一个炸弹的对象
        auto shooter = new GameObject();
        bomb_num--;
        //qDebug() << "现在的炸弹数量为" << bomb_num;
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
        shooter->addComponent(new Shooter(this->range, 0, transform));
        //这个是调用userController的数据
        //这个炸弹也要和玩家一样具有相同的type,这样才好把分加上去
        auto trans = shooter->getComponent<Transform>();
        trans->setType(this->transform->type());
        this->attachGameObject(shooter);
        //这一步相当于把shoooter放到了gameScene上面
    }
    else
    {
        if (getKey(key_left) )
        {
           imageTransform->setImage(left_image);
           if(judge_walk(-35 * speed, 0, 1))
               vx -= 35 * speed;
           //qDebug() << "vxA = " << vx;
        }
        if (getKey(key_right) )
        {
            imageTransform->setImage(right_image);
            if(judge_walk(35 * speed, 0, 2))
                vx += 35 * speed;
            //qDebug() << "vxD = " << vx;
        }
        if (getKey(key_up) )
        {
            imageTransform->setImage(up_image);
            if(judge_walk(0, -35 * speed, 3))
                vy -= 35 * speed;
            //qDebug() << "vyW = " << vy;
        }
        if (getKey(key_down) )
        {
            imageTransform->setImage(down_image);
            if(judge_walk(0, 35 * speed, 4))
                vy += 35 * speed;
            //qDebug() << "vyS = " << vy;
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
    const float offset1 = 27;
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
        //qDebug() << "1所在的位置为" << (y + vy * 0.0166) / wall_h << ", " << (left_x + offset2) / wall_w << ",内容为" << My_map.get_map((y + vy * 0.0166) / wall_h, (left_x + offset2) / wall_w);
        //qDebug() << "2所在的位置为" << (y + vy * 0.0166) / wall_h << ", " << (right_x - offset2) / wall_w << ",内容为" << My_map.get_map((y + vy * 0.0166) / wall_h, (right_x - offset2) / wall_w);
        if(My_map.get_map((int)((y + vy * 0.0166) / wall_h), (int)((left_x + offset2) / wall_w)) >= 1 || My_map.get_map((int)((y + vy * 0.0166) / wall_h), (int)((right_x - offset2) / wall_w)) >= 1)
            return false;
        else
        {           
            judge_tool((int)((y + vy * 0.0166) / wall_h), (int)((left_x + offset2) / wall_w));
            judge_tool((int)((y + vy * 0.0166) / wall_h), (int)((right_x - offset2) / wall_w));
            return true;
        }
    }
    else
    {
        //down
        y = down_y;
        //qDebug() << "1所在的位置为" << y / wall_h << ", " << (left_x + offset2) / wall_w << ",内容为" << My_map.get_map(y / wall_h, (left_x + offset2) / wall_w);
        //qDebug() << "2所在的位置为" << y / wall_h << ", " << (right_x - offset2) / wall_w << ",内容为" << My_map.get_map(y / wall_h, (right_x - offset2) / wall_w);
        if(My_map.get_map((int)((y + vy * 0.0166) / wall_h), (int)((left_x + offset2) / wall_w)) >= 1 || My_map.get_map((int)((y + vy * 0.0166) / wall_h), (int)((right_x - offset2) / wall_w)) >= 1)
            return false;
        else
        {
            judge_tool((int)((y + vy * 0.0166) / wall_h), (int)((left_x + offset2) / wall_w));
            judge_tool((int)((y + vy * 0.0166) / wall_h), (int)((right_x - offset2) / wall_w));
            return true;
        }
    }
    float nowX = x + vx * 0.0166;
    float nowY = y + vy * 0.0166;
    int mapX = nowX / wall_w;
    int mapY = nowY / wall_h;
    //
    //qDebug() << "现在所在坐标为(" << mapY  << ',' << mapX << ")" << "， 内容为 " << My_map.get_map(mapY, mapX);
    if(My_map.get_map(mapY, mapX) >= 1)
    {
        return false;
    }
    else
    {
        judge_tool(mapY, mapX);
        return true;
    }

    //似乎好像改对了吧www太难了www
}

//int UserController::get_score()
//{
//    return this->score;
//}

//void UserController::add_score(int s)
//{
//    this->score += s;
//}

void UserController::judge_tool(int x, int y)
{
    int now = My_map.get_map(x, y);
    //qDebug() << x << "  "<< y;
    if(now == speed_tool || now == bomb_num_tool || now == range_tool)
    {
        //利用队列查找位置
        //qDebug() << tool_list.size();
        auto begin = tool_list.begin();
        for(int i = 0; i < tool_list.size(); i++)
        {
            GameObject* tool = tool_list.at(i);
            Transform* pos = tool->getComponent<Transform>();
            //qDebug() << "道具的位置是" << pos->pos().y() / 40 << "  "<< pos->pos().x() / 40;
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
            tool_speed += 8;
        }
        else if(now == bomb_num_tool)
        {
            //qDebug() << "-2";
            bomb_num += 1;
            tool_bomb_num += 8;
            qDebug() << "加完道具后的炸弹数量为" << bomb_num;
        }
        else if(now == range_tool)
        {
            //qDebug() << "-3";
            tool_range += 8;
            range += 1;
        }
    }
    My_map.set_map(x, y, 0);
}
