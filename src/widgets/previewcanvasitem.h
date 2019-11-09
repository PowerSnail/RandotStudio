#pragma once

#include <QLabel>
#include <QWidget>

class PreviewCanvasItem : public QLabel {
  Q_OBJECT
 private:
  constexpr static const char* kStyleSheetSelected = "background: rgba(0, 0, 255, 50); border: 1px solid blue;";
  constexpr static const char* kStyleSheetUnselected = "background: transparent; border: none;";

  bool selected = false;
  int posX = 0;
  int posY = 0;

 private:  // Methods
  void mousePressEvent(QMouseEvent *event);

 public:
  explicit PreviewCanvasItem(QWidget *parent = nullptr);
  void setSelected(bool selected);

  int getX();
  void setX(int value);

  int getY();
  void setY(int value);

 signals:
  void clicked(PreviewCanvasItem *sender);
};
