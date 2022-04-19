#ifndef SHOOTER_H_
#define SHOOTER_H_

#include "component.h"
#include "imagetransform.h"

class Transform;
//将这个当作炸弹的类
class Shooter : public Component {
 public:
  Shooter(float range, float wait_time);

  void onAttach() override;
  void onUpdate(float deltaTime) override;

 protected:
  Transform *transform = nullptr;
  ImageTransform * imageTransform = nullptr;

  //const float interval = .5;
  //float cooldown = interval;
  float wait_time;//这个从0开始，达到某个deltaTime时，发生爆炸
  int range;//默认范围是1.5个墙砖那么远

public:
  void set_wait_time(float t);
  float get_wait_time();
  void set_range(int r);
  float get_range();
};

#endif  // SHOOTER_H_
