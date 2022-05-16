#include "mybutton.h"
#include <QPushButton>
#include <QDebug>
#include <QPropertyAnimation>
#include "mainwindow.h"
MyButton::MyButton(QString image, QString pressImage)
{   
    this->image = image;
    this->pressImage = pressImage;
    QPixmap pix;
    bool ret = pix.load(image);
    if(!ret)
    {
        qDebug() << "failure loading";
        return;
    }
    /*this->QPushButton::setFixedSize(pix.width(), pix.height());
    this->QPushButton::setStyleSheet("QPushButton{border:0px;}");
    this->QPushButton::setIcon(pix);
    this->QPushButton::setIconSize(QSize(pix.width(), pix.height()));*/
}

void MyButton::zoom(int offset1, int offset2)
{
    QPropertyAnimation *ani = new QPropertyAnimation(this, "geometry");
    //设置时间间隔
    ani->setDuration(200);
    //起始和结束位置
    ani->setStartValue(QRect(this->QPushButton::x(), this->y() + offset1, this->width(), this->height()));
    ani->setEndValue(QRect(this->x(), this->y() + offset2, this->width(), this->height()));

    //设置弹跳曲线
    ani->setEasingCurve(QEasingCurve::OutBounce);

    //执行动画(上面的设置都配备好了)
    ani->start();
}
