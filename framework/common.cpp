#include "common.h"
int soft_score = 2;
int robot_score = 7;
int player_score = 10;
map My_map;
int speed_tool = -1;
int range_tool = -2;
int bomb_num_tool = -3;
GameObject* common_obj = new GameObject();
QList<GameObject*> tool_list;
//这个是用来挂各种item类的图片
