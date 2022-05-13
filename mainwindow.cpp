#include "mainwindow.h"
#include <gameobject.h>
#include <imagetransform.h>
#include <shooter.h>
#include <summondummy.h>
#include <transformbuilder.h>
#include "./ui_mainwindow.h"
#include <QGraphicsEllipseItem>
#include <QPainter>
#include "physics.h"
#include "usercontroller.h"
#include "map.h"
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QDebug>
#include "hitable.h"
#include <QRandomGenerator>
#include "imagetransform.h"
#include "common.h"
#include "aicontroller.h"
#include "health.h"

void loadScene(GameScene *gameScene) {
  //加载地图图片
  My_map.init_Map();
  //上面的围栏
  for(int i = 0; i < 3; i++)
  {
      auto wall = new GameObject();
      ImageTransformBuilder()
              .setPos(QPointF(40+240*i, 0))
              .setAlignment(Qt::AlignLeft | Qt::AlignTop)
              .setImage("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Map/side1")
              .addToGameObject(wall);
      gameScene->attachGameObject(wall);
  }
  //左侧的围栏
  for(int i = 0; i < 3; i++)
  {
      auto wall = new GameObject();
      ImageTransformBuilder()
              .setPos(QPointF(0, 200*i))
              .setAlignment(Qt::AlignLeft | Qt::AlignTop)
              .setImage("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Map/side11")
              .addToGameObject(wall);
      gameScene->attachGameObject(wall);
  }
  for(int i = 1; i < 14; i++)
  {
      for(int j = 1; j < 19; j++)
      {
            if(My_map.get_map(i,j))
            {
                auto wall = new GameObject();
                if(My_map.get_map(i,j) == 1 && i % 2 == 0)
                {
                    ImageTransformBuilder()
                            .setPos(QPointF(40*j, 40*i))
                            .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                            .setImage("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Map/hard1")
                            .addToGameObject(wall);                     
                }
                else if(My_map.get_map(i,j) == 1 && i % 2 == 1)
                {

                    ImageTransformBuilder()
                            .setPos(QPointF(40*j, 40*i))
                            .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                            .setImage("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Map/hard2")
                            .addToGameObject(wall);
                }
                else if(My_map.get_map(i,j) == 2 && j % 2 == 1)
                {
                    ImageTransformBuilder()
                            .setPos(QPointF(40*j, 40*i))
                            .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                            .setImage("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Map/soft1")
                            .addToGameObject(wall);
                    wall->addComponent(new Hitable);
                }
                else if(My_map.get_map(i,j) == 2 && j % 2 == 0)
                {
                    ImageTransformBuilder()
                            .setPos(QPointF(40*j, 40*i))
                            .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                            .setImage("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Map/soft2")
                            .addToGameObject(wall);
                    wall->addComponent(new Hitable);
                }
                gameScene->attachGameObject(wall);
            }
      }
      //下面的围栏
      for(int i = 0; i < 3; i++)
      {
          auto wall = new GameObject();
          ImageTransformBuilder()
                  .setPos(QPointF(40+240*i, 40*14))
                  .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                  .setImage("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Map/side2")
                  .addToGameObject(wall);
          gameScene->attachGameObject(wall);
      }
      //右边的围栏
      for(int i = 0; i < 3; i++)
      {
          auto wall = new GameObject();
          ImageTransformBuilder()
                  .setPos(QPointF(19*40, 200*i))
                  .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                  .setImage("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Map/side22")
                  .addToGameObject(wall);
          gameScene->attachGameObject(wall);
      }
  }
  auto wall = new GameObject();
  ImageTransformBuilder()
          .setPos(QPointF(8*40,6*40))
          .setAlignment(Qt::AlignLeft | Qt::AlignTop)
          .setImage("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Map/other")
          .addToGameObject(wall);
  gameScene->attachGameObject(wall);
  //打印二维数组
    /*for(int i = 0; i < 15; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            printf("%d ", My_map.get_map(i, j));
        }
        printf("\n");
    }*/
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setFixedSize(1000,600);
  gameScene = new GameScene();
  gameScene->setParent(this);
  auto view = new QGraphicsView(gameScene, this);
  view->setFrameStyle(QFrame::NoFrame);
  view->resize(this->size());
  view->setSceneRect(QRect(0, 0, this->width(), this->height()));
  //添加地砖的图片
  QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(QPixmap("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Map/dizhuan.png"));
  //pixmapItem->setOffset(QPoint(100,200));//,,,调整位置
  gameScene->addItem(pixmapItem);

  loadScene(gameScene);
  //初始化player1
  QString str1(":/player1/image/Player1/p1_live.png");
  QByteArray cpath1 = str1.toLocal8Bit();
  char*path1 = cpath1.data();//这一步是把QString转为char*类型
  init_player(1, path1, Qt::Key_W, Qt::Key_S, Qt::Key_A, Qt::Key_D, Qt::Key_Space,
              ":/player1/image/Player1/p1_up1.png",
              ":/player1/image/Player1/p1_up2.png",
              ":/player1/image/Player1/p1_down1.png",
              ":/player1/image/Player1/p1_down2.png",
              ":/player1/image/Player1/p1_down3.PNG",
              ":/player1/image/Player1/p1_left1.png",
              ":/player1/image/Player1/p1_left2.png",
              ":/player1/image/Player1/p1_left3.png",
              ":/player1/image/Player1/p1_right1.png",
              ":/player1/image/Player1/p1_right2.png",
              ":/player1/image/Player1/p1_right3.png");

  //初始化player2
  QString str2(":/player2/image/Player2/p2_live.png");
  QByteArray cpath2 = str2.toLocal8Bit();
  char*path2 = cpath2.data();
  init_player(2, path2, Qt::Key_Up, Qt::Key_Down, Qt::Key_Left, Qt::Key_Right, Qt::Key_Return,
              ":/player2/image/Player2/p2_up1.png",
              ":/player2/image/Player2/p2_up2.png",
              ":/player2/image/Player2/p2_down1.png",
              ":/player2/image/Player2/p2_down2.png",
              ":/player2/image/Player2/p2_down3.png",
              ":/player2/image/Player2/p2_left1.png",
              ":/player2/image/Player2/p2_left2.png",
              ":/player2/image/Player2/p2_left3.png",
              ":/player2/image/Player2/p2_right1.png",
              ":/player2/image/Player2/p2_right2.png",
              ":/player2/image/Player2/p2_right3.png");

  //初始化robot1
  QString str3(":/robot1/image/Robot1/r1_down1.png");
  QByteArray cpath3 = str3.toLocal8Bit();
  char*path3 = cpath3.data();
  init_robot(-1, path3,
             ":/robot1/image/Robot1/r1_up1.png",
             ":/robot1/image/Robot1/r1_up2.png",
             ":/robot1/image/Robot1/r1_down1.png",
             ":/robot1/image/Robot1/r1_down2.png",
             ":/robot1/image/Robot1/r1_left1.png",
             ":/robot1/image/Robot1/r1_left2.png",
             ":/robot1/image/Robot1/r1_right1.png",
             ":/robot1/image/Robot1/r1_right2.png");

  //初始化robot2
  QString str4(":/robot2/image/Robot2/r2_down1.png");
  QByteArray cpath4 = str4.toLocal8Bit();
  char*path4 = cpath4.data();
  init_robot(-2, path4,
             ":/robot2/image/Robot2/r2_up1.png",
             ":/robot2/image/Robot2/r2_up2.png",
             ":/robot2/image/Robot2/r2_down1.png",
             ":/robot2/image/Robot2/r2_down2.png",
             ":/robot2/image/Robot2/r2_left1.png",
             ":/robot2/image/Robot2/r2_left2.png",
             ":/robot2/image/Robot2/r2_right1.png",
             ":/robot2/image/Robot2/r2_right2.png");
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::init_player(int type, char* photo, Qt::Key key_up, Qt::Key key_down, Qt::Key key_left, Qt::Key key_right, Qt::Key key_bomb, QString up1, QString up2, QString down1, QString down2, QString down3, QString left1, QString left2, QString left3, QString right1, QString right2, QString right3)
{
    while(1)
    {
        quint32 x = (QRandomGenerator::global()->generate()) % 14;
        quint32 y = (QRandomGenerator::global()->generate()) % 20;
        if(My_map.get_map(x, y) == 0 && (!My_map.get_map(x - 1, y) || !My_map.get_map(x + 1, y) || !My_map.get_map(x, y - 1) || !My_map.get_map(x, y + 1)))
        {
            auto player = new GameObject();
            ImageTransformBuilder()
                    .setPos(QPointF(40*y,40*x))
                    .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                    .setImage(photo)
                    .addToGameObject(player);
            player->addComponent(new Physics());
            player->addComponent(new UserController(1, 1, 1, key_up, key_down, key_left, key_right, key_bomb, up1, up2, down1, down2, down3, left1, left2, left3, right1, right2, right3));//玩家的相关数据的初始化
            player->addComponent(new ImageTransform);
            player->addComponent(new Health(3));
            gameScene->attachGameObject(player);
            auto user = player->getComponent<UserController>();
            user->set_tool_speed(0);//设置初始的速度
            user->set_tool_range(0);
            user->set_tool_bomb_num(0);//初始时的道具时间都为0
            auto trans = player->getComponent<Transform>();
            trans->setType(type);
            break;
        }
    }
}

void MainWindow::init_robot(int type, char *photo, QString up1, QString up2, QString down1, QString down2, QString left1, QString left2, QString right1, QString right2)
{
    while(1)
    {
        quint32 x = (QRandomGenerator::global()->generate()) % 14;
        quint32 y = (QRandomGenerator::global()->generate()) % 20;
        if(My_map.get_map(x, y) == 0 && (!My_map.get_map(x - 1, y) || !My_map.get_map(x + 1, y) || !My_map.get_map(x, y - 1) || !My_map.get_map(x, y + 1)))
        {
            auto robot = new GameObject();
            ImageTransformBuilder()
                    .setPos(QPointF(40*y,40*x))
                    .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                    .setImage(photo)
                    .addToGameObject(robot);
            robot->addComponent(new Health(1));
            robot->addComponent(new Physics());
            robot->addComponent(new AIcontroller(up1, up2, down1, down2, left1, left2, right1, right2));//玩家的相关数据的初始化
            robot->addComponent(new ImageTransform);
            auto trans = robot->getComponent<Transform>();
            trans->setType(type);
            gameScene->attachGameObject(robot);
            break;
        }
    }
}
