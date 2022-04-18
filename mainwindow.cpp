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

map My_map;
void loadScene(GameScene *gameScene) {
 /* auto shooter = new GameObject();
  //这个是用来绘图的
  ImageTransformBuilder()
      .setPos(QPointF(100, 100))
      //.setImage("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Map/soft1")
      .setAlignment(Qt::AlignCenter)
      .addToGameObject(shooter);
  shooter->addComponent(new Shooter);
  gameScene->attachGameObject(shooter);
  auto summonDummyBtn = new GameObject();
  summonDummyBtn->addComponent(new Transform);
  summonDummyBtn->addComponent(new SummonDummy);
  gameScene->attachGameObject(summonDummyBtn);
*/

  /*
  //
  auto obj = new GameObject();
  auto transform = new Transform();
  auto circle = new QGraphicsEllipseItem (transform);
  circle->setRect(-5, -5, 10 , 10);//这应该是circle所在的区域，然后挂载到了transform下面
  transform->setPos(100 , 100);
  obj->addComponent(transform);
  obj->addComponent(new Physics());
  obj->addComponent(new UserController());
  //属性都好了，再加到scene里面
  gameScene->attachGameObject(obj);
*/

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
      wall->addComponent(new Transform);
      //wall->addComponent(new Hitable);
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
      wall->addComponent(new Transform);
     // wall->addComponent(new Hitable);
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
                     wall->addComponent(new Hitable);
                }
                else if(My_map.get_map(i,j) == 1 && i % 2 == 1)
                {

                    ImageTransformBuilder()
                            .setPos(QPointF(40*j, 40*i))
                            .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                            .setImage("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Map/hard2")
                            .addToGameObject(wall);
                      wall->addComponent(new Hitable);
                }
                else if(My_map.get_map(i,j) == 2 && j % 2 == 1)
                {
                    ImageTransformBuilder()
                            .setPos(QPointF(40*j, 40*i))
                            .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                            .setImage("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Map/soft1")
                            .addToGameObject(wall);
                }
                else if(My_map.get_map(i,j) == 2 && j % 2 == 0)
                {
                    ImageTransformBuilder()
                            .setPos(QPointF(40*j, 40*i))
                            .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                            .setImage("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Map/soft2")
                            .addToGameObject(wall);
                }
                wall->addComponent(new Transform);
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
          wall->addComponent(new Transform);
         // wall->addComponent(new Hitable);
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
          wall->addComponent(new Transform);
          //wall->addComponent(new Hitable);
          gameScene->attachGameObject(wall);
      }
  }
  auto wall = new GameObject();
  ImageTransformBuilder()
          .setPos(QPointF(8*40,6*40))
          .setAlignment(Qt::AlignLeft | Qt::AlignTop)
          .setImage("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Map/other")
          .addToGameObject(wall);
  wall->addComponent(new Transform);
  //wall->addComponent(new Hitable);
  gameScene->attachGameObject(wall);
  //初始化玩家
    while(1)
    {
        quint32 x = (QRandomGenerator::global()->generate()) % 14;
        quint32 y = (QRandomGenerator::global()->generate()) % 20;
        if(!My_map.get_map(x, y))
        {
            auto player1 = new GameObject();
            ImageTransformBuilder()
                    .setPos(QPointF(40*y,40*x))
                    .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                    .setImage("C:/Users/DELL/Desktop/pro2/project-2-Sudakks/image/Player1/p1_live")
                    .addToGameObject(player1);
            player1->addComponent(new Transform);
            //player1->addComponent(new Hitable);
            player1->addComponent(new Physics());
            player1->addComponent(new UserController);
            player1->addComponent(new ImageTransform);
            gameScene->attachGameObject(player1);
            break;
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setFixedSize(1200,1000);
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
}

MainWindow::~MainWindow() { delete ui; }

