#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <gamescene.h>
#include "mybutton.h"
#include <QGraphicsView>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void loadScene1(GameScene *gameScene);//home
  void loadScene2(GameScene *gameScene);//introduction1
  void loadScene3(GameScene * gameScene);//introduction2
  GameScene *gameScene;
  void init_btn(QPushButton *btn, QString image);
  void zoom(QPushButton *btn, int offset1, int offset2);
  void change();
  void paintEvent(QPaintEvent *);

 private:
  Ui::MainWindow *ui;
  QPushButton *startBtn;
  QPushButton *backBtn;
  QPushButton *crolBtn;
  QPushButton *toolBtn;
  QPushButton *exitBtn;
  QPushButton *introBtn;
  QGraphicsView *view;
  GameObject * pause1;
  bool stop;
};
#endif  // MAINWINDOW_H_
