// Copyright (c) 2020 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#include "previewcanvasitem.h"

PreviewCanvasItem::PreviewCanvasItem(QWidget* parent) : QLabel(parent) {
  this->setScaledContents(true);
}

void PreviewCanvasItem::SetSelected(bool selected) {
  if (selected == this->selected) {
    return;
  }
  this->setStyleSheet((selected) ? kStyleSheetSelected : kStyleSheetUnselected);
  this->selected = selected;
}

int PreviewCanvasItem::X() {
  return posX;
}
void PreviewCanvasItem::SetX(int value) {
  posX = value;
}

int PreviewCanvasItem::Y() {
  return posY;
}
void PreviewCanvasItem::SetY(int value) {
  posY = value;
}

void PreviewCanvasItem::mousePressEvent(QMouseEvent* event) {
  QLabel::mousePressEvent(event);
  emit clicked(this);
}
