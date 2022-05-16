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
#include <QPainter>
#include "mybutton.h"
#include <QTimer>
#include <QPushButton>
#include <QPropertyAnimation>

void loadScene(GameScene *gameScene) {
  //增加背景板的图片
  QGraphicsPixmapItem *pixmapItem0 = new QGraphicsPixmapItem(QPixmap(":/surface/image/surface/background.png"));
  gameScene->addItem(pixmapItem0);
  //添加地砖的图片
  QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(QPixmap(":/map/image/Map/dizhuan.png"));
  gameScene->addItem(pixmapItem);
  //pixmapItem->setOffset(QPoint(100,200));//,,,调整位置
  QGraphicsPixmapItem *pix = new QGraphicsPixmapItem(QPixmap(":/surface/image/surface/score1.png"));
  pix->setOffset(820, 130);
  gameScene->addItem(pix);
  QGraphicsPixmapItem *pixx = new QGraphicsPixmapItem(QPixmap(":/surface/image/surface/score2.png"));
  pixx->setOffset(820, 350);
  gameScene->addItem(pixx);

  QPainter painter;
  painter.setPen(Qt::white);
  QFont font = painter.font();
  font.setPixelSize(10);
  font.setFamily("Microsoft YaHei");
  painter.setFont(font);
  painter.drawText(830, 160, QString::number(p1_score).toStdString());


  //加载地图图片
  My_map.init_Map();
  //上面的围栏
  for(int i = 0; i < 3; i++)
  {
      auto wall = new GameObject();
      ImageTransformBuilder()
              .setPos(QPointF(40+240*i, 0))
              .setAlignment(Qt::AlignLeft | Qt::AlignTop)
              .setImage(":/map/image/Map/side1.png")
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
              .setImage(":/map/image/Map/side11.png")
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
                            .setImage(":/map/image/Map/hard1.png")
                            .addToGameObject(wall);                     
                }
                else if(My_map.get_map(i,j) == 1 && i % 2 == 1)
                {

                    ImageTransformBuilder()
                            .setPos(QPointF(40*j, 40*i))
                            .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                            .setImage(":/map/image/Map/hard2.png")
                            .addToGameObject(wall);
                }
                else if(My_map.get_map(i,j) == 2 && j % 2 == 1)
                {
                    ImageTransformBuilder()
                            .setPos(QPointF(40*j, 40*i))
                            .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                            .setImage(":/map/image/Map/soft1.png")
                            .addToGameObject(wall);
                    wall->addComponent(new Hitable);
                }
                else if(My_map.get_map(i,j) == 2 && j % 2 == 0)
                {
                    ImageTransformBuilder()
                            .setPos(QPointF(40*j, 40*i))
                            .setAlignment(Qt::AlignLeft | Qt::AlignTop)
                            .setImage(":/map/image/Map/soft2.png")
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
                  .setImage(":/map/image/Map/side2.png")
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
                  .setImage(":/map/image/Map/side22.png")
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

  //init_player1
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
                  .setImage(":/player1/image/Player1/p1_live.png")
                  .addToGameObject(player);
          player->addComponent(new Physics());
          player->addComponent(new UserController(1, 1, 1, Qt::Key_W, Qt::Key_S, Qt::Key_A, Qt::Key_D, Qt::Key_Space,
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
                                                  ":/player1/image/Player1/p1_right3.png"));
          player->addComponent(new ImageTransform);
          player->addComponent(new Health(3));
          gameScene->attachGameObject(player);
          auto user = player->getComponent<UserController>();
          user->set_tool_speed(0);//设置初始的速度
          user->set_tool_range(0);
          user->set_tool_bomb_num(0);//初始时的道具时间都为0
          auto trans = player->getComponent<Transform>();
          trans->setType(1);
          break;
      }
  }

  //init_player2
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
                  .setImage(":/player2/image/Player2/p2_live.png")
                  .addToGameObject(player);
          player->addComponent(new Physics());
          player->addComponent(new UserController(1, 1, 1, Qt::Key_Up, Qt::Key_Down, Qt::Key_Left, Qt::Key_Right, Qt::Key_Return,
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
                                                  ":/player2/image/Player2/p2_right3.png"));
          player->addComponent(new ImageTransform);
          player->addComponent(new Health(3));
          gameScene->attachGameObject(player);
          auto user = player->getComponent<UserController>();
          user->set_tool_speed(0);//设置初始的速度
          user->set_tool_range(0);
          user->set_tool_bomb_num(0);//初始时的道具时间都为0
          auto trans = player->getComponent<Transform>();
          trans->setType(2);
          break;
      }
  }

  //init_robot1
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
                  .setImage(":/robot1/image/Robot1/r1_down1.png")
                  .addToGameObject(robot);
          robot->addComponent(new Health(1));
          robot->addComponent(new Physics());
          robot->addComponent(new AIcontroller(
                                  ":/robot1/image/Robot1/r1_up1.png",
                                  ":/robot1/image/Robot1/r1_up2.png",
                                  ":/robot1/image/Robot1/r1_down1.png",
                                  ":/robot1/image/Robot1/r1_down2.png",
                                  ":/robot1/image/Robot1/r1_left1.png",
                                  ":/robot1/image/Robot1/r1_left2.png",
                                  ":/robot1/image/Robot1/r1_right1.png",
                                  ":/robot1/image/Robot1/r1_right2.png"));//玩家的相关数据的初始化
          robot->addComponent(new ImageTransform);
          auto trans = robot->getComponent<Transform>();
          trans->setType(-1);
          gameScene->attachGameObject(robot);
          break;
      }
  }

  //init_robot2
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
                  .setImage(":/robot2/image/Robot2/r2_down1.png")
                  .addToGameObject(robot);
          robot->addComponent(new Health(1));
          robot->addComponent(new Physics());
          robot->addComponent(new AIcontroller(
                                  ":/robot2/image/Robot2/r2_up1.png",
                                  ":/robot2/image/Robot2/r2_up2.png",
                                  ":/robot2/image/Robot2/r2_down1.png",
                                  ":/robot2/image/Robot2/r2_down2.png",
                                  ":/robot2/image/Robot2/r2_left1.png",
                                  ":/robot2/image/Robot2/r2_left2.png",
                                  ":/robot2/image/Robot2/r2_right1.png",
                                  ":/robot2/image/Robot2/r2_right2.png"));//玩家的相关数据的初始化
          robot->addComponent(new ImageTransform);
          auto trans = robot->getComponent<Transform>();
          trans->setType(-2);
          gameScene->attachGameObject(robot);
          break;
      }
  }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    stop = false;
  ui->setupUi(this);
  setWindowFlags(Qt::WindowCloseButtonHint | Qt:: MSWindowsFixedSizeDialogHint);
  setFixedSize(1000, 700);
  setWindowIcon(QIcon(":/surface/image/surface/upIcon.png"));
  setWindowTitle("Crazy Arcade");

  auto game1 = new GameScene();
  gameScene = game1;
  gameScene->setParent(this);
  view = new QGraphicsView(gameScene, this);
  view->setFrameStyle(QFrame::NoFrame);
  view->resize(this->size());
  view->setSceneRect(QRect(0, 0, this->width(), this->height()));
  view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    startBtn = new QPushButton;
    startBtn->move(this->width() * 0.35, this->height() * 0.8);

    backBtn = new QPushButton;

    crolBtn = new QPushButton;
    crolBtn->move(this->width() * 0.685, this->height() * 0.89);

    pause = new QPushButton;
    pause->move(this->width() * 0.83, this->height() * 0.8);

    exitBtn = new QPushButton;
    exitBtn->move(this->width() * 0.9, this->height() * 0.9);

    toolBtn = new QPushButton;

    introBtn = new QPushButton;
    introBtn->move(this->width() * 0.65, this->height() * 0.8);


  init_btn(startBtn, ":/surface/image/surface/start.png");
  init_btn(backBtn, ":/surface/image/surface/home_page.png");
  init_btn(crolBtn, ":/surface/image/surface/con_page.png");
  init_btn(toolBtn, ":/surface/image/surface/tool_page.png");
  init_btn(pause, ":/surface/image/surface/pause.png");
  init_btn(exitBtn, ":/surface/image/surface/back.png");
  init_btn(introBtn, ":/surface/image/surface/rule.png");

  //controlBtn
   connect(crolBtn, &QPushButton::pressed, this, [=]{
      zoom(crolBtn, 0, 10);
      zoom(crolBtn, 10, 0);
      //延时进入
      QTimer::singleShot(500, this, [=](){
          change();
          backBtn->hide();
          crolBtn->hide();
          auto game2 = new GameScene;
          this->gameScene = game2;
          view->setScene(this->gameScene);
          loadScene2(this->gameScene);
      });
  });

  //startBtn
  connect(startBtn, &QPushButton::pressed, this, [=]{
      zoom(startBtn, 0, 10);
      zoom(startBtn, 10, 0);
      //延时进入
      QTimer::singleShot(500, this, [=](){
          //转换页面操作(删除主页图片，隐藏btn)
          change();
          startBtn->hide();
          introBtn->hide();
          exitBtn->hide();
          pause->show();
          auto game2 = new GameScene();
          this->gameScene = game2;
          view->setScene(this->gameScene);
          loadScene(this->gameScene);
      });
  });

  //exitBtn
  connect(exitBtn, &QPushButton::pressed, this, [=]{
      zoom(exitBtn, 0, 3);
      zoom(exitBtn, 3, 0);
      QTimer::singleShot(200, this, [=](){
          this->close();
      });
  });

  //backBtn
  connect(backBtn, &QPushButton::pressed, this, [=]{
      zoom(backBtn, 0, 10);
      zoom(backBtn, 10, 0);
      //延时进入
      QTimer::singleShot(500, this, [=](){
          change();
          backBtn->hide();
          toolBtn->hide();
          crolBtn->hide();
          auto game2 = new GameScene;
          this->gameScene = game2;
          view->setScene(this->gameScene);
          loadScene1(this->gameScene);
      });
  });

  //toolBtn
  connect(toolBtn, &QPushButton::pressed, this, [=]{
      zoom(toolBtn, 0, 10);
      zoom(toolBtn, 10, 0);
      //延时进入
      QTimer::singleShot(500, this, [=](){
          change();
          backBtn->hide();
          toolBtn->hide();
          auto game2 = new GameScene;
          this->gameScene = game2;
          view->setScene(this->gameScene);
          loadScene3(this->gameScene);
      });
  });

  //introBtn
  connect(introBtn, &QPushButton::pressed, this, [=]{
      zoom(introBtn, 0, 10);
      zoom(introBtn, 10, 0);
      //延时进入
      QTimer::singleShot(500, this, [=](){
          change();
          startBtn->hide();
          introBtn->hide();
          exitBtn->hide();
          auto game2 = new GameScene;
          this->gameScene = game2;
          view->setScene(this->gameScene);
          loadScene2(this->gameScene);
      });
  });
  //pause
  connect(pause, &QPushButton::pressed, this, [=]{
      zoom(pause, 0, 10);
      zoom(pause, 10, 0);
      //延时进入
      QTimer::singleShot(500, this, [=](){
          change();
          if(stop == true)
          {
              updateTimer->start();
              stop = false;
          }
          else
          {
              updateTimer->stop();
              stop = true;
          }

      });
  });
        loadScene1(gameScene);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::loadScene1(GameScene *gameScene)
{
    //homepage
    auto background = new GameObject();
    background->setObjectName("background");
    ImageTransformBuilder()
            .setPos(QPoint(0, 0))
            .setAlignment(Qt::AlignLeft | Qt::AlignTop)
            .setImage(":/surface/image/surface/surface_done.png")
            .addToGameObject(background);
    this->gameScene->attachGameObject(background);
    startBtn->show();
    introBtn->show();
    exitBtn->show();
}

