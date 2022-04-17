#include "hitable.h"

Hitable::Hitable() {}

void Hitable::beHit() { destory(this->gameObject); }
//被撞到了就销毁
