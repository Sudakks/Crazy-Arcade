#ifndef AMMO_H_
#define AMMO_H_

#include <component.h>
#include <gameobject.h>
#include <transform.h>
//弹药
//将炸弹的冲击加入这个属性，这样的话就能检测到与它碰撞的物体
class Ammo : public Component {
 public:
  explicit Ammo(int type, int No);//不需要速度，但是需要冲击波的范围

  void onAttach() override;
  void onUpdate(float deltaTime) override;
  void set_collider(QGraphicsItem * co);
  void judge_type(Transform* transform, GameObject* gameObject);
  //用来判断类型然后更新画面的

 protected:
  Transform *transform = nullptr;
  QGraphicsItem *collider = nullptr;
  float timeToLive = 61;//这个是炸弹显示的时间（即等待爆炸的时间）
  QGraphicsItem* flash;//这个表示附着在其上的图片（利用这个找到对应的图片然后进行更换）
  int type;
  int No;
};

#endif  // AMMO_H_
