#ifndef COMMON_H
#define COMMON_H
#include "map.h"
#include "gameobject.h"
#include "gamescene.h"
#include "transform.h"
#include "mainwindow.h"
#include <QTimer>
#include <QGraphicsSimpleTextItem>
extern int soft_score;
extern int robot_score;
extern int player_score;
extern map My_map;
extern int speed_tool;
extern int range_tool;
extern int bomb_num_tool;
extern int move_tool;
extern GameObject* common_obj;
extern QList<GameObject*> tool_list;//用来放道具的队列，即存了已经炸出来的道具
extern int player1_type;
extern int player2_type;
extern int robot1_type;
extern int robot2_type;
extern QList<GameObject*> bomb_list;//里面装的是指针
extern int p1_score;
extern int p2_score;
extern int r1_score;
extern int r2_score;
extern int UP, DOWN, LEFT, RIGHT;
extern int win;
extern int No_bomb;
extern int type_bomb;//似乎是为了机器人躲开炸弹而设计的
extern int type_tool;
extern int type_player;
extern int GeX1;//分别表示玩家现在所处的位置
extern int GeY1;
extern int GeX2;
extern int GeY2;

extern QGraphicsSimpleTextItem *score1;
extern QGraphicsSimpleTextItem *score2;
extern QGraphicsSimpleTextItem *live1;
extern QGraphicsSimpleTextItem *live2;
extern QGraphicsSimpleTextItem *p1_tool_num;
extern QGraphicsSimpleTextItem *p2_tool_num;
extern QGraphicsSimpleTextItem *p1_tool_speed;
extern QGraphicsSimpleTextItem *p2_tool_speed;
extern QGraphicsSimpleTextItem *p1_tool_range;
extern QGraphicsSimpleTextItem *p2_tool_range;
extern QGraphicsSimpleTextItem *score_r1;
extern QGraphicsSimpleTextItem *score_r2;
extern QGraphicsSimpleTextItem *r1_tool_num;
extern QGraphicsSimpleTextItem *r2_tool_num;
extern QGraphicsSimpleTextItem *r1_tool_speed;
extern QGraphicsSimpleTextItem *r2_tool_speed;
extern QGraphicsSimpleTextItem *r1_tool_range;
extern QGraphicsSimpleTextItem *r2_tool_range;
#endif // COMMON_H
