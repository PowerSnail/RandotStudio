#include "previewcanvasitem.h"

PreviewCanvasItem::PreviewCanvasItem(QWidget* parent) : QLabel(parent) {
  this->setScaledContents(true);
}

void PreviewCanvasItem::setSelected(bool selected) {
  if (selected == this->selected) return;
  if (selected) {
    this->setStyleSheet(kStyleSheetSelected);
  } else {
    this->setStyleSheet(kStyleSheetUnselected);
  }
  this->selected = selected;
}

int PreviewCanvasItem::getX() {
  return posX;
}
void PreviewCanvasItem::setX(int value) {
  posX = value;
}

int PreviewCanvasItem::getY() {
  return posY;
}
void PreviewCanvasItem::setY(int value) {
  posY = value;
}

void PreviewCanvasItem::mousePressEvent(QMouseEvent* event) {
  QLabel::mousePressEvent(event);
  emit clicked(this);
}