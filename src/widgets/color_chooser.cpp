// Copyright (c) 2020 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#include "color_chooser.h"
#include <QColorDialog>
#include <QStyle>
#include "ui_color_chooser.h"

const QString ColorChooser::kBtnStyleTemplate{
    "QPushButton {background-color: %1; "
    "border-style: solid;"
    "border-color: %1;"
    "border-width: 1px;"
    "border-radius: 2px;"
    "}"};

ColorChooser::ColorChooser(QWidget* parent)
    : QWidget(parent), ui(new Ui::ColorChooser) {
  ui->setupUi(this);
  SetColor(QColor(0, 0, 0));
}

ColorChooser::~ColorChooser() {
  delete ui;
}

QColor ColorChooser::Color() {
  return this->color;
}

void ColorChooser::SetColor(QColor color) {
  if (color == this->color) {
    return;
  }
  this->color = color;
  this->ui->btnChoose->setStyleSheet(kBtnStyleTemplate.arg(color.name()));
  emit colorChanged(this->color);
}

void ColorChooser::on_btnChoose_clicked() {
  QColor color = QColorDialog::getColor(this->color, this, tr("Choose a color"));
  this->SetColor(color);
  ui->btnChoose->clearFocus();
}
