#include "models/stereoimage.h"
#include "views/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  StereoImage img(1920, 1080);
  QApplication a(argc, argv);
  MainWindow w(img);
  w.show();
  return a.exec();
}
