#include "startscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mybutton.h"
#include <QDebug>
#include "mainwindow.h"
#include "common.h"
#include <QTimer>
#include "gamescene.h"
#include "instruction.h"

StartScene::StartScene(QWidget *parent)
    : QMainWindow{parent}
{
    this->setFixedSize(800, 650);
    this->setWindowIcon(QPixmap(":/surface/image/surface/upIcon.png"));
    this->setWindowTitle("Crazzy Arcade");

    /*GameScene *start_page = new GameScene;
    GameScene *tool_page = new GameScene;
    GameScene *player_page = new GameScene;
    start_page->setParent(this);
    tool_page->setParent(this);
    player_page->setParent(this);*/

    init_start_page();

    /*view = new QGraphicsView(gameScene, this);
    //view = new QGraphicsView(start_page, this);
    view->setFrameStyle(QFrame::NoFrame);
    view->resize(this->size());
    view->setSceneRect(QRect(0, 0, this->width(), this->height()));
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);*/


    /*w = new MainWindow;
    i = new instruction;

    //创建开始按钮
    MyButton * startBtn = new MyButton(":/surface/image/surface/start.png");
    startBtn->setParent(this);
    startBtn->move(this->width() * 0.30 - startBtn->width() * 0.5, this->height() * 0.82);
    connect(startBtn, &MyButton::clicked, [=]{
        startBtn->zoom(0, 10);
        startBtn->zoom(10, 0);
        //延时进入
        QTimer::singleShot(500, this, [=](){
            this->hide();
            w->show();           
        });
    });

    //创建退出动画
    MyButton * instructBtn = new MyButton(":/surface/image/surface/rule.png");
    instructBtn->setParent(this);
    instructBtn->move(this->width() * 0.72 - startBtn->width() * 0.5, this->height() * 0.82);
    connect(instructBtn, &MyButton::clicked, [=]{
        instructBtn->zoom(0, 10);
        instructBtn->zoom(10, 0);
        QTimer::singleShot(500, this, [=](){
            this->hide();
            i->show();
        });
    });

    //音效按钮
    MyButton * musicBtn = new MyButton(":/surface/image/surface/medium-volume.png");
    musicBtn->setParent(this);
    musicBtn->move(this->width() * 0.07, this->height() * 0.05);
    connect(musicBtn, &MyButton::clicked, [=]{
        musicBtn->zoom(0, 3);
        musicBtn->zoom(3, 0);
    });

    MyButton * exitBtn = new MyButton(":/surface/image/surface/back.png");
    exitBtn->setParent(this);
    exitBtn->move(this->width() * 0.92, this->height() * 0.92);
    connect(exitBtn, &MyButton::clicked, [=]{
        exitBtn->zoom(0, 3);
        exitBtn->zoom(3, 0);
        QTimer::singleShot(200, this, [=](){
            this->close();
        });
    });*/
}

void StartScene::paintEvent(QPaintEvent *)
{
    /*QPainter painter(this);
    QPixmap pix;
    pix.load(":/surface/image/surface/surface_done.png");
    painter.drawPixmap(0, 0,this->width(), this->height(), pix);

    QPixmap pix1;
    pix1.load(":/surface/image/surface/title.png");
    painter.drawPixmap(this->width()*0.50, this->height()*0.54,this->width()*0.4, this->height()*0.3, pix1);
    */
}

void StartScene::init_start_page()
{

}

void StartScene::init_tool_page()
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/surface/image/surface/intro2.png");
    painter.drawPixmap(0, 0,this->width(), this->height(), pix);

    //back按钮
    MyButton * backBtn = new MyButton(":/surface/image/surface/back.png");
    backBtn->setParent(this);
    backBtn->move(this->width(), this->height());
    connect(backBtn, &MyButton::clicked, [=]{
        backBtn->zoom(0, 3);
        backBtn->zoom(3, 0);
        QTimer::singleShot(200, this, [=](){
            //this->close();//增加转换功能
            this->hide();
        });
    });

    MyButton * playerBtn = new MyButton(":/surface/image/surface/con_page.png");
    backBtn->setParent(this);
    backBtn->move(this->width(), this->height());
    connect(backBtn, &MyButton::clicked, [=]{
        backBtn->zoom(0, 3);
        backBtn->zoom(3, 0);
        QTimer::singleShot(200, this, [=](){
            //this->close();//增加转换功能
            this->hide();
        });
    });
}

void StartScene::init_player_page()
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/surface/image/surface/intro1.png");
    painter.drawPixmap(0, 0,this->width(), this->height(), pix);

    //返回按钮
    MyButton * backBtn = new MyButton(":/surface/image/surface/back.png");
    backBtn->setParent(this);
    backBtn->move(this->width(), this->height());
    connect(backBtn, &MyButton::clicked, [=]{
        backBtn->zoom(0, 3);
        backBtn->zoom(3, 0);
        QTimer::singleShot(200, this, [=](){
            //this->close();//增加转换功能
            this->hide();
        });
    });

    //tool页面按钮
    MyButton * toolBtn = new MyButton(":/surface/image/surface/tool_page.png");
    toolBtn->setParent(this);
    toolBtn->move(this->width(), this->height());
    connect(toolBtn, &MyButton::clicked, [=]{
        toolBtn->zoom(0, 3);
        toolBtn->zoom(3, 0);
        QTimer::singleShot(200, this, [=](){
            //this->close();//增加转换功能
            this->hide();
        });
    });
}
