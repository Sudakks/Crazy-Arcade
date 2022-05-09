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
int player1_type = 1;
int player2_type = 2;
int robot1_type = -1;
int robot2_type = -2;
QList<GameObject*> bomb_list;//即对于地图上的所有炸弹都是这样处理的
int p1_score = 0;
int p2_score = 0;
int UP = 1, DOWN = 2, LEFT = 3, RIGHT = 4;
//这个是用来挂各种item类的图片
