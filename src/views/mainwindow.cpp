// Copyright (c) 2019 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#include "mainwindow.h"

#include <QBitmap>
#include <QFileDialog>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPixmap>
#include <QtDebug>
#include <fstream>
#include <optional>

#include "../imaging/imaging.h"
#include "../utils/widget_helper.h"
#include "dialogabout.h"
#include "ui_mainwindow.h"

using std::optional;
using namespace widget_helper;

const QString kImageFileFilter("Images (*.png *.jpg *.jpeg *.PNG *.JPG *.JPEG)");

const QString kJsonFileFilter("JSON File (*.json)");
const QString kPNSFileFilter("Stereo Image File (*.PNS)");

/*
 * MainWindow Implementation
 * -------------------------
 */

/**
 * @brief Construct a new Main Window:: Main Window object
 *
 * @param img the data object for this canvas
 * @param parent
 */
MainWindow::MainWindow(MainWindowViewModel *vm, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), vm(vm) {
  ui->setupUi(this);
  ui->targetSettingPanel->setVisible(false);
  loadCanvasProperties();

  connect(vm, &MainWindowViewModel::currentTargetIDChanged, this,
          &MainWindow::on_vm_currentTargetIDChanged);
  connect(vm, &MainWindowViewModel::currentShapeIDChanged, this,
          &MainWindow::on_vm_currentShapeIDChanged);
  connect(vm, &MainWindowViewModel::targetCreated, this, &MainWindow::on_vm_targetCreated);
  connect(vm, &MainWindowViewModel::targetUpdated, this, &MainWindow::on_vm_targetUpdated);
  connect(vm, &MainWindowViewModel::targetRemoved, this, &MainWindow::on_vm_targetRemoved);
  connect(vm, &MainWindowViewModel::shapeLoaded, this, &MainWindow::on_vm_shapeLoaded);
  connect(vm, &MainWindowViewModel::shapeRemoved, this, &MainWindow::on_vm_shapeRemoved);
  connect(vm, &MainWindowViewModel::canvasUpdated, this, &MainWindow::on_vm_canvasUpdated);

  for (int i = 0; i < MainWindowViewModel::kDefaultShapeCount; ++i) {
    on_vm_shapeLoaded(i);
  }

  ui->retranslateUi(this);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::loadCanvasProperties() {
  *ui->lineEditWidth << vm->getCanvas().width;
  *ui->lineEditHeight << vm->getCanvas().height;
  *ui->lineEditGrainSize << vm->getCanvas().grainSize;
  ui->colorChooserForeground->setColor(vm->getCanvas().foreground);
  ui->colorChooserBackground->setColor(vm->getCanvas().background);
  ui->checkBoxCrossed->setChecked(vm->getCanvas().crossedParity);
  ui->previewCanvas->setCanvasSize(vm->getCanvas().width, vm->getCanvas().height);
  ui->previewCanvas->setBackground(vm->getCanvas().background);
}

void MainWindow::loadTargetProperties() {
  int targetID = vm->getCurrentTargetID();
  if (targetID == -1) {
    return;
  }
  const Target &target = vm->getTarget(targetID);
  *ui->lineEditX << target.x;
  *ui->lineEditY << target.y;
  *ui->lineEditScale << target.scale;
  *ui->lineEditParity << target.parity;

  switch (target.rotate) {
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
  auto &shape = vm->getShape(target.shapeID);
  ui->labelRealSize->setText(
      QString("(%1 x %2)").arg(target.scale * shape.width()).arg(target.scale * shape.height()));
  ui->colorChooserTarget->setColor(target.color);
  ui->listWidgetShape->setCurrentRow(target.shapeID);
}

QPixmap MainWindow::getTargetPreview(const Target &target) {
  auto mask = imaging::targetMask(vm->getShape(target.shapeID), target);
  QPixmap targetview(mask.size());
  targetview.fill(target.color);
  targetview.setMask(mask);
  return targetview;
}
/**
 * Slots
 */

// ## Target List Slots
void MainWindow::on_listWidgetTarget_currentRowChanged(int currentRow) {
  vm->setCurrentTargetID(currentRow);
}

void MainWindow::on_btnAddTarget_clicked() {
  vm->createTarget(Target(0, 0, 1, 0, 0, 0, vm->getCanvas().foreground));
}

void MainWindow::on_btnRemoveTarget_clicked() {
  int currentID = vm->getCurrentTargetID();
  if (currentID != -1) {
    vm->removeTarget(currentID);
  }
}

// ## Shape List Slots
void MainWindow::on_listWidgetShape_currentRowChanged(int currentRow) {
  vm->setCurrentShapeID(currentRow);
}

void MainWindow::on_btnAddShape_clicked() {
  QStringList fileList = QFileDialog::getOpenFileNames(this, tr("Choose shape images"),
                                                       vm->getPrevShapeDir(), kImageFileFilter);
  if (fileList.empty()) {
    return;
  }
  vm->setPrevShapeDir(QFileInfo(fileList[0]).dir().path());
  for (auto &filename : fileList) {
    vm->loadShape(filename);
  }
}

void MainWindow::on_btnRemoveShape_clicked() {
  int currentID = vm->getCurrentShapeID();
  if (currentID == -1) {
    return;
  }
  if (currentID < 2) {
    ui->statusbar->showMessage(tr("Cannot delete E and C shapes."));
    return;
  }
  vm->removeShape(currentID);
}

// Slots for canvas properties
void MainWindow::on_lineEditWidth_editingFinished() {
  vm->updateCanvas(Canvas::Property::Width, editGetInt(*ui->lineEditWidth));
}

void MainWindow::on_lineEditHeight_editingFinished() {
  vm->updateCanvas(Canvas::Property::Height, editGetInt(*ui->lineEditHeight));
}

void MainWindow::on_lineEditGrainSize_editingFinished() {
  vm->updateCanvas(Canvas::Property::GrainSize, editGetInt(*ui->lineEditGrainSize));
}

void MainWindow::on_colorChooserForeground_colorChanged(const QColor &color) {
  vm->updateCanvas(Canvas::Property::Foreground, color);
}

void MainWindow::on_colorChooserBackground_colorChanged(const QColor &color) {
  vm->updateCanvas(Canvas::Property::Background, color);
}

void MainWindow::on_checkBoxCrossed_stateChanged(int state) {
  vm->updateCanvas(Canvas::Property::CrossedParity, state == Qt::CheckState::Checked);
}

// Slots for target properties
void MainWindow::on_lineEditX_editingFinished() {
  int value = editGetInt(*ui->lineEditX);
  vm->updateTarget(vm->getCurrentTargetID(), Target::Property::X, value);
}

void MainWindow::on_lineEditY_editingFinished() {
  int value = editGetInt(*ui->lineEditY);
  vm->updateTarget(vm->getCurrentTargetID(), Target::Property::Y, value);
}

void MainWindow::on_lineEditScale_editingFinished() {
  int value = editGetInt(*ui->lineEditScale);
  vm->updateTarget(vm->getCurrentTargetID(), Target::Property::Scale, value);
}

void MainWindow::on_lineEditParity_editingFinished() {
  int value = editGetInt(*ui->lineEditParity);
  vm->updateTarget(vm->getCurrentTargetID(), Target::Property::Parity, value);
}

void MainWindow::on_comboBoxRotate_currentIndexChanged(int index) {
  int value = getRotateAngle(index);
  vm->updateTarget(vm->getCurrentTargetID(), Target::Property::Rotate, value);
}

void MainWindow::on_colorChooserTarget_colorChanged(const QColor &color) {
  vm->updateTarget(vm->getCurrentTargetID(), Target::Property::Color, color);
}

void MainWindow::on_previewCanvas_currentIndexChanged(int index) {
  vm->setCurrentTargetID(index);
}

// VM signals
void MainWindow::on_vm_currentTargetIDChanged(int oldID, int newID) {
  qDebug() << "on_vm_currentTargetIDChanged " << oldID << "," << newID;
  ui->listWidgetTarget->setCurrentRow(newID);
  ui->previewCanvas->setCurrentIndex(newID);
  if (newID == -1) {
    ui->targetSettingPanel->setVisible(false);
  } else {
    ui->targetSettingPanel->setVisible(true);
    loadTargetProperties();
  }
}

void MainWindow::on_vm_currentShapeIDChanged(int,  // Ignore oldID
                                             int newID) {
  if (vm->getCurrentTargetID() == -1) {
    return;
  }
  ui->listWidgetShape->setCurrentRow(newID);
}

void MainWindow::on_vm_targetCreated(int targetID) {
  auto target = vm->getTarget(targetID);
  auto targetIcon = getTargetPreview(target);
  insertIconItem(*ui->listWidgetTarget, targetIcon, targetID);

  ui->previewCanvas->insertPixmap(targetID, target.x, target.y, targetIcon);
  vm->setCurrentTargetID(targetID);
}

void MainWindow::on_vm_targetUpdated(int targetID, Target::Property pname) {
  auto target = vm->getTarget(targetID);
  if (targetID == vm->getCurrentTargetID()) {
    loadTargetProperties();
  }
  QPixmap preview;
  switch (pname) {
    case Target::Property::X:
    case Target::Property::Y:
      ui->previewCanvas->movePixmap(targetID, target.x, target.y);
      break;
    case Target::Property::Color:
    case Target::Property::Scale:
    case Target::Property::ShapeID:
    case Target::Property::Rotate:
      preview = getTargetPreview(target);
      setImageToItem(*ui->listWidgetTarget->item(targetID), preview);
      ui->previewCanvas->replacePixmap(targetID, preview);
      break;
    case Target::Property::Parity:
      qDebug() << "Parity does not affect list nor preview.";
      break;
  }
}

void MainWindow::on_vm_targetRemoved(int targetID) {
  qDebug() << "on_vm_targetRemoved" << targetID;

  ui->listWidgetTarget->removeItemWidget(ui->listWidgetTarget->item(targetID));
  delete ui->listWidgetTarget->takeItem(targetID);
  ui->listWidgetTarget->update();
  ui->previewCanvas->removePixmap(targetID);
}

void MainWindow::on_vm_shapeLoaded(int shapeID) {
  insertIconItem(*ui->listWidgetShape, vm->getShape(shapeID), shapeID);
  vm->setCurrentShapeID(shapeID);
}

void MainWindow::on_vm_shapeRemoved(int shapeID) {
  ui->listWidgetShape->removeItemWidget(ui->listWidgetShape->item(shapeID));
}

void MainWindow::on_vm_canvasUpdated(Canvas::Property pname) {
  loadCanvasProperties();
  switch (pname) {
    case Canvas::Property::Width:
    case Canvas::Property::Height:
      ui->previewCanvas->setCanvasSize(vm->getCanvas().width, vm->getCanvas().height);
      break;
    case Canvas::Property::Background:
      ui->previewCanvas->setBackground(vm->getCanvas().background);
      break;
    case Canvas::Property::Foreground:
    case Canvas::Property::CrossedParity:
    case Canvas::Property::GrainSize:
      // Not affecting preview
      break;
  }
}

void MainWindow::on_actionAbout_triggered() {
  DialogAbout about(this);
  about.exec();
}

void MainWindow::on_actionSave_triggered() {
  QString filename = QFileDialog::getSaveFileName(this, tr("Choose file to save"),
                                                  vm->getPrevConfigDir(), kJsonFileFilter);
  if (filename.isNull()) {
    return;
  }
  vm->saveToFile(filename);
}

void MainWindow::on_actionLoad_triggered() {
  QString filename = QFileDialog::getOpenFileName(this, tr("Choose file to save"),
                                                  vm->getPrevConfigDir(), kJsonFileFilter);
  if (filename.isNull()) {
    return;
  }
  vm->loadFromFile(filename);
}

void MainWindow::on_actionRandot_triggered() {
  QString filename = QFileDialog::getSaveFileName(this, tr("Choose file to save"),
                                                  vm->getPrevExportDir(), kPNSFileFilter);
  vm->exportImage(filename, imaging::StereoImageType::Randot);
}

void MainWindow::on_actionStereo_PNG_triggered() {
  QString filename = QFileDialog::getSaveFileName(this, tr("Choose file to save"),
                                                  vm->getPrevExportDir(), kPNSFileFilter);
  vm->exportImage(filename, imaging::StereoImageType::Regular);
}
