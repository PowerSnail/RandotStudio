#include "mainwindow.h"

#include <QFileDialog>
#include <QGraphicsDropShadowEffect>
#include <QImage>
#include <QPainter>
#include <optional>

#include "../imaging/imaging.h"
#include "../utils/logging.h"
#include "../utils/widget_helper.h"
#include "dialogabout.h"
#include "fstream"
#include "ui_mainwindow.h"

using std::optional;
using namespace logging;
using namespace widget_helper;

const QString kImageFileFilter(
    "Images (*.png *.jpg *.jpeg *.PNG *.JPG *.JPEG)");

const QString kJsonFileFilter("JSON File (*.json)");
const QString kPNSFileFilter("Stereo Image File (*.PNS)");

/*
 * MainWindow Implementation
 * -------------------------
 */

Target MainWindow::kNoTarget(0, 0, 0, 0, 0, 0, QColor(0, 0, 0));

/**
 * @brief Construct a new Main Window:: Main Window object
 *
 * @param img the data object for this canvas
 * @param parent
 */
MainWindow::MainWindow(StereoImage& img, QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      image(img),
      selectionSquare(),
      loadedShapes(),
      prevShapeDir("") {
  
  ui->setupUi(this);

  ui->widgetPreviewPanel->setBackgroundRole(QPalette::Dark);
  ui->labelPreview->setScaledContents(true);
  QGraphicsDropShadowEffect* shadow =
      new QGraphicsDropShadowEffect(ui->labelPreview);
  shadow->setBlurRadius(kPreviewShadowSize);
  shadow->setXOffset(0);
  shadow->setYOffset(0);
  ui->labelPreview->setGraphicsEffect(shadow);

  ui->targetSettingPanel->setVisible(false);
  loadCanvasProperties();

  selectionSquare.setParent(ui->labelPreview);
  selectionSquare.setVisible(false);
  selectionSquare.setStyleSheet("border: 1px solid blue; background-color: rgba(0,0,255,50);");
}

MainWindow::~MainWindow() { delete ui; }

int MainWindow::getCurrentTargetId() { return ui->listWidgetTarget->currentRow(); }

void MainWindow::setCurrentTargetId(int value) {
  ui->listWidgetTarget->setCurrentRow(value);
}

Target& MainWindow::currentTarget() {
  int id = getCurrentTargetId();
  if (id < 0) {
    return kNoTarget;
  }
  return image.targetList[id];
}


void MainWindow::loadCanvasProperties() {
  *ui->lineEditWidth << image.width;
  *ui->lineEditHeight << image.height;
  *ui->lineEditGrainSize << image.grainSize;
  ui->colorChooserForeground->setColor(image.foreground);
  ui->colorChooserBackground->setColor(image.background);
  ui->checkBoxCrossed->setChecked(image.crossedParity);

  loadedShapes.clear();
  ui->listWidgetShape->clear();
  for (auto shapeFile : image.shapeList) {
    loadShape(shapeFile);
  }

  ui->listWidgetTarget->clear();
  for (auto& target : image.targetList) {
    insertIconItem(*ui->listWidgetTarget, getTargetPreview(target),
                   ui->listWidgetTarget->count() - 1);
  }
}

/**
 * @brief Load a shape from an image file and add to shape list
 *
 * @param filename
 */
void MainWindow::loadShape(QString filename) {
  if (loadedShapes.find(filename) != loadedShapes.end()) {
    throw "filename already loaded";
  }
  QPixmap shape =
      QPixmap::fromImage(QImage(filename).convertToFormat(QImage::Format_Mono));
  loadedShapes[filename] = shape;

  insertIconItem(*ui->listWidgetShape, shape, loadedShapes.size() - 1);
}


void MainWindow::refreshCurrentTarget() {
  Target& target = currentTarget();
  if (&target == &kNoTarget) {
    return;
  }
  ui->targetSettingPanel->setVisible(true);
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

  ui->colorChooserTarget->setColor(target.color);
  ui->listWidgetShape->setCurrentRow(target.shapeID);
  ui->listWidgetTarget->item(getCurrentTargetId())->setIcon(
    QIcon(getTargetPreview(target).scaled(16, 16)));
}

QPixmap& MainWindow::getShape(int id) {
  return loadedShapes[image.shapeList[id]];
}

Target& MainWindow::getTarget(int id) { return image.targetList[id]; }

QPixmap MainWindow::getTargetPreview(Target& target) {
  auto shape =
      getShape(target.shapeID).transformed(QTransform().rotate(target.rotate));
  auto preview = imaging::renderShapePreview(shape, target.color);
  return preview;
}

void MainWindow::renderPreview() {
  QPixmap preview = imaging::renderPreview(this->image);
  QSize sizeConstraint = ui->widgetPreviewPanel->size() / 5 * 4;
  QSize size = preview.size().scaled(sizeConstraint, Qt::KeepAspectRatio);

  ui->labelPreview->resize(size);
  ui->labelPreview->move(
    (ui->widgetPreviewPanel->width() - ui->labelPreview->width()) / 2,
    (ui->widgetPreviewPanel->height() - ui->labelPreview->height()) / 2
  );
  ui->labelPreview->setPixmap(preview);
  showSelectionSquare();
}

void MainWindow::showSelectionSquare() {
  if (getCurrentTargetId() == -1) {
    return;
  }
  auto& target = currentTarget();  
  double scaleX = (ui->labelPreview->width() - 2 * kPreviewShadowSize) * 1.0 / image.width;
  double scaleY = (ui->labelPreview->height() - 2 * kPreviewShadowSize) * 1.0 / image.height;
  std::cerr << "scale = " << scaleX << "," << scaleY << std::endl;

  int x = kPreviewShadowSize + int(target.x * scaleX);
  int y = kPreviewShadowSize + int(target.y * scaleY);
  std::cerr << "poop pos = " << x << "," <<  y << std::endl;

  int width = int(scaleX * target.scale * getShape(target.shapeID).width()) + 1;
  int height = int(scaleY * target.scale * getShape(target.shapeID).height()) + 1;
  std::cerr << "poop size = " << width << "," <<  height << std::endl;

  selectionSquare.hide();
  selectionSquare.move(x, y);
  selectionSquare.resize(width, height);
  selectionSquare.setVisible(true);
  selectionSquare.show();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
  QMainWindow::resizeEvent(event);
  renderPreview();
}

void MainWindow::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);
  renderPreview();
}

