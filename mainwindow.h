#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <gamescene.h>

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
  void init_player(int type, char* photo, Qt::Key key_up, Qt::Key key_down, Qt::Key key_left, Qt::Key key_right, Qt::Key key_bomb, QString up1, QString up2, QString down1, QString down2, QString down3, QString left1, QString left2, QString left3, QString right1, QString right2, QString right3);
  void init_robot(int type, char* photo, QString up, QString down, QString left, QString right);
 private:
  Ui::MainWindow *ui;
  GameScene *gameScene;
};
#endif  // MAINWINDOW_H_
