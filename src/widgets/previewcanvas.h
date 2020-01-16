// Copyright (c) 2020 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#pragma once

#include <QColor>
#include <QLabel>
#include <QPixmap>
#include <QWidget>
#include <deque>

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
  explicit PreviewCanvas(QWidget* parent = nullptr);

  void SetCanvasSize(int w, int h);
  void SetBackground(QColor color);
  int CurrentIndex();
  void SetCurrentIndex(int index);
  void InsertPixmap(int id, int x, int y, const QPixmap& img);
  void replacePixmap(int id, const QPixmap& img);
  void MovePixmap(int id, int x, int y);
  void RemovePixmap(int id);
  QSize PreviewSize();

 private:  // Methods
  void resizeEvent(QResizeEvent* event);
  int find(PreviewCanvasItem* child);
  void redrawChild(PreviewCanvasItem* child);
  PreviewCanvasItem* child(int index);

 private slots:
  void on_child_clicked(PreviewCanvasItem* sender);

 signals:
  void currentIndexChanged(int index);
};
