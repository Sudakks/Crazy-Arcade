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
extern int UP, DOWN, LEFT, RIGHT;
extern int win;

extern QGraphicsSimpleTextItem *score1;
extern QGraphicsSimpleTextItem *score2;
extern QGraphicsSimpleTextItem *live1;
extern QGraphicsSimpleTextItem *live2;
#endif // COMMON_H
