#include <QApplication>
#include "common.h"
#include "mainwindow.h"
#include "startscene.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  StartScene start;
  start.show();
  //MainWindow w;
  //w.show();
  return a.exec();
}
