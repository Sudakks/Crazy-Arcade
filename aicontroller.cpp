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
#include <QList>
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
    if(tool_speed > 0)
        tool_speed -= deltaTime;
    if(tool_range > 0)
        tool_range -= deltaTime;
    if(tool_move > 0)
        tool_move -= deltaTime;
    if(tool_speed <= 0 && speed > 1)
    {
        speed -= 1;
        if(this->transform->type() == robot1_type)
            r1_tool_speed->setText(QString::number(speed));
        else
            r2_tool_speed->setText(QString::number(speed));
    }
    if(tool_range <= 0 && range > 1)
    {
        range -= 1;
        if(this->transform->type() == robot1_type)
            r1_tool_range->setText(QString::number(range));
        else
            r2_tool_range->setText(QString::number(range));
    }
    if(tool_bomb_num <= 0 && bomb_num > 1)
    {
        bomb_num -= 1;
        if(this->transform->type() == robot1_type)
            r1_tool_num->setText(QString::number(bomb_num));
        else
            r2_tool_num->setText(QString::number(bomb_num));
    }
    float X = this->transform->pos().x();
    float Y = this->transform->pos().y();
    int GeX = (Y + 15) / 40;
    int GeY = (X + 15) / 40;
    bool into1 = false;
    bool into2 = false;
    if(interval > 0)
        interval--;
    if(tool_interval > 0)
        tool_interval--;
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
        if(!pathX.empty())
        {
            pathY.clear();
            pathX.clear();
        }
        qDebug() << "正在bomb_walk";
        bomb_walk();
        //这个就属于放了炸弹，然后要判断是否规避了炸弹
    }
    else if(interval <= 0)
    {
        //这个表示到了放炸弹的时间,开始放炸弹（设计几条路线然后判断是否有余地放炸弹）
        int mapX = (X + 18) / 40;
        int mapY = (Y + 18) / 40;
        //bool flag = false;
        bool flag = true;
        if(last_dir == UP)
        {
            //case1
            //可以释放炸弹
            for(int i = 1; i <= range + 1; i++)
            {
                if(My_map.get_map(mapY - i, mapX) > 0)
                    flag = false;
            }
        }
        if(last_dir == DOWN)
        {
            //case2
            for(int i = 1; i <= range + 1; i++)
            {
                if(My_map.get_map(mapY + i, mapX) > 0)
                    flag = false;
            }
        }
        if(last_dir == LEFT)
        {
            //case3
            for(int i = 1; i <= range + 1; i++)
            {
                if(My_map.get_map(mapY, mapX - i) > 0)
                    flag = false;
            }
        }
        if(last_dir == RIGHT)
        {
            //case4
            for(int i = 1; i <= range + 1; i++)
            {
                if(My_map.get_map(mapY, mapX + i) > 0)
                    flag = false;
            }
        }
        if(flag == true)
        {
            into2 = true;
            //My_map.set_map(GeX, GeY, type_bomb);
            //表示有地方可以用来放炸弹
            whether_bomb = 1;
            bomb_time = 4 * 60;
            bomb = new GameObject();
            bomb_list.emplace_back(bomb);
            ImageTransformBuilder()
                    .setPos(QPointF(GeY * 40 +20, GeX * 40 +20))
                    .setImage(":/bomb1/image/Bomb1/1.png")
                    .setAlignment(Qt::AlignCenter)
                    .addToGameObject(bomb);
            bomb->addComponent(new Component);
            bomb->addComponent(new ImageTransform);
            bomb->addComponent(new Transform);
            bomb->addComponent(new Physics);
            auto trans = bomb->getComponent<Transform>();
            trans->setType(this->transform->type());
            bomb_x = this->transform->pos().x(), bomb_y = this->transform->pos().y();
            bomb->addComponent(new Shooter(this->range, 0, this->transform->type(), No_bomb));//这个是调用userController的数据
            this->attachGameObject(bomb);
            No_bomb++;
        }
    }
    if(into1 == false && into2 == false)
    {
        //qDebug() << "正在free_walk";
        free_walk();
    }
    X = this->transform->pos().x(), Y = this->transform->pos().y();
    GeX = (Y + 15) / 40, GeY = (X + 15) / 40;
    judge_tool(GeX, GeY);
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
    if(x >= bomb_x - 5.0000 - 30.0000 * range && x <= bomb_x + 5.0000 + 30.0000 * range + 35.0000)
        flag1 = 1;
    if(y >= bomb_y - 3.0000 - 35.0000 * range && y <= bomb_y + 3.0000 + 35.0000 + 35.0000 * range)
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
            return true;
    }
    else
    {
        //down
        y = down_y;
        if(My_map.get_map((int)((y + vy * 0.0166) / wall_h), (int)((left_x + offset2) / wall_w)) >= 1 || My_map.get_map((int)((y + vy * 0.0166) / wall_h), (int)((right_x - offset2) / wall_w)) >= 1)
            return false;
        else
            return true;
    }
    float nowX = x + vx * 0.0166;
    float nowY = y + vy * 0.0166;
    int mapX = nowX / wall_w;
    int mapY = nowY / wall_h;

    if(My_map.get_map(mapY, mapX) >= 1)
        return false;
    else
        return true;
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
    if(!pathX.empty())
    {
        //表示不为空，所以搜索到了路线
        tool_walk();
        return;
    }
    else if(tool_interval <= 0)
    {
        qDebug() << "-----";
        int flag= find_tool(0, (this->transform->pos().y() + 15) / 40, (this->transform->pos().x() + 15) / 40, 0, 0);
        if(flag == 1)
        {
            qDebug() << "路线的大小是" << pathX.size() << pathY.size();
            for(int i = 0; i < pathX.size(); i++)
            {
                qDebug() << pathX.at(i) << pathY.at(i);
            }
            tool_walk();
            return;
        }

    }
    quint32 dir = (QRandomGenerator::global()->generate()) % 4 + 1;
    //这是利用随机数版本
    if(last_dir == UP)
    {
        //up
        if(judge(0, -45 * speed, UP))
        {
            vy -= 45 * speed;
            change_image(UP);
            //qDebug() << "继续up";
            physics->setVelocity(vx, vy);
            return;
        }
    }
    else if(last_dir == LEFT)
    {
        //left
        if(judge(-45 * speed, 0, LEFT))
        {
            vx -= 45 * speed;
            change_image(LEFT);
            physics->setVelocity(vx, vy);
            return;
        }
    }
    else if(last_dir == RIGHT)
    {
        //right
        if(judge(45 * speed, 0, RIGHT))
        {
            vx += 45 * speed;
            change_image(RIGHT);
            physics->setVelocity(vx, vy);
            return;
        }
    }
    else if(last_dir == DOWN)
    {
        //down
        if(judge(0, 45 * speed, DOWN))
        {
            vy += 45 * speed;
            change_image(DOWN);
            physics->setVelocity(vx, vy);
            return;
        }
    }
    if(dir == 1)
    {
        //up
        if(judge(0, -45 * speed, UP))
        {
            vy -= 45 * speed;
            change_image(UP);
            last_dir = UP;
        }
        else
            dir++;
    }
    if(dir == 2)
    {
        //left
        if(judge(-45 * speed, 0, LEFT))
        {
            vx -= 45 * speed;
            change_image(LEFT);
            last_dir = LEFT;
        }
        else
            dir++;
    }
    if(dir == 3)
    {
        //down
        if(judge(0, 45 * speed, DOWN))
        {
            vy += 45 * speed;
            change_image(DOWN);
            last_dir = DOWN;
        }
        else
            dir++;
    }
    if(dir == 4)
    {
        //right
        if(judge(45 * speed, 0, RIGHT))
        {
            vx += 45 * speed;
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
        if(judge(0, -45 * speed, UP) && (distance(0, -45 * speed) || out_of_range(0, -45 * speed, UP)))
        {
            vy -= 45 * speed;
            change_image(UP);
            goto end;
        }
    }
    else if(last_dir == LEFT)
    {
        //left
        if(judge(-45 * speed, 0, LEFT) && ( distance(-45 * speed, 0) || out_of_range(-45 * speed, 0, LEFT)))
        {
            vx -= 45 * speed;
            change_image(LEFT);
            goto end;
        }
    }
    else if(last_dir == RIGHT)
    {
        //right
        if(judge(45 * speed, 0, RIGHT) && ( distance(45 * speed, 0) || out_of_range(45 * speed, 0, RIGHT)))
        {
            vx += 45 * speed;
            change_image(RIGHT);
            goto end;
        }
    }
    else if(last_dir == DOWN)
    {
        //down
        if(judge(0, 45 * speed, DOWN) && ( distance(0, 45 * speed) || out_of_range(0, 45 * speed, DOWN)))
        {
            vy += 45 * speed;
            change_image(DOWN);
            goto end;
        }
    }
    if(dir == 1)
    {
        //up
        if(judge(0, -45 * speed, UP) && ( distance(0, -45 * speed) || out_of_range(0, -45 * speed, UP)))
        {
            vy -= 45 * speed;
            change_image(UP);
            last_dir = UP;
        }
        else
            dir++;
    }
    if(dir == 2)
    {
        //left
        if(judge(-45 * speed, 0, LEFT) && ( distance(-45 * speed, 0) || out_of_range(-45 * speed, 0, LEFT)))
        {
            vx -= 45 * speed;
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
        if(judge(0, 45 * speed, DOWN) && ( distance(0, 45 * speed) || out_of_range(0, 45 * speed, DOWN)))
        {
            vy += 45 * speed;
            change_image(DOWN);
            last_dir = DOWN;
        }
        else
            dir++;
    }
    if(dir == 4)
    {
        //right
        if(judge(45 * speed, 0, RIGHT) && ( distance(45 * speed, 0) || out_of_range(45 * speed, 0, RIGHT)))
        {
            vx += 45 * speed;
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

void AIcontroller::judge_tool(int x, int y)
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
            My_map.set_map(x, y, 0);
            tool_interval = 4 * 60;
            if(!pathX.empty())
            {
                pathX.clear();
                pathY.clear();
            }
            break;
        }
    }
    if(now < 0)
    {
        if(now == speed_tool)
        {
            speed += 1;
            tool_speed += 8;
            if(this->transform->type() == robot1_type)
                r1_tool_speed->setText(QString::number(speed));
            else
                r2_tool_speed->setText(QString::number(speed));
        }
        else if(now == bomb_num_tool)
        {
            bomb_num += 1;
            tool_bomb_num += 8;
            if(this->transform->type() == robot1_type)
                r1_tool_num->setText(QString::number(bomb_num));
            else
                r2_tool_num->setText(QString::number(bomb_num));
        }
        else if(now == range_tool)
        {
            tool_range += 8;
            range += 1;
            if(whether_bomb == 0)
            {
                interval = 0;
            }
            if(this->transform->type() == robot1_type)
                r1_tool_range->setText(QString::number(range));
            else
                r2_tool_range->setText(QString::number(range));
        }
        else if(now == move_tool)
        {
            tool_move += 8;
        }
    }
    //My_map.set_map(x, y, 0);
}

int AIcontroller::find_tool(int step, int X, int Y, int offsetX, int offsetY)
{
    //用来递归寻找路线
    //X和Y分别表示此时的坐标
    int now = My_map.get_map(X + offsetX, Y + offsetY);
    if(now == 1 || now == 2 || now == 3)
    {
        return 0;
        //说明是完全走不通
    }
    else if(step == 7 || now == speed_tool || now == range_tool || now == bomb_num_tool || now == move_tool || (X + offsetX == GeX1 && Y + offsetY == GeY1) || (X + offsetX == GeX2 && Y + offsetY == GeY2))//这表示在最后一步或者说是在中途找到了
    {//
        if(now == range_tool || now == bomb_num_tool || now == move_tool || now == speed_tool || (X + offsetX == GeX1 && Y + offsetY == GeY1) || (X + offsetX == GeX2 && Y + offsetY == GeY2))
        {//
            //说明找到了
            //qDebug() << now;
            pathX.prepend(X + offsetX);
            pathY.prepend(Y + offsetY);
            //qDebug() << "步数为" << pathX.size();
            qDebug() << "现在的坐标是" << X + offsetX << Y + offsetY;
            //应该是能找到的
            return 1;
        }
        else
        {
            return 0;//就是说走到了最后一步也没有找到
        }
    }
    else
    {
        int flag = 0;
        if(offsetX != 1)
        {
            flag = find_tool(step + 1, X + offsetX, Y + offsetY, -1, 0);
            if(flag == 1)
            {
                //往上走，看是否可以往上走
                pathX.prepend(X + offsetX);
                pathY.prepend(Y + offsetY);
                return 1;
            }
        }
        if(offsetX != -1)
        {
            flag = find_tool(step + 1, X + offsetX, Y + offsetY, 1, 0);
            if(flag == 1)
            {
                //往下走
                pathX.prepend(X + offsetX);
                pathY.prepend(Y + offsetY);
                return 1;
            }
        }
        if(offsetY != 1)
        {
            flag = find_tool(step + 1, X + offsetX, offsetY + Y, 0, -1);
            if(flag == 1)
            {
                //向左走
                pathX.prepend(X + offsetX);
                pathY.prepend(Y + offsetY);
                return 1;
            }
        }
        if(offsetY != -1)
        {
            flag = find_tool(step + 1, X + offsetX, Y + offsetY, 0, 1);
            if(flag == 1)
            {
                //向右走
                pathX.prepend(X + offsetX);
                pathY.prepend(Y + offsetY);
                return 1;
            }
        }
        return 0;
    }
}

void AIcontroller::tool_walk()
{
    //按照递归找到的路径走就可以了,但似乎有时不太行
    double X = this->transform->pos().x();
    double Y = this->transform->pos().y();
    int GeX = pathX.at(0);
    int GeY = pathY.at(0);
    if(last_dir == UP)
    {
        X += 15;
        Y += 35;
        qDebug() << "UP";
    }
    else if(last_dir == LEFT)
    {
        Y += 18;
        X += 27;
        qDebug() << "LEFT";
    }
    else if(last_dir == DOWN)
    {
        Y += 5;
        X += 15;
        qDebug() << "DOWN";
    }
    else if(last_dir == RIGHT)
    {
        Y += 18;
        //X += 2;
        qDebug() << "RIGHT";
    }
    int nowX = Y / 40;
    int nowY = X / 40;
    if(GeX == nowX && GeY == nowY)
    {
        qDebug() << "现在所处的格子是" << GeX << GeY;
        pathX.removeAt(0);
        pathY.removeAt(0);
        //qDebug() << "越过了一个格子";
        qDebug() << "还要走" << pathX.size() << "步";
    }
    if(pathX.empty())
    {
        qDebug() << "Done";
        tool_interval = 4 * 60;
        if(whether_bomb == 0)
            interval = 0;
        return;
    }
    GeX = pathX.at(0);
    GeY = pathY.at(0);
        if(GeX - nowX == -1)
        {
            if(judge1(0, -45 * speed, UP))
            {
                qDebug() << "继续up";
                change_image(UP);
                last_dir = UP;
                physics->setVelocity(0, -45 * speed);
            }
            else
            {
                qDebug() << "还没走到就清空了";
                pathX.clear();
                pathY.clear();
            }
        }
        else if(GeX - nowX == 1)
        {
            if(judge1(0, 45 * speed, DOWN))
            {
                qDebug() << "继续down";
                change_image(DOWN);
                last_dir = DOWN;
                physics->setVelocity(0, 45 * speed);
            }
            else
            {
                qDebug() << "还没走到就清空了";
                pathX.clear();
                pathY.clear();
            }
        }
        else if(GeY - nowY == -1)
        {
            if(judge1(-45 * speed, 0, LEFT))
            {
                qDebug() << "继续left";
                change_image(LEFT);
                last_dir = LEFT;
                physics->setVelocity(-45 * speed, 0);
            }
            else
            {
                qDebug() << "还没走到就清空了";
                pathX.clear();
                pathY.clear();
            }
        }
        else if(GeY - nowY == 1)
        {
            if(judge1(45 * speed, 0, RIGHT))
            {
                qDebug() << "继续right";
                change_image(RIGHT);
                last_dir = RIGHT;
                physics->setVelocity(45 * speed, 0);
            }
            else
            {
                qDebug() << "还没走到就清空了";
                pathX.clear();
                pathY.clear();
            }
        }
}

int AIcontroller::judge1(int vx, int vy, int dir)
{
    double X = this->transform->pos().x();
    double Y = this->transform->pos().y();
    if(dir == UP)
    {
        Y += 2;
        if(My_map.get_map((Y + vy * 0.0166) / 40, (X + 5) / 40) >= 1 || My_map.get_map((Y + vy * 0.0166) / 40, (X + 25) / 40) >= 1)
            return false;
    }
    else if(dir == LEFT)
    {
        X += 2;
        if(My_map.get_map((Y + 5) / 40, (X + vx * 0.0166) / 40) >= 1 || My_map.get_map((Y + 32) / 40, (X + vx * 0.0166) / 40) >= 1)
            return false;
    }
    else if(dir == DOWN)
    {
        Y += 38;
        if(My_map.get_map((Y + vy * 0.0166) / 40, (X + 5) / 40) >= 1 || My_map.get_map((Y + vy * 0.0166) / 40, (X + 25) / 40) >= 1)
            return false;
    }
    else if(dir == RIGHT)
    {
        X += 35;
        if(My_map.get_map((Y + 5) / 40, (X + 0.0166 * vx) / 40) >= 1 || My_map.get_map((Y + 32) / 40, (X + 0.0166 * vx) / 40) >= 1)
            return false;
    }
    return true;
}
