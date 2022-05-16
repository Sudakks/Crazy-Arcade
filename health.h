#ifndef HEALTH_H_
#define HEALTH_H_

#include "hitable.h"

class Health : public Hitable {
 public:
  explicit Health(int health);

  void beHit() override;
  void set_hit(int val);

 protected:
  int health;
  int have_hit = false;
};

#endif  // HEALTH_H_