/**
 * Slots
 */

// ## Target List Slots
void MainWindow::on_listWidgetTarget_currentRowChanged(int currentRow) {
  if (currentRow == -1) {
    ui->targetSettingPanel->setVisible(false);
  }
  refreshCurrentTarget();
  showSelectionSquare();
}

void MainWindow::on_btnAddTarget_clicked() {
  Target target(0, 0, 1, 0, 0, 0, image.foreground);
  image.targetList.push_back(target);
  int targetId = image.targetList.size() - 1;
  insertIconItem(*ui->listWidgetTarget, getTargetPreview(target), targetId);
  ui->listWidgetTarget->setCurrentRow(targetId);
  renderPreview();
}

void MainWindow::on_btnRemoveTarget_clicked() {
  auto selectedId = listWidgetRemoveCurrentRow(*ui->listWidgetTarget);
  if (selectedId < 0) {
    return;
  }
  image.targetList.erase(image.targetList.begin() + selectedId);
  renderPreview();
}

// ## Shape List Slots
void MainWindow::on_btnAddShape_clicked() {
  QStringList fileList = QFileDialog::getOpenFileNames(
      this, tr("Choose shape images"), prevShapeDir, kImageFileFilter);
  if (fileList.empty()) {
    return;
  }
  prevShapeDir = QFileInfo(fileList[0]).dir().path();

  for (auto& filename : fileList) {
    image.shapeList.push_back(filename);
    loadShape(filename);
  }
}

void MainWindow::on_btnRemoveShape_clicked() {
  auto selectedShapeId = ui->listWidgetShape->currentRow();
  if (selectedShapeId < 0) {
    return;
  }
  if (selectedShapeId < 2) {
    ui->statusbar->showMessage(tr("Cannot delete E and C shapes."));
    return;
  }
  loadedShapes.erase(image.shapeList[selectedShapeId]);
  image.shapeList.erase(image.shapeList.begin() + selectedShapeId);
  for (auto& target : image.targetList) {
    if (target.shapeID == selectedShapeId) {
      target.shapeID = 0;
    } else if (target.shapeID > selectedShapeId) {
      target.shapeID -= 1;
    }
  }
  loadCanvasProperties();
  refreshCurrentTarget();
  renderPreview();
}

