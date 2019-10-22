#include "color_chooser.h"
#include <QColorDialog>
#include <QStyle>
#include "ui_color_chooser.h"

const QString ColorChooser::kBtnStyleTemplate{"background: %1;"};

ColorChooser::ColorChooser(QWidget* parent)
    : QWidget(parent), ui(new Ui::ColorChooser) {
  ui->setupUi(this);
}

ColorChooser::~ColorChooser() { delete ui; }

QColor ColorChooser::getColor() { return this->color; }

void ColorChooser::setColor(QColor color) {
  if (color == this->color) {
    return;
  }
  this->color = color;
  this->ui->btnChoose->setStyleSheet(kBtnStyleTemplate.arg(color.name()));
  emit colorChanged(this->color);
}

void ColorChooser::on_btnChoose_clicked() {
  QColor color =
      QColorDialog::getColor(this->color, this, tr("Choose a color"));
  this->setColor(color);
}
