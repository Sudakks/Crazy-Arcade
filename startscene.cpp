#include "startscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mybutton.h"
#include <QDebug>
#include "mainwindow.h"
#include "common.h"
#include <QTimer>
#include "gamescene.h"

StartScene::StartScene(QWidget *parent)
    : QMainWindow{parent}
{
    this->setFixedSize(800, 650);
    this->setWindowIcon(QPixmap(":/surface/image/surface/upIcon.png"));
    this->setWindowTitle("Crazzy Arcade");

    //创建菜单栏
    /*QMenuBar *bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu * startMenu = bar->addMenu("Start");

    //创建退出菜单
    QAction * quitAction = startMenu->addAction("Quit");

    //点击退出，实现退出游戏
    connect(quitAction, &QAction::triggered, [=](){
        this->close();
    });
*/
    w = new MainWindow;
    //创建开始按钮
    MyButton * startBtn = new MyButton(":/surface/image/surface/start.png");
    startBtn->setParent(this);
    startBtn->move(this->width() * 0.30 - startBtn->width() * 0.5, this->height() * 0.82);
    connect(startBtn, &MyButton::clicked, [=]{
        startBtn->zoom(0, 10);
        startBtn->zoom(10, 0);
        //延时进入
        QTimer::singleShot(500, this, [=](){
            //scene_num = 1;
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
    });
    /*connect(exitBtn, &MyButton::clicked, [=]{
        this->close();
   });*/

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
    });
}

void StartScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/surface/image/surface/surface_done.png");
    painter.drawPixmap(0, 0,this->width(), this->height(), pix);

    QPixmap pix1;
    pix1.load(":/surface/image/surface/title.png");
    painter.drawPixmap(this->width()*0.50, this->height()*0.54,this->width()*0.4, this->height()*0.3, pix1);

}
