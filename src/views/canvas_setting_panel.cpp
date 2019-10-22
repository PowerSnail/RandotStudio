#include "canvas_setting_panel.h"
#include <QIntValidator>
#include "ui_canvas_setting_panel.h"

CanvasSettingPanel::CanvasSettingPanel(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::CanvasSettingPanel),
      canvasSizeValidator(1, 40000, this),
      grainSizeValidator(1, 100, this),
      image(nullptr) {
  ui->setupUi(this);

  ui->lineEditWidth->setValidator(&canvasSizeValidator);
  ui->lineEditHeight->setValidator(&canvasSizeValidator);
  ui->lineEditGrainSize->setValidator(&grainSizeValidator);
}

CanvasSettingPanel::~CanvasSettingPanel() { delete ui; }

int CanvasSettingPanel::getWidth() { return ui->lineEditWidth->text().toInt(); }

int CanvasSettingPanel::getHeight() {
  return ui->lineEditHeight->text().toInt();
}

int CanvasSettingPanel::getGrainSize() {
  return ui->lineEditGrainSize->text().toInt();
}

QColor CanvasSettingPanel::getForeground() {
  return ui->colorChooserForeground->getColor();
}

QColor CanvasSettingPanel::getBackground() {
  return ui->colorChooserBackground->getColor();
}

bool CanvasSettingPanel::getCrossed() {
  return ui->checkBoxCrossed->isChecked();
}

void CanvasSettingPanel::setWidth(int value) {
  ui->lineEditWidth->setText(QString::number(value));
}

void CanvasSettingPanel::setHeight(int value) {
  ui->lineEditHeight->setText(QString::number(value));
}

void CanvasSettingPanel::setGrainSize(int value) {
  ui->lineEditGrainSize->setText(QString::number(value));
}

void CanvasSettingPanel::setForeground(QColor value) {
  ui->colorChooserForeground->setColor(value);
}

void CanvasSettingPanel::setBackground(QColor value) {
  ui->colorChooserBackground->setColor(value);
}

void CanvasSettingPanel::setCrossed(bool value) {
  ui->checkBoxCrossed->setChecked(value);
}

void CanvasSettingPanel::on_lineEditWidth_editingFinished() {
  if (image != nullptr) {
    image->setWidth(getWidth());
    emit canvasChanged();
  }
}

void CanvasSettingPanel::on_lineEditHeight_editingFinished() {
  if (image != nullptr) {
    image->setWidth(getHeight());
    emit canvasChanged();
  }
}

void CanvasSettingPanel::on_lineEditGrainSize_editingFinished() {
  if (image != nullptr) {
    image->setGrainSize(getGrainSize());
    emit canvasChanged();
  }
}

void CanvasSettingPanel::on_colorChooserForeground_colorChanged(
    const QColor& color) {
  if (image != nullptr) {
    image->setForeground(color);
    emit canvasChanged();
  }
}

void CanvasSettingPanel::on_colorChooserBackground_colorChanged(
    const QColor& color) {
  if (image != nullptr) {
    image->setBackground(color);
    emit canvasChanged();
  }
}

void CanvasSettingPanel::on_checkBoxCrossed_stateChanged(int state) {
  if (image != nullptr) {
    switch (state) {
      case Qt::CheckState::Checked:
        image->setCrossedParity(true);
        break;
      case Qt::CheckState::Unchecked:
        image->setCrossedParity(false);
        break;
      default:
        throw "Unexpected state from checkBoxCrossed";
    }
    emit canvasChanged();
  }
}
