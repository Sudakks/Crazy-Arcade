#ifndef SHOOTER_H_
#define SHOOTER_H_

#include "component.h"
#include "imagetransform.h"
#include "usercontroller.h"
#include <QList>

class Transform;
//将这个当作炸弹的类
class Shooter : public Component {
 public:
  Shooter(float range, float wait_time, int  userType, int No);

  void onAttach() override;
  void onUpdate(float deltaTime) override;

 protected:
  Transform *transform = nullptr;
  ImageTransform * imageTransform = nullptr;
  Physics *physics = nullptr;
  float wait_time;//这个从0开始，达到某个deltaTime时，发生爆炸
  //分为两个阶段，炸弹等待爆炸的阶段以及炸弹爆炸的阶段
  int range;//默认范围是1个墙砖那么远
  int score = 0;//即这个炸弹贡献了多少分
  QList<QGraphicsPixmapItem*> flash_list;
  QList<GameObject*> ammo_list;
  int type;
  int move = 0;
  int dir = 0;//这个表示炸弹要一直往哪个方向移动
  int No;

public:
  void set_wait_time(float t);
  float get_wait_time();

  void set_range(int r);
  float get_range();

  int get_score();
  int random_tool();//用这个来随机生成道具

  void change_map(int x, int y);//获得道具后改变地图
  void develop_bomb(float offsetX, float offsetY);

  void enable_move(int dir);//使炸弹能够动起来
  int get_dir();
  bool judge_move();
};

#endif  // SHOOTER_H_
