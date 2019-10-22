#include "mainwindow.h"
#include "ui_mainwindow.h"

const QVector<QRgb> kDefaultColorTable{QColor(255, 255, 255).rgb(),
                                       QColor(0, 0, 0).rgb()};

QImage renderShapePreview(const Shape& shape) {
  QImage image(&shape.getData()[0], shape.getSize(), shape.getSize(),
               shape.getSize(), QImage::Format_Indexed8);
  image.setColorTable(kDefaultColorTable);
  return image;
}

QImage renderTargetPreview(const Shape& shape, const Target& target,
                           const QColor& background) {
  QImage image =
      renderShapePreview(shape).transformed(QTransform().rotate(target.rotate));
  image.setColorTable(QVector<QRgb>({background.rgb(), target.color.rgb()}));
  return image;
}

QIcon imageToIcon(const QImage&& image) {
  return QIcon(QPixmap::fromImage(image));
}

MainWindow::MainWindow(StereoImage& img, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), image(img) {
  ui->setupUi(this);

  ui->targetSettingPanel->setVisible(false);
  ui->canvasSettingPanel->setWidth(img.getWidth());
  ui->canvasSettingPanel->setHeight(img.getHeight());
  ui->canvasSettingPanel->setForeground(img.getForeground());
  ui->canvasSettingPanel->setBackground(img.getBackground());
  ui->canvasSettingPanel->setCrossed(img.getCrossedParity());

  for (auto& shape : img.getShapeList()) {
    QListWidgetItem shapeItem(ui->listWidgetShape);
    shapeItem.setIcon(imageToIcon(renderShapePreview(shape)));
  }

  for (auto& target : img.getTargetList()) {
    QListWidgetItem targetItem(ui->listWidgetTarget);
    Shape shape = img.getShapeList()[static_cast<size_t>(target.shapeID)];
    targetItem.setIcon(imageToIcon(renderTargetPreview(
        img.getShapeList()[static_cast<size_t>(target.shapeID)], target,
        img.getBackground())));
  }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_listWidgetTarget_currentRowChanged(int currentRow) {
  if (currentRow == -1) {
    ui->targetSettingPanel->setVisible(false);
  }
  auto& p = ui->targetSettingPanel;
  auto& target = this->image.getTargetList()[static_cast<size_t>(currentRow)];
  p->bindTarget(&target);
}

void MainWindow::on_targetSettingPanel_targetModified(const Target& target) {
  ui->statusbar->showMessage("Target modified");
}

void MainWindow::on_canvasSettingPanel_canvasChanged() {
  ui->statusbar->showMessage("Target modified");
}
