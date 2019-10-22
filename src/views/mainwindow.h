#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../models/stereoimage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(StereoImage& image, QWidget* parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow* ui;
  StereoImage& image;

private slots:
  void on_listWidgetTarget_currentRowChanged(int currentRow);
  void on_targetSettingPanel_targetModified(const Target &target);
  void on_canvasSettingPanel_canvasChanged();
  
};
#endif  // MAINWINDOW_H
