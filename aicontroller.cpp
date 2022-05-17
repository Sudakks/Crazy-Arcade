#include "aicontroller.h"
#include <QRandomGenerator>
#include "common.h"
#include "imagetransform.h"
#include "transform.h"
#include "transformbuilder.h"
#include "shooter.h"
#include <QDebug>
#include "ammo.h"
#include <QString>

AIcontroller::AIcontroller(QString up1, QString up2, QString down1, QString down2, QString left1, QString left2, QString right1, QString right2)
{
    this->up1 = up1, this->up2 = up2;
    this->down1 = down1, this->down2 = down2;
    this->left1 = left1, this->left2 = left2;
    this->right1 = right1, this->right2 = right2;
}

void AIcontroller::onAttach()
{
    physics = this->gameObject->getComponent<Physics>();
    assert ( physics != nullptr );
    imageTransform = this->gameObject->getComponent<ImageTransform>();
    Q_ASSERT ( imageTransform != nullptr );
    transform = this->gameObject->getComponent<Transform>();
    Q_ASSERT ( transform != nullptr );
}

void AIcontroller::onUpdate(float deltaTime)
{
    //这个是实现机器人走动和放炸弹的update
    //约定进一次update走一次，但是是间隔一段时间后才会放炸弹
    //就利用随机数判断是否可以走
    //但是当放炸弹时，就要先找到一条可以走的路，并记录下炸弹的位置，要一直保证不会被炸弹炸到
    float X = this->transform->pos().x();
    float Y = this->transform->pos().y();
    bool into1 = false;
    bool into2 = false;
    if(interval > 0)
        interval--;
    float vx = 0, vy = 0;
    if(whether_bomb == 1)
    {
        into1 = true;
        bomb_time--;
        if(bomb_time == 0)//这个就表示炸弹爆炸结束了
        {
            interval = 12 * 60;
            whether_bomb = 0;
        }
        bomb_walk();
        //这个就属于放了炸弹，然后要判断是否规避了炸弹
    }
    else if(interval <= 0)
    {
        //qDebug() << "判断放炸弹";
        //这个表示到了放炸弹的时间,开始放炸弹（设计几条路线然后判断是否有余地放炸弹）
        int mapX = (X + 18) / 40;
        int mapY = (Y + 18) / 40;
        bool flag = false;
        if(last_dir == UP && My_map.get_map(mapY - 1, mapX) <= 0 && My_map.get_map(mapY - 2, mapX) <= 0)
        {
            //case1
            //可以释放炸弹
            flag = true;
        }
        else if(last_dir == DOWN && My_map.get_map(mapY + 1, mapX) <= 0 && My_map.get_map(mapY + 2, mapX) <= 0)
        {
            //case2
            flag = true;
        }
        else if(last_dir == LEFT && My_map.get_map(mapY, mapX - 1) <= 0 && My_map.get_map(mapY, mapX - 2) <= 0)
        {
            //case3
            flag = true;
        }
        else if(last_dir == RIGHT && My_map.get_map(mapY, mapX + 1) <= 0 && My_map.get_map(mapY, mapX + 2) <= 0)
        {
            //case4
            flag = true;
        }
        if(flag == true)
        {
            into2 = true;
            //表示有地方可以用来放炸弹
            whether_bomb = 1;
            bomb_time = 4 * 60;
            bomb = new GameObject();
            bomb_list.emplace_back(bomb);
            ImageTransformBuilder()
                    .setPos(QPointF(X + 20, Y + 20))
                    .setImage(":/bomb1/image/Bomb1/1.png")
                    .setAlignment(Qt::AlignCenter)
                    .addToGameObject(bomb);
            bomb->addComponent(new Component);
            bomb->addComponent(new ImageTransform);
            bomb->addComponent(new Transform);
            bomb->addComponent(new Physics);
            auto trans = bomb->getComponent<Transform>();
            trans->setType(this->transform->type());
            //qDebug() << this->transform->type();
            bomb_x = this->transform->pos().x(), bomb_y = this->transform->pos().y();
            bomb->addComponent(new Shooter(this->range, 0, this->transform->type()));//这个是调用userController的数据
            this->attachGameObject(bomb);
        }
    }
    if(into1 == false && into2 == false)
    {
        free_walk();
    }
}