void MainWindow::loadScene2(GameScene *gameScene)
{

    //加载说明界面的按钮(人物)
    auto background = new GameObject();
    background->setObjectName("background");
    ImageTransformBuilder()
            .setPos(QPoint(0, 0))
            .setAlignment(Qt::AlignLeft | Qt::AlignTop)
            .setImage(":/surface/image/surface/intro1.png")
            .addToGameObject(background);
    this->gameScene->attachGameObject(background);
    backBtn->move(this->width() * 0.06, this->height() * 0.88);
    toolBtn->move(this->width() * 0.685, this->height() * 0.89);
    backBtn->show();
    toolBtn->show();
}

void MainWindow::loadScene3(GameScene *gameScene)
{
    //加载说明界面的按钮(道具)
    auto background = new GameObject();
    background->setObjectName("background");
    ImageTransformBuilder()
            .setPos(QPoint(0, 0))
            .setAlignment(Qt::AlignLeft | Qt::AlignTop)
            .setImage(":/surface/image/surface/intro2.png")
            .addToGameObject(background);
    this->gameScene->attachGameObject(background);
    backBtn->move(this->width() * 0.072, this->height() * 0.87);
    crolBtn->move(this->width() * 0.665, this->height() * 0.862);
    backBtn->show();
    crolBtn->show();
}

void MainWindow::init_btn(QPushButton *btn, QString image)
{
    btn->setParent(this);//!!!
    QPixmap pix;
    pix.load(image);
    btn->QPushButton::setFixedSize(pix.width(), pix.height());
    btn->QPushButton::setStyleSheet("QPushButton{border:0px;}");
    btn->QPushButton::setIcon(pix);
    btn->QPushButton::setIconSize(QSize(pix.width(), pix.height()));
    btn->hide();
}

void MainWindow::zoom(QPushButton *btn, int offset1, int offset2)
{
    QPropertyAnimation *ani = new QPropertyAnimation(btn, "geometry");
    //设置时间间隔
    ani->setDuration(200);
    //起始和结束位置
    ani->setStartValue(QRect(btn->QPushButton::x(), btn->y() + offset1, btn->width(), btn->height()));
    ani->setEndValue(QRect(btn->x(), btn->y() + offset2, btn->width(), btn->height()));
    //设置弹跳曲线
    ani->setEasingCurve(QEasingCurve::OutBounce);
    //执行动画(上面的设置都配备好了)
    ani->start();
}

void MainWindow::change()
{
    auto ptr = gameScene->getGameObject("background");
    if(ptr != NULL)
    gameScene->detachGameObject(ptr);
}

