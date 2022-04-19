#ifndef AMMO_H_
#define AMMO_H_

#include <component.h>
#include <gameobject.h>
#include <transform.h>
//弹药
//将炸弹的冲击加入这个属性，这样的话就能检测到与它碰撞的物体
class Ammo : public Component {
 public:
  explicit Ammo(float range);//不需要速度，但是需要冲击波的范围

  void onAttach() override;
  void onUpdate(float deltaTime) override;
  Transform * get_transform();
  void set_collider(QGraphicsItem * co);

 protected:
  Transform *transform = nullptr;
  QGraphicsItem *collider = nullptr;
  float range;
  float timeToLive = 1;//这个是炸弹显示的时间（即等待爆炸的时间）
  //QPointF velocity;
};

#endif  // AMMO_H_
