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
#include <QRandomGenerator>

UserController::UserController (int speed, float range, int bomb_num, Qt::Key key_up, Qt::Key key_down, Qt::Key key_left, Qt::Key key_right, Qt::Key key_bomb, QString up1, QString up2, QString down1, QString down2, QString down3, QString left1, QString left2, QString left3, QString right1, QString right2, QString right3)
{
    this->speed = speed;
    this->range = range;
    this->bomb_num = bomb_num;

    this->key_up = key_up;
    this->key_down = key_down;
    this->key_left = key_left;
    this->key_right = key_right;
    this->key_bomb = key_bomb;

    this->up1 = up1, this->up2 = up2;
    this->down1 = down1, this->down2 = down2, this->down3 = down3;
    this->right1 = right1, this->right2 = right2, this->right3 = right3;
    this->left1 = left1, this->left2 = left2, this->left3 = left3;
}

void UserController::onAttach () {
    physics = this->gameObject->getComponent<Physics>();
    assert ( physics != nullptr );
    imageTransform = this->gameObject->getComponent<ImageTransform>();
    Q_ASSERT ( imageTransform != nullptr );
    transform = this->gameObject->getComponent<Transform>();
    Q_ASSERT ( transform != nullptr );
    health = this->gameObject->getComponent<Health>();
}

