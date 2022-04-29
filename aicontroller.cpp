#include "aicontroller.h"
#include <QRandomGenerator>
#include "common.h"
#include "imagetransform.h"
#include "transform.h"
#include "transformbuilder.h"
#include "shooter.h"
#include <QDebug>
AIcontroller::AIcontroller(QString up, QString down, QString left, QString right)
{
    this->up_image = up,
    this->down_image = down,
    this->left_image = left,
    this->right_image = right;
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
    if(bomb_waiting > 0)
        bomb_waiting--;
    float vx = 0, vy = 0;
    /*if(!bombX && !bombY)
    {
        bomb_walk();
        //这个就属于放了炸弹，然后要判断是否规避了炸弹
    }
    else if(bomb_num > 0 && bomb_waiting <= 0 && whether_bomb == 0)
    {
        //这个表示到了放炸弹的时间,开始放炸弹（设计几条路线然后判断是否有余地放炸弹）
        int mapX = X / 40;
        int mapY = Y / 40;
        bool flag = false;
        if(My_map.get_map(mapY - 1, mapX) <= 0 && My_map.get_map(mapY - 2, mapX) <= 0)
        {
            //case1
            //可以释放炸弹
            flag = true;
        }
        else if(My_map.get_map(mapY + 1, mapX) <= 0 && My_map.get_map(mapY + 2, mapX) <= 0)
        {
            //case2
            flag = true;
        }
        else if(My_map.get_map(mapY, mapX - 1) <= 0 && My_map.get_map(mapY, mapX - 2) <= 0)
        {
            //case3
            flag = true;
        }
        else if(My_map.get_map(mapY, mapX + 1) <= 0 && My_map.get_map(mapY, mapX + 2) <= 0)
        {
            //case4
            flag = true;
        }
        else if(My_map.get_map(mapY - 1, mapX) <= 0 && My_map.get_map(mapY - 1, mapX + 1) <= 0)
        {
            //case5(上+右)
            flag = true;
        }
        else if(My_map.get_map(mapY + 1, mapX) <= 0 && My_map.get_map(mapY + 1, mapX - 1) <= 0)
        {
            //case6(下+左)
            flag = true;
        }
        else if(My_map.get_map(mapY, mapX - 1) <= 0 && My_map.get_map(mapY - 1, mapX - 1) <= 0)
        {
            //case7(左+上)
            flag = true;
        }
        else if(My_map.get_map(mapY, mapX + 1) <= 0 && My_map.get_map(mapY - 1, mapX + 1) <= 0)
        {
            //case8(右+上)
            flag = true;
        }
        if(flag == true)
        {
            //表示有地方可以用来放炸弹
            bombX = X, bombY = Y;
            bomb_waiting = 60 * 7;
            bomb = new GameObject();
            bomb_list.emplace_back(bomb);
            ImageTransformBuilder()
                    .setPos(QPointF(X + 20, X + 20))
                    .setImage(":/bomb1/image/Bomb1/1.png")
                    .setAlignment(Qt::AlignCenter)
                    .addToGameObject(bomb);
            bomb->addComponent(new Component);
            bomb->addComponent(new ImageTransform);
            bomb->addComponent(new Transform);
            bomb->addComponent(new Shooter(this->range, 0, this->transform));//这个是调用userController的数据
            this->attachGameObject(bomb);
        }
    }
    else if(whether_bomb == 0)
    {*/
        free_walk();
    //}
}

bool AIcontroller::out_of_range(int x, int y)
{
    //这个是用来判断机器人是否走到了炸弹的范围里
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    if(x == bombX && y == bombY)
        return false;
    for(int i = 0; i < 4; i++)
    {
        //这个判断它是否走到了有火焰的地方
        if(My_map.get_map(bombY + dy[i], bombX + dx[i]) <= 0)
        {
            if(x != bombX + dx[i] && y != bombY + dy[i])
            {
                continue;
            }
            else
                return false;
        }
    }
    return true;
}

