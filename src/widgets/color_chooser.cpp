// Copyright (c) 2020 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#include "color_chooser.h"
#include <QColorDialog>
#include <QStyle>

static const QString kBtnStyleTemplate{
    "QPushButton {background-color: %1; "
    "border-style: solid;"
    "border-color: %1;"
    "border-width: 1px;"
    "border-radius: 2px;"
    "}"};

ColorChooser::ColorChooser(QWidget* parent) : QPushButton(parent) {
  SetColor(QColor(0, 0, 0));
  connect(this, &ColorChooser::clicked, this, [=]() {
    this->SetColor(QColorDialog::getColor(this->color, this, tr("Choose a color")));
  });
}

QColor ColorChooser::Color() {
  return this->color;
}

void ColorChooser::SetColor(QColor color) {
  if (color == this->color) {
    return;
  }
  this->color = color;
  this->setStyleSheet(kBtnStyleTemplate.arg(color.name()));
  emit colorChanged(color);
}