void UserController::onUpdate( float deltaTime ) {
    if(tool_speed > 0)
        tool_speed -= deltaTime;
    if(tool_bomb_num > 0)
        tool_bomb_num -= deltaTime;
    if(tool_range > 0)
        tool_range -= deltaTime;
    if(tool_move > 0)
        tool_move -= deltaTime;
    if(tool_speed <= 0 && speed > 1)
    {
        speed -= 1;
        if(this->transform->type() == 1)
            p1_tool_speed->setText(QString::number(speed));
        else
            p2_tool_speed->setText(QString::number(speed));
    }
    if(tool_bomb_num <= 0 && bomb_num > 1)
    {
        bomb_num -= 1;
        if(this->transform->type() == 1)
            p1_tool_num->setText(QString::number(bomb_num));
        else
            p2_tool_num->setText(QString::number(bomb_num));
    }
    if(tool_range <= 0 && range > 1)
    {
        range -= 1;
        if(this->transform->type() == 1)
            p1_tool_range->setText(QString::number(range));
        else
            p2_tool_range->setText(QString::number(range));
    }
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

            else if(tool_move > 0 && shoot->get_dir() == 0 && shoot->get_wait_time() >=50)
            {
                //说明这个炸弹还没有爆炸，但是如果人碰到了它，就要移动
                float bombX = trans->pos().x();
                float bombY = trans->pos().y();
                //哪一边碰到他
                int temp = judge_dir(bombX, bombY);
                if(temp > 0)
                {
                    shoot->enable_move(temp);
                }
            }
        }
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
        shooter->addComponent(new Physics);
        //这个好像不能用碰撞检测，因为没有item
        shooter->addComponent(new Shooter(this->range, 0, this->transform->type()));
        //这个是调用userController的数据
        //这个炸弹也要和玩家一样具有相同的type,这样才好把分加上去
        auto trans = shooter->getComponent<Transform>();
        trans->setType(this->transform->type());
        this->attachGameObject(shooter);
        //这一步相当于把shoooter放到了gameScene上面
        //给中间的炸弹也加上碰撞检测
    }
    else
    {
        if (getKey(key_left) )
        {
            left_num = (left_num + 1) % 31;
            if(left_num <= 10)
            imageTransform->setImage(left1);
            else if(left_num > 10 && left_num <= 20)
                imageTransform->setImage(left2);
            else if(left_num > 20 && left_num <= 30)
                imageTransform->setImage(left3);
           if(judge_walk(-40 * speed, 0, 1))
               vx -= 40 * speed;
           dir = LEFT;
        }
        else if (getKey(key_right) )
        {
            right_num = (right_num + 1) % 31;
            if(right_num <= 10)
            imageTransform->setImage(right1);
            else if(right_num > 10 && right_num <= 20)
                imageTransform->setImage(right2);
            else if(right_num > 20 && right_num <= 30)
                imageTransform->setImage(right3);
            if(judge_walk(40 * speed, 0, 2))
                vx += 40 * speed;
            dir = RIGHT;
        }
        else if (getKey(key_up) )
        {
            up_num = (up_num + 1) % 21;
            if(up_num <= 10)
            imageTransform->setImage(up1);
            else if(up_num > 10 && up_num <= 20)
                imageTransform->setImage(up2);
            if(judge_walk(0, -40 * speed, 3))
                vy -= 40 * speed;
            dir = UP;
        }
        else if (getKey(key_down) )
        {
            down_num = (down_num + 1) % 31;
            if(down_num <= 10)
            imageTransform->setImage(down1);
            else if(down_num > 10 && down_num <= 20)
                imageTransform->setImage(down2);
            else if(down_num > 20 && down_num <= 30)
                imageTransform->setImage(down3);
            if(judge_walk(0, 40 * speed, 4))
                vy += 40 * speed;
            dir = DOWN;
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
    const float offset2 = 5;
    //
    float left_x = this->transform->pos().x();
    float up_y = this->transform->pos().y();
    float right_x = left_x + 32;
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
        if(My_map.get_map((y + vy * 0.0166) / wall_h, (left_x + offset2) / wall_w) >= 1 || My_map.get_map((y + vy * 0.0166) / wall_h, (right_x - offset2) / wall_w) >= 1)
            return false;
        else
        {           
            judge_tool((y + vy * 0.0166) / wall_h, (left_x + offset2) / wall_w);
            judge_tool((y + vy * 0.0166) / wall_h, (right_x - offset2) / wall_w);
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

void UserController::judge_tool(int x, int y)
{
    int now = 0;
    auto begin = tool_list.begin();
    for(int i = 0; i < tool_list.size(); i++)
    {
        GameObject* tool = tool_list.at(i);
        auto pos = tool->getComponent<Transform>();
        if((int)(pos->pos().y() / 40) == x && (int)(pos->pos().x() / 40) == y)
        {
            //说明找到了这一个道具
            //直接消除这一个gameObject
            now = pos->type();
            tool_list.erase(begin + i);//先从队列中移走
            this->detachGameObject(tool);
            break;
        }
    }
    if(now < 0)
    {
        if(now == speed_tool)
        {
            speed += 1;
            tool_speed += 8;
            if(this->transform->type() == 1)
                p1_tool_speed->setText(QString::number(speed));
            else
                p2_tool_speed->setText(QString::number(speed));
        }
        else if(now == bomb_num_tool)
        {
            bomb_num += 1;
            tool_bomb_num += 8;
            if(this->transform->type() == 1)
                p1_tool_num->setText(QString::number(bomb_num));
            else
                p2_tool_num->setText(QString::number(bomb_num));
        }
        else if(now == range_tool)
        {
            tool_range += 8;
            range += 1;
            if(this->transform->type() == 1)
                p1_tool_range->setText(QString::number(range));
            else
                p2_tool_range->setText(QString::number(range));
        }
        else if(now == move_tool)
        {
            tool_move += 8;
        }
    }
    My_map.set_map(x, y, 0);
}

int UserController::judge_dir(float bombX, float bombY)
{
    float leftx = this->transform->pos().x();
    float upy = this->transform->pos().y();
    float rightx = leftx + 33;
    float downy = upy + 40;
    int GeY = bombX / 40, GeX = bombY / 40;
    int GeUp = upy / 40, GeDown = downy / 40, GeLeft = leftx / 40, GeRight = rightx / 40;
    //判断是哪个方向碰到
    if(dir == LEFT)
    {
        if(GeLeft == GeY && (GeUp == GeX || GeDown == GeX))
        {
            return LEFT;
        }
    }
    else if(dir == RIGHT)
    {
        if(GeRight == GeY && (GeUp == GeX || GeDown == GeX))
        {
            return RIGHT;
        }
    }
    else if(dir == DOWN)
    {
        if(GeDown == GeX && (GeLeft == GeY || GeRight == GeY))
        {
            return DOWN;
        }
    }
    else
    {
        if(GeUp == GeX && (GeLeft == GeY || GeRight == GeY))
        {
            return UP;
        }
    }
    return 0;//return 0表示没有碰到
}