bool AIcontroller::judge(float vx, float vy, int dir)
{
    //这个就是判断机器人是否能走(欸还是要分方向的)
   /* float deltaTime = 1 / 60;
    float x = this->transform->pos().x();
    float y = this->transform->pos().y();
    float offset = 5;
    float offset1 = 20;
    float offset2 = 10;

    if(dir == 1)
    {
        //up
        qDebug() << "up";
        y = y + vy * deltaTime;
        qDebug() << "格子分别是: " << (int)y / 40 << (int)(x + offset) / 40 << "和 " << (int)y / 40 << (int)(x + 40 - offset) / 40;
        qDebug() << "map:  " << My_map.get_map(y / 40, (x + offset) / 40) << My_map.get_map(y / 40, (x + 40 - offset) / 40);
        if(My_map.get_map(y / 40, (x + offset) / 40) > 0 || My_map.get_map(y / 40, (x + 40 - offset) / 40 > 0))
        {
            return false;
        }
        return true;
    }
    else if(dir == 3)
    {
        //down
        qDebug() << "down";
        y += 40;
        y = y + vy * deltaTime ;
        qDebug() << "格子分别是: " << (int)y / 40 << (int)(x + offset) / 40 << "和 " << (int)y / 40 << (int)(x + 40 - offset) / 40;
        qDebug() << "map:  " << My_map.get_map(y / 40, (x + offset) / 40) << My_map.get_map(y / 40, (x + 40 - offset) / 40);
        if(My_map.get_map(y / 40, (x + offset) / 40) > 0 || My_map.get_map(y / 40, (x + 40 - offset) / 40 > 0))
        {
            return false;
        }
        return true;
    }
    else if(dir == 2)
    {
        //left
        qDebug() << "left";
        x = x + vx * deltaTime ;
        if((y + 20) / 40 == y / 40)
        {
            //以上面为主
            y += offset1;
        }
        else
        {
            y += 40;
        }
        qDebug() << "格子是: " << (int)y / 40 << (int)x / 40;
        qDebug() << "map:  " << My_map.get_map(y / 40, x / 40);
        if(My_map.get_map(y / 40, x / 40) > 0)
            return false;
        return true;
    }
    else
    {
        //right
        qDebug() << "right";
        x += 40;
        x += vx * deltaTime;
        if((y + 20) / 40 == y / 40)
        {
            y += offset1;
        }
        else
        {
            y += 40;
        }
        qDebug() << "格子是： " << (int)y / 40 << (int)x / 40;
        qDebug() << My_map.get_map(y / 40, x / 40);
        if(My_map.get_map(y / 40, x / 40) > 0)
            return false;
        else
            return true;
    }*/
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
    if(dir == 2)
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
    else if(dir == 4)
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
    else if(dir == 1)
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
//
/*void AIcontroller::update_image(int dirX, int dirY)
{
    if(dirX == 1)
    {
        //right
        imageTransform->setImage(right_image);
    }
    else if(dirX == -1)
    {
        //left
        imageTransform->setImage(left_image);
    }
    else if(dirY == 1)
    {
        //down
        imageTransform->setImage(down_image);
    }
    else if(dirY == -1)
    {
        //up
        imageTransform->setImage(up_image);
    }
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
    if(dir == 2)
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
    else if(dir == 4)
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
    else if(dir == 1)
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
*/
void AIcontroller::free_walk()
{
    float vx = 0, vy = 0;
    qDebug() << "------------";
    qDebug() << "free walk";
    quint32 dir = (QRandomGenerator::global()->generate()) % 4 + 1;
    //这是利用随机数版本
    if(last_dir == 1)
    {
        //up
        if(judge(0, -30 * speed, 1))
        {
            vy -= 30 * speed;
            qDebug() << "继续up";
            imageTransform->setImage(up_image);
            goto end;
        }
    }
    else if(last_dir == 2)
    {
        //left
        if(judge(-30 * speed, 0, 2))
        {
            vx -= 30 * speed;
            qDebug() << "继续left";
            imageTransform->setImage(left_image);
            goto end;
        }
    }
    else if(last_dir == 4)
    {
        //right
        if(judge(30 * speed, 0, 4))
        {
            vx += 30 * speed;
            qDebug() << "继续right";
            imageTransform->setImage(right_image);
            goto end;
        }
    }
    else if(last_dir == 3)
    {
        //down
        if(judge(0, 30 * speed, 3))
        {
            vy += 30 * speed;
            qDebug() << "继续down";
            imageTransform->setImage(down_image);
            goto end;
        }
    }

    if(dir == 1)
    {
        //up
        if(judge(0, -30 * speed, 1))
        {
            vy -= 30 * speed;
            imageTransform->setImage(up_image);
            qDebug() << "最后是up";
            last_dir = 1;
        }
        else
            dir++;
    }
    if(dir == 2)
    {
        //left
        if(judge(-30 * speed, 0, 2))
        {
            vx -= 30 * speed;
            imageTransform->setImage(left_image);
            qDebug() << "最后是left";
            last_dir = 2;
        }
        else
            dir++;
    }
    if(dir == 3)
    {
        //down
        if(judge(0, 30 * speed, 3))
        {
            vy += 30 * speed;
            imageTransform->setImage(down_image);
            qDebug() << "最后是down";
            last_dir = 3;
        }
        else
            dir++;
    }
    if(dir == 4)
    {
        //right
        if(judge(30 * speed, 0, 4))
        {
            vx += 30 * speed;
            imageTransform->setImage(right_image);
            qDebug() << "最后是right";
            last_dir = 4;
        }
    }
    //
    end: physics->setVelocity(vx, vy);
    qDebug() << vx << vy;
}

void AIcontroller::bomb_walk()
{
    //这个是在有炸弹的时候的行走
    float X = this->transform->pos().x();
    float Y = this->transform->pos().y();
    //up, left, down, right
    //规避炸弹再结合free_walk的逻辑进行判断
}
