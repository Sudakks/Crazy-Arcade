#ifndef STARTSCENE_H
#define STARTSCENE_H

#include <QMainWindow>
#include <QWidget>
#include "mainwindow.h"
#include "instruction.h"
#include "gamescene.h"

class StartScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit StartScene(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);

private:
    MainWindow *w = NULL;
    GameScene *start_page = NULL;
    GameScene *tool_page = NULL;
    GameScene *player_page = NULL;

public:
    void init_start_page();
    void init_tool_page();
    void init_player_page();
    QGraphicsView * view;
signals:

};

#endif // STARTSCENE_H