bool AIcontroller::out_of_range(double vx, double vy, int dir)
{
    //这个是用来判断机器人是否走到了炸弹的范围里
    double x = this->transform->pos().x() + 0.0166 * vx;
    double y = this->transform->pos().y() + 0.0166 * vy;
    if(dir == DOWN)
        y += 38.0000;
    else if(dir == RIGHT)
        x += 32.0000;
    //炸弹的尺寸大概35左右
    int flag1 = 0, flag2 = 0;//分别表示x和y上的
    if(x >= bomb_x - 5.0000 - 30.0000 && x <= bomb_x + 5.0000 + 30.0000 + 35.0000)
        flag1 = 1;
    if(y >= bomb_y - 3.0000 - 35.0000 && y <= bomb_y + 3.0000 + 35.0000 + 35.0000)
        flag2 = 1;
    if(flag1 && flag2)
        return false;
    return true;
}

bool AIcontroller::judge(float vx, float vy, int dir)
{
    //这个就是判断机器人是否能走(欸还是要分方向的)
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
    float right_x = left_x + 40;
    float down_y = up_y + 40;
    float half_y = (up_y + down_y) / 2;
    //
    int up_Y = up_y / wall_h;
    int half_Y = half_y / wall_h;
    if(dir == LEFT)
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
    else if(dir == RIGHT)
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
    else if(dir == UP)
    {
        //up
        y = up_y;
        if(My_map.get_map((int)((y + vy * 0.0166) / wall_h), (int)((left_x + offset2) / wall_w)) >= 1 || My_map.get_map((int)((y + vy * 0.0166) / wall_h), (int)((right_x - offset2) / wall_w)) >= 1)
            return false;
        else
        {
            return true;
        }
    }
    else
    {
        //down
        y = down_y;
        if(My_map.get_map((int)((y + vy * 0.0166) / wall_h), (int)((left_x + offset2) / wall_w)) >= 1 || My_map.get_map((int)((y + vy * 0.0166) / wall_h), (int)((right_x - offset2) / wall_w)) >= 1)
            return false;
        else
        {
            return true;
        }
    }
    float nowX = x + vx * 0.0166;
    float nowY = y + vy * 0.0166;
    int mapX = nowX / wall_w;
    int mapY = nowY / wall_h;

    if(My_map.get_map(mapY, mapX) >= 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool AIcontroller::distance(double vx, double vy)
{
    //这个用来判断机器人是否与炸弹的范围越来越远
    double x = this->transform->pos().x() + 0.0001;
    double y = this->transform->pos().y() + 0.0001;
    if(fabs(x - bomb_x) < fabs(x + vx * 0.0166 - bomb_x) || fabs(y - bomb_y) < fabs(y + vy * 0.0166 - bomb_y))
    {
        return true;
    }
    return false;
}

void AIcontroller::free_walk()
{
    float vx = 0, vy = 0;
    //qDebug() << "------------";
    //qDebug() << "free walk";
    quint32 dir = (QRandomGenerator::global()->generate()) % 4 + 1;
    //这是利用随机数版本
    if(last_dir == UP)
    {
        //up
        if(judge(0, -35 * speed, UP))
        {
            vy -= 35 * speed;
            change_image(UP);
            //qDebug() << "继续up";
            physics->setVelocity(vx, vy);
            return;
        }
    }
    else if(last_dir == LEFT)
    {
        //left
        if(judge(-35 * speed, 0, LEFT))
        {
            vx -= 35 * speed;
            change_image(LEFT);
            physics->setVelocity(vx, vy);
            return;
        }
    }
    else if(last_dir == RIGHT)
    {
        //right
        if(judge(35 * speed, 0, RIGHT))
        {
            vx += 35 * speed;
            change_image(RIGHT);
            physics->setVelocity(vx, vy);
            return;
        }
    }
    else if(last_dir == DOWN)
    {
        //down
        if(judge(0, 35 * speed, DOWN))
        {
            vy += 35 * speed;
            change_image(DOWN);
            physics->setVelocity(vx, vy);
            return;
        }
    }
    if(dir == 1)
    {
        //up
        if(judge(0, -35 * speed, UP))
        {
            vy -= 35 * speed;
            change_image(UP);
            last_dir = UP;
        }
        else
            dir++;
    }
    if(dir == 2)
    {
        //left
        if(judge(-35 * speed, 0, LEFT))
        {
            vx -= 35 * speed;
            change_image(LEFT);
            last_dir = LEFT;
        }
        else
            dir++;
    }
    if(dir == 3)
    {
        //down
        if(judge(0, 35 * speed, DOWN))
        {
            vy += 35 * speed;
            change_image(DOWN);
            last_dir = DOWN;
        }
        else
            dir++;
    }
    if(dir == 4)
    {
        //right
        if(judge(35 * speed, 0, RIGHT))
        {
            vx += 35 * speed;
            change_image(RIGHT);
            last_dir = RIGHT;
        }
    }
    physics->setVelocity(vx, vy);
}

void AIcontroller::bomb_walk()
{
    //这个是在有炸弹的时候的行走
    float vx = 0, vy = 0;
    //规避炸弹再结合judge的逻辑进行判断
    quint32 dir = (QRandomGenerator::global()->generate()) % 4 + 1;
    if(last_dir == UP)
    {
        if(judge(0, -35 * speed, UP) && (distance(0, -35 * speed) || out_of_range(0, -35 * speed, UP)))
        {
            vy -= 35 * speed;
            change_image(UP);
            goto end;
        }
    }
    else if(last_dir == LEFT)
    {
        //left
        if(judge(-35 * speed, 0, LEFT) && ( distance(-35 * speed, 0) || out_of_range(-35 * speed, 0, LEFT)))
        {
            vx -= 35 * speed;
            change_image(LEFT);
            goto end;
        }
    }
    else if(last_dir == RIGHT)
    {
        //right
        if(judge(35 * speed, 0, RIGHT) && ( distance(35 * speed, 0) || out_of_range(35 * speed, 0, RIGHT)))
        {
            vx += 35 * speed;
            change_image(RIGHT);
            goto end;
        }
    }
    else if(last_dir == DOWN)
    {
        //down
        if(judge(0, 35 * speed, DOWN) && ( distance(0, 35 * speed) || out_of_range(0, 35 * speed, DOWN)))
        {
            vy += 35 * speed;
            change_image(DOWN);
            goto end;
        }
    }
    if(dir == 1)
    {
        //up
        if(judge(0, -35 * speed, UP) && ( distance(0, -35 * speed) || out_of_range(0, -35 * speed, UP)))
        {
            vy -= 35 * speed;
            change_image(UP);
            last_dir = UP;
        }
        else
            dir++;
    }
    if(dir == 2)
    {
        //left
        if(judge(-35 * speed, 0, LEFT) && ( distance(-35 * speed, 0) || out_of_range(-35 * speed, 0, LEFT)))
        {
            vx -= 35 * speed;
            change_image(LEFT);
            //qDebug() << "最后是left";
            last_dir = LEFT;
        }
        else
            dir++;
    }
    if(dir == 3)
    {
        //down
        if(judge(0, 35 * speed, DOWN) && ( distance(0, 35 * speed) || out_of_range(0, 35 * speed, DOWN)))
        {
            vy += 35 * speed;
            change_image(DOWN);
            last_dir = DOWN;
        }
        else
            dir++;
    }
    if(dir == 4)
    {
        //right
        if(judge(35 * speed, 0, RIGHT) && ( distance(35 * speed, 0) || out_of_range(35 * speed, 0, RIGHT)))
        {
            vx += 35 * speed;
            change_image(RIGHT);
            last_dir = RIGHT;
        }
    }

    end: physics->setVelocity(vx, vy);
}

void AIcontroller::change_image(int dir)
{
    if(dir == UP)
    {
        //up
        up_num = (up_num + 1) % 40 + 1;
        if(up_num <= 20)
            this->imageTransform->setImage(up1);
        else
            this->imageTransform->setImage(up2);
    }
    else if(dir == LEFT)
    {
        //left
        left_num = (left_num + 1) % 40 + 1;
        if(left_num <= 20)
            this->imageTransform->setImage(left1);
        else
            this->imageTransform->setImage(left2);
    }
    else if(dir ==  DOWN)
    {
        //down
        down_num = (down_num + 1) % 40 + 1;
        if(down_num <= 20)
            this->imageTransform->setImage(down1);
        else
            this->imageTransform->setImage(down2);
    }
    else
    {
        //right 4
        right_num = (right_num + 1) % 40 + 1;
        if(right_num <= 20)
            this->imageTransform->setImage(right1);
        else
            this->imageTransform->setImage(right2);
    }
}
