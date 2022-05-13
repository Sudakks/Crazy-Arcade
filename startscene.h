#ifndef STARTSCENE_H
#define STARTSCENE_H

#include <QMainWindow>
#include <QWidget>
#include "mainwindow.h"
class StartScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit StartScene(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);

private:
    MainWindow *w = NULL;
signals:

};

#endif // STARTSCENE_H