// Slots for canvas properties
void MainWindow::on_lineEditWidth_editingFinished() {
  image.width = editGetInt(*ui->lineEditWidth);
  renderPreview();
}

void MainWindow::on_lineEditHeight_editingFinished() {
  image.height = editGetInt(*ui->lineEditHeight);
  renderPreview();
}

void MainWindow::on_lineEditGrainSize_editingFinished() {
  image.grainSize = editGetInt(*ui->lineEditGrainSize);
}

void MainWindow::on_colorChooserForeground_colorChanged(const QColor& color) {
  image.foreground = ui->colorChooserForeground->getColor();
}

void MainWindow::on_colorChooserBackground_colorChanged(const QColor& color) {
  image.background = ui->colorChooserBackground->getColor();
  renderPreview();
}

void MainWindow::on_checkBoxCrossed_stateChanged(int state) {
  image.crossedParity = state == Qt::CheckState::Checked;
}

// Slots for target properties
void MainWindow::on_lineEditX_editingFinished() {
  int value = editGetInt(*ui->lineEditX);
  Target& target = currentTarget();
  if (&target != &kNoTarget && value != target.x) {
    target.x = value;
    renderPreview();
  }
}

void MainWindow::on_lineEditY_editingFinished() {
  int value = editGetInt(*ui->lineEditY);
  Target& target = currentTarget();
  if (&target != &kNoTarget && value != target.y) {
    target.y = value;
    renderPreview();
  }
}

void MainWindow::on_lineEditScale_editingFinished() {
  int value = editGetInt(*ui->lineEditScale);
  Target& target = currentTarget();
  if (&target != &kNoTarget && value != target.scale) {
    target.scale = value;
    renderPreview();
  }
}

void MainWindow::on_lineEditParity_editingFinished() {
  int value = editGetInt(*ui->lineEditParity);
  Target& target = currentTarget();
  if (&target != &kNoTarget && value != target.parity) {
    target.parity = value;
    renderPreview();
  }
}

void MainWindow::on_comboBoxRotate_currentIndexChanged(int index) {
  int value = getRotateAngle(index);
  Target& target = currentTarget();
  if (&target != &kNoTarget && value != target.rotate) {
    target.rotate = value;
    ui->listWidgetTarget->item(getCurrentTargetId())
        ->setIcon(QIcon(getTargetPreview(target).scaled(16, 16)));
    renderPreview();
  }
}

void MainWindow::on_colorChooserTarget_colorChanged(const QColor& color) {
  Target& target = currentTarget();
  if (&target != &kNoTarget && color != target.color) {
    target.color = color;
    setImageToItem(*ui->listWidgetTarget->item(getCurrentTargetId()), getTargetPreview(target));
    renderPreview();
  }
}

void MainWindow::on_listWidgetShape_currentRowChanged(int currentRow) {
  Target& target = currentTarget();
  if (&target == &kNoTarget || target.shapeID == currentRow) {
    return;
  }
  target.shapeID = currentRow;
  refreshCurrentTarget();
  renderPreview();
}

void MainWindow::on_actionAbout_triggered() {
  DialogAbout about(this);
  about.exec();
}

void MainWindow::on_actionSave_triggered() {
  QString filename = QFileDialog::getSaveFileName(
      this, tr("Choose file to save"), prevShapeDir, kJsonFileFilter);
  image.saveToFile(filename);
}

void MainWindow::on_actionLoad_triggered() {
  QString filename = QFileDialog::getOpenFileName(
      this, tr("Choose file to save"), prevShapeDir, kJsonFileFilter);
  image.loadFromFile(filename);
  loadCanvasProperties();
  renderPreview();
}

void MainWindow::on_actionRandot_triggered() {
  auto canvas = imaging::renderRandot(this->image);
  QString filename = QFileDialog::getSaveFileName(
    this, tr("Choose file to save"), prevShapeDir, kPNSFileFilter
  );
  canvas.save(filename, "PNG", 100);

}

void MainWindow::on_actionStereo_PNG_triggered() {
  std::cerr << "on_actionStereo_PNG_triggered" << std::endl;
  auto canvas = imaging::renderStereo(this->image);
  std::cerr << "canvas returned" << std::endl;

  QString filename = QFileDialog::getSaveFileName(
    this, tr("Choose file to save"), prevShapeDir, kPNSFileFilter
  );
  canvas.save(filename, "PNG", 100);
}
