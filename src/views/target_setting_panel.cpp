#include "target_setting_panel.h"
#include "ui_target_setting_panel.h"

TargetSettingPanel::TargetSettingPanel(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::TargetSettingPanel),
      coordinateValidator(0, 40000, this),
      parityValidator(-40000, 40000),
      scaleValidator(1, 100),
      target(nullptr) {
  ui->setupUi(this);
  ui->lineEditX->setValidator(&coordinateValidator);
  ui->lineEditY->setValidator(&coordinateValidator);
  ui->lineEditParity->setValidator(&parityValidator);
}

TargetSettingPanel::~TargetSettingPanel() { delete ui; }

void TargetSettingPanel::bindTarget(Target* target) {
  this->target = target;
  setX(target->x);
  setY(target->y);
  setScale(target->scale);
  setRotate(target->rotate);
  setParity(target->parity);
  setShapeId(target->shapeID);
  setColor(target->color);
}

void TargetSettingPanel::on_lineEditX_editingFinished() {
  if (target != nullptr) {
    target->x = getX();
    emit targetModified(*target);
  }
}

void TargetSettingPanel::on_lineEditY_editingFinished() {
  if (target != nullptr) {
    target->y = getY();
    emit targetModified(*target);
  }
}

void TargetSettingPanel::on_lineEditScale_editingFinished() {
  if (target != nullptr) {
    target->scale = getScale();
    emit targetModified(*target);
  }
}

void TargetSettingPanel::on_lineEditParity_editingFinished() {
  if (target != nullptr) {
    target->parity = getParity();
    emit targetModified(*target);
  }
}

void TargetSettingPanel::on_comboBoxRotate_currentIndexChanged(int index) {
  if (target != nullptr) {
    target->rotate = getRotate();
    emit targetModified(*target);
  }
}

void TargetSettingPanel::on_comboBoxShapeChooser_currentIndexChanged(
    int index) {
  if (target != nullptr) {
    target->shapeID = getShapeId();
    emit targetModified(*target);
  }
}

void TargetSettingPanel::on_colorChooser_colorChanged(const QColor& color) {
  if (target != nullptr) {
    target->color = color;
    emit targetModified(*target);
  }
}

int TargetSettingPanel::getX() const { return ui->lineEditX->text().toInt(); }

void TargetSettingPanel::setX(int value) {
  ui->lineEditX->setText(QString::number(value));
}

int TargetSettingPanel::getY() const { return ui->lineEditY->text().toInt(); }

void TargetSettingPanel::setY(int value) {
  ui->lineEditY->setText(QString::number(value));
}

int TargetSettingPanel::getScale() const {
  return ui->lineEditScale->text().toInt();
}

void TargetSettingPanel::setScale(int value) {
  ui->lineEditScale->setText(QString::number(value));
}

int TargetSettingPanel::getRotate() const {
  int selectedId = ui->comboBoxRotate->currentIndex();
  switch (selectedId) {
    case 0:
      return 0;
    case 1:
      return 90;
    case 2:
      return 180;
    case 3:
      return 270;
    default:
      throw "Unexpected selected ID for rotate combobox";
  }
}

void TargetSettingPanel::setRotate(int value) {
  switch (value) {
    case 0:
      ui->comboBoxRotate->setCurrentIndex(0);
      break;
    case 90:
      ui->comboBoxRotate->setCurrentIndex(1);
      break;
    case 180:
      ui->comboBoxRotate->setCurrentIndex(2);
      break;
    case 270:
      ui->comboBoxRotate->setCurrentIndex(4);
      break;
    default:
      throw "Unexpected angle for rotation";
  }
}

int TargetSettingPanel::getParity() const {
  return ui->lineEditParity->text().toInt();
}

void TargetSettingPanel::setParity(int value) {
  ui->lineEditParity->setText(QString::number(value));
}

int TargetSettingPanel::getShapeId() const {
  return ui->comboBoxShapeChooser->currentIndex();
}

void TargetSettingPanel::setShapeId(int value) {
  ui->comboBoxShapeChooser->setCurrentIndex(value);
}

QColor TargetSettingPanel::getColor() const {
  return ui->colorChooser->getColor();
}

void TargetSettingPanel::setColor(const QColor color) {
  ui->colorChooser->setColor(color);
}
