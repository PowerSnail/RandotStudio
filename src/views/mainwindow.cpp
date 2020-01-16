// Copyright (c) 2019 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#include "src/views/mainwindow.h"

#include <QBitmap>
#include <QFileDialog>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPixmap>
#include <QtDebug>
#include <fstream>
#include <optional>

#include "../imaging/imaging.h"
#include "dialogabout.h"
#include "ui_mainwindow.h"

using std::optional;

namespace {
static const QString kImageFileFilter(
    "Images (*.bmp *.png *.jpg *.jpeg *.PNG *.BMP *.JPG *.JPEG)");

static const QString kJsonFileFilter("JSON File (*.json)");
static const QString kPNSFileFilter("Stereo Image File (*.PNS *.pns)");

static void SetImageToItem(QListWidgetItem &item, QPixmap image) {
  item.setIcon(QIcon(image.scaled(15, 15, Qt::AspectRatioMode::KeepAspectRatio)));
}

static void InsertIconItem(QListWidget &listWidget, QPixmap image, int index) {
  QListWidgetItem *item = new QListWidgetItem();
  SetImageToItem(*item, std::move(image));
  item->setSizeHint(QSize(20, 20));
  listWidget.insertItem(index, item);
}
}  // namespace

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
    : QMainWindow(parent), vm(vm), ui(new Ui::MainWindow) {
  vm->setParent(this);
  ui->setupUi(this);
  ui->targetSettingPanel->setEnabled(false);
  LoadCanvasProperties();

  for (int i = 0; i < MainWindowViewModel::kDefaultShapeCount; ++i) {
    on_vm_shapeLoaded(i);
  }

  ui->retranslateUi(this);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::LoadCanvasProperties() {
  auto &canvas = vm->getCanvas();
  ui->lineEditWidth->setText(QString::number(canvas.width));
  ui->lineEditHeight->setText(QString::number(canvas.height));
  ui->lineEditGrainSize->setText(QString::number(canvas.grainSize));
  ui->lineEditGrainRatio->setText(QString::number(canvas.grainRatio));
  ui->colorChooserForeground->SetColor(canvas.foreground);
  ui->colorChooserBackground->SetColor(canvas.background);
  ui->checkBoxCrossed->setChecked(canvas.crossedParity);
  ui->previewCanvas->SetCanvasSize(canvas.width, canvas.height);
  ui->previewCanvas->SetBackground(canvas.background);
}

void MainWindow::LoadTargetProperties() {
  int targetID = vm->CurrentTargetID();
  if (targetID == -1) {
    return;
  }
  const Target &target = vm->GetTarget(targetID);
  ui->lineEditX->setText(QString::number(target.x));
  ui->lineEditY->setText(QString::number(target.y));
  ui->lineEditScale->setText(QString::number(target.scale));
  ui->lineEditParity->setText(QString::number(target.parity));

  switch (target.rotate) {
    case 0:
      ui->radioButtonRotate0->setChecked(true);
      break;
    case 90:
      ui->radioButtonRotate90->setChecked(true);
      break;
    case 180:
      ui->radioButtonRotate180->setChecked(true);
      break;
    case 270:
      ui->radioButtonRotate270->setChecked(true);
      break;
    default:
      throw "Unexpected angle for rotation";
  }
  auto &shape = vm->getShape(target.shapeID);
  ui->labelRealSize->setText(QString("Size = (%1 x %2)")
                                 .arg(target.scale * shape.width())
                                 .arg(target.scale * shape.height()));
  ui->colorChooserTarget->SetColor(target.color);
  ui->listWidgetShape->setCurrentRow(target.shapeID);
}

QPixmap MainWindow::GetTargetPreview(const Target &target) {
  auto mask = imaging::TargetMask(vm->getShape(target.shapeID), target);
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
  vm->SetCurrentTargetID(currentRow);
}

void MainWindow::on_btnAddTarget_clicked() {
  vm->CreateTarget(Target(0, 0, 10, 0, 0, 0, vm->getCanvas().foreground));
}

void MainWindow::on_btnRemoveTarget_clicked() {
  int currentID = vm->CurrentTargetID();
  if (currentID != -1) {
    vm->RemoveTarget(currentID);
  }
}

// ## Shape List Slots
void MainWindow::on_listWidgetShape_currentRowChanged(int currentRow) {
  vm->SetCurrentShapeID(currentRow);
}

void MainWindow::on_btnAddShape_clicked() {
  QStringList fileList = QFileDialog::getOpenFileNames(
      this, tr("Choose shape images"), vm->PrevShapeDir(), kImageFileFilter);
  if (fileList.empty()) {
    return;
  }
  vm->SetPrevShapeDir(QFileInfo(fileList[0]).dir().path());
  for (auto &filename : fileList) {
    vm->LoadShape(filename);
  }
}

void MainWindow::on_btnRemoveShape_clicked() {
  int currentID = vm->CurrentShapeID();
  if (currentID == -1) {
    return;
  }
  if (currentID < 2) {
    ui->statusbar->showMessage(tr("Cannot delete E and C shapes."));
    return;
  }
  vm->RemoveShape(currentID);
}

// Slots for canvas properties
void MainWindow::on_lineEditWidth_editingFinished() {
  int value = ui->lineEditWidth->text().toInt();
  vm->SetCanvasWidth(value);
}

void MainWindow::on_lineEditHeight_editingFinished() {
  int value = ui->lineEditHeight->text().toInt();
  vm->SetCanvasHeight(value);
}

void MainWindow::on_lineEditGrainSize_editingFinished() {
  int value = ui->lineEditGrainSize->text().toInt();
  vm->SetCanvasGrainSize(value);
}

void MainWindow::on_lineEditGrainRatio_editingFinished() {
  double value = ui->lineEditGrainRatio->text().toDouble();
  vm->SetCanvasGrainRatio(value);
}

void MainWindow::on_colorChooserForeground_colorChanged(const QColor &color) {
  vm->SetCanvasForeground(color);
}

void MainWindow::on_colorChooserBackground_colorChanged(const QColor &color) {
  vm->SetCanvasBackground(color);
}

void MainWindow::on_checkBoxCrossed_stateChanged(int state) {
  vm->SetCanvasCrossedParity(state == Qt::CheckState::Checked);
}

// Slots for target properties
void MainWindow::on_lineEditX_editingFinished() {
  int value = ui->lineEditX->text().toInt();
  vm->SetTargetX(vm->CurrentTargetID(), value);
}

void MainWindow::on_lineEditY_editingFinished() {
  int value = ui->lineEditY->text().toInt();
  vm->SetTargetY(vm->CurrentTargetID(), value);
}

void MainWindow::on_lineEditScale_editingFinished() {
  int value = ui->lineEditScale->text().toInt();
  vm->SetTargetScale(vm->CurrentTargetID(), value);
}

void MainWindow::on_lineEditParity_editingFinished() {
  int value = ui->lineEditParity->text().toInt();
  vm->SetTargetParity(vm->CurrentTargetID(), value);
}

void MainWindow::on_colorChooserTarget_colorChanged(const QColor &color) {
  vm->SetTargetColor(vm->CurrentTargetID(), color);
}

void MainWindow::on_radioButtonRotate0_toggled(bool checked) {
  if (!checked) {
    return;
  }
  vm->SetTargetRotate(vm->CurrentTargetID(), 0);
}

void MainWindow::on_radioButtonRotate90_toggled(bool checked) {
  if (!checked) {
    return;
  }
  vm->SetTargetRotate(vm->CurrentTargetID(), 90);
}

void MainWindow::on_radioButtonRotate180_toggled(bool checked) {
  if (!checked) {
    return;
  }
  vm->SetTargetRotate(vm->CurrentTargetID(), 180);
}

void MainWindow::on_radioButtonRotate270_toggled(bool checked) {
  if (!checked) {
    return;
  }
  vm->SetTargetRotate(vm->CurrentTargetID(), 270);
}

void MainWindow::on_previewCanvas_currentIndexChanged(int index) {
  vm->SetCurrentTargetID(index);
}

// VM signals
void MainWindow::on_vm_currentTargetIDChanged(int oldID, int newID) {
  qDebug() << "on_vm_currentTargetIDChanged " << oldID << "," << newID;
  ui->listWidgetTarget->setCurrentRow(newID);
  ui->previewCanvas->SetCurrentIndex(newID);
  if (newID != -1) {
    ui->targetSettingPanel->setEnabled(true);
    LoadTargetProperties();
  } else {
    ui->targetSettingPanel->setEnabled(false);
  }
}

void MainWindow::on_vm_currentShapeIDChanged(int,  // Ignore oldID
                                             int newID) {
  if (vm->CurrentTargetID() == -1) {
    return;
  }
  ui->listWidgetShape->setCurrentRow(newID);
}

void MainWindow::on_vm_targetCreated(int targetID) {
  auto target = vm->GetTarget(targetID);
  auto targetIcon = GetTargetPreview(target);
  InsertIconItem(*ui->listWidgetTarget, targetIcon, targetID);
  ui->listWidgetTarget->item(targetID)->setText(
      QString(" %1x%2").arg(target.x).arg(target.y));

  ui->previewCanvas->InsertPixmap(targetID, target.x, target.y, targetIcon);
  vm->SetCurrentTargetID(targetID);
}

void MainWindow::on_vm_targetUpdated(int targetID, Target::Property pname) {
  auto target = vm->GetTarget(targetID);
  if (targetID == vm->CurrentTargetID()) {
    LoadTargetProperties();
  }
  QPixmap preview;
  switch (pname) {
    case Target::Property::X:
    case Target::Property::Y:
      ui->previewCanvas->MovePixmap(targetID, target.x, target.y);
      ui->listWidgetTarget->item(targetID)->setText(
          QString(" %1x%2").arg(target.x).arg(target.y));
      break;
    case Target::Property::Color:
    case Target::Property::Scale:
    case Target::Property::ShapeID:
    case Target::Property::Rotate:
      preview = GetTargetPreview(target);
      SetImageToItem(*ui->listWidgetTarget->item(targetID), preview);
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
  ui->previewCanvas->RemovePixmap(targetID);
}

void MainWindow::on_vm_shapeLoaded(int shapeID) {
  InsertIconItem(*ui->listWidgetShape, vm->getShape(shapeID), shapeID);
  vm->SetCurrentShapeID(shapeID);
}

void MainWindow::on_vm_shapeRemoved(int shapeID) {
  ui->listWidgetShape->removeItemWidget(ui->listWidgetShape->item(shapeID));
}

void MainWindow::on_vm_canvasUpdated(Canvas::Property pname) {
  LoadCanvasProperties();
  switch (pname) {
    case Canvas::Property::Width:
    case Canvas::Property::Height:
      ui->previewCanvas->SetCanvasSize(vm->getCanvas().width, vm->getCanvas().height);
      break;
    case Canvas::Property::Background:
      ui->previewCanvas->SetBackground(vm->getCanvas().background);
      break;
    case Canvas::Property::Foreground:
    case Canvas::Property::CrossedParity:
    case Canvas::Property::GrainSize:
    case Canvas::Property::GrainRatio:
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
                                                  vm->PrevConfigDir(), kJsonFileFilter);
  if (filename.isNull()) {
    return;
  }
  if (!filename.endsWith(".json")) {
    filename.append(".json");
  }
  vm->SaveToFile(filename);
}

void MainWindow::on_actionLoad_triggered() {
  QString filename = QFileDialog::getOpenFileName(this, tr("Choose file to save"),
                                                  vm->PrevConfigDir(), kJsonFileFilter);
  if (filename.isNull()) {
    return;
  }
  vm->LoadFromFile(filename);
}

void MainWindow::on_actionRandot_triggered() {
  QString filename = QFileDialog::getSaveFileName(this, tr("Choose file to save"),
                                                  vm->PrevExportDir(), kPNSFileFilter);
  if (filename.isNull()) {
    return;
  }
  if (!filename.endsWith(".pns")) {
    filename.append(".pns");
  }
  vm->ExportImage(filename, imaging::StereoImageType::Randot);
}

void MainWindow::on_actionStereo_PNG_triggered() {
  QString filename = QFileDialog::getSaveFileName(this, tr("Choose file to save"),
                                                  vm->PrevExportDir(), kPNSFileFilter);
  if (filename.isNull()) {
    return;
  }
  if (!filename.endsWith(".pns")) {
    filename.append(".pns");
  }
  vm->ExportImage(filename, imaging::StereoImageType::Regular);
}
