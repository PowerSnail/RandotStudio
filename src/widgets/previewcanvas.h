#pragma once

#include <QColor>
#include <QLabel>
#include <deque>
#include <QPixmap>
#include <QWidget>

#include "previewcanvasitem.h"


class PreviewCanvas : public QWidget {
  Q_OBJECT

 private:  // Fields
  constexpr static int kPreviewShadowSize = 20;

  QLabel* bgLabel;
  std::deque<PreviewCanvasItem*> labelList;
  int canvasWidth = 1;
  int canvasHeight = 1;
  int currentIndex = -1;

 public:
  explicit PreviewCanvas(QWidget *parent = nullptr);

  void setCanvasSize(int w, int h);
  void setBackground(QColor color);
  int getCurrentIndex();
  void setCurrentIndex(int index);
  void insertPixmap(int id, int x, int y, const QPixmap& img);
  void replacePixmap(int id, const QPixmap& img);
  void movePixmap(int id, int x, int y);
  void removePixmap(int id);
  QSize previewSize();


 private:  // Methods
  void resizeEvent(QResizeEvent *event);
  int find(PreviewCanvasItem* child);
  void redrawChild(PreviewCanvasItem* child);
  PreviewCanvasItem* child(int index);
 
 private slots:
  void on_child_clicked(PreviewCanvasItem* sender);

 signals:
  void currentIndexChanged(int index);
};
