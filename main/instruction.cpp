#include "instruction.h"
#include <QPainter>
#include <QIcon>
#include <QPixmap>
#include <QLabel>
#include <QFont>
#include <QRectF>
#include "mybutton.h"
#include <QTimer>
#include "mainwindow.h"
instruction::instruction(QWidget *parent)
    : QMainWindow{parent}
{
    this->setFixedSize(800, 650);
    this->setWindowIcon(QPixmap(":/surface/image/surface/instruct.png"));
    this->setWindowTitle("Game Rules");

    //加转换按钮
    MyButton * backBtn = new MyButton("");
    backBtn->setParent(this);
    backBtn->move(this->width() * 0.30 - backBtn->width() * 0.5, this->height() * 0.82);
    connect(backBtn, &MyButton::clicked, [=]{
        backBtn->zoom(0, 10);
        backBtn->zoom(10, 0);
        //延时进入
        QTimer::singleShot(500, this, [=](){
            this->hide();
            //s->show();
        });
    });

    //转到工具页面（实现方式是换照片）
}

void instruction::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap intro1;
    intro1.load(":/surface/image/surface/intro1.png");
    painter.drawPixmap(0, 0,this->width(), this->height(), intro1);
}
