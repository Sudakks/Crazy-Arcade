#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QWidget>
#include "mainwindow.h"
class instruction : public QMainWindow
{
    Q_OBJECT
public:
    explicit instruction(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);

signals:

};

#endif // INSTRUCTION_H
