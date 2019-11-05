#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <map>
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
  QLabel selectionSquare;
  std::map<QString, QPixmap> loadedShapes;
  QString prevShapeDir;

  // Sentinel
  static Target kNoTarget;
  constexpr static int kPreviewShadowSize = 20;

  int getCurrentTargetId();
  void setCurrentTargetId(int value);
  Target& currentTarget();

  void loadCanvasProperties();
  void loadShape(QString filename);
  void refreshCurrentTarget();
  
  QPixmap& getShape(int id);
  Target& getTarget(int id);
  QPixmap getTargetPreview(Target&);
  
  void renderPreview();
  void showSelectionSquare();

  void resizeEvent(QResizeEvent* event);
  void showEvent(QShowEvent *event);

 private slots:
  // Slots for target list
  void on_listWidgetTarget_currentRowChanged(int currentRow);
  void on_btnAddTarget_clicked();
  void on_btnRemoveTarget_clicked();

  // Slots for shape list
  void on_btnAddShape_clicked();
  void on_btnRemoveShape_clicked();

  // Slots for canvas properties
  void on_lineEditWidth_editingFinished();
  void on_lineEditHeight_editingFinished();
  void on_lineEditGrainSize_editingFinished();
  void on_colorChooserForeground_colorChanged(const QColor& color);
  void on_colorChooserBackground_colorChanged(const QColor& color);
  void on_checkBoxCrossed_stateChanged(int state);

  // Slots for target properties
  void on_lineEditX_editingFinished();
  void on_lineEditY_editingFinished();
  void on_lineEditScale_editingFinished();
  void on_lineEditParity_editingFinished();
  void on_comboBoxRotate_currentIndexChanged(int index);
  void on_colorChooserTarget_colorChanged(const QColor& color);
  void on_listWidgetShape_currentRowChanged(int currentRow);

  // Actions
  void on_actionAbout_triggered();
  void on_actionSave_triggered();
  void on_actionLoad_triggered();
  void on_actionRandot_triggered();
  void on_actionStereo_PNG_triggered();

//  signals:



};
#endif  // MAINWINDOW_H
