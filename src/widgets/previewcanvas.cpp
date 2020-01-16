// Copyright (c) 2019 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#include "previewcanvas.h"

#include <QGraphicsDropShadowEffect>
#include <QMessageLogger>
#include <QtDebug>

PreviewCanvas::PreviewCanvas(QWidget* parent)
    : QWidget(parent), bgLabel(new QLabel(this)), labelList() {
  bgLabel->setScaledContents(true);
}

void PreviewCanvas::SetCanvasSize(int w, int h) {
  canvasWidth = w;
  canvasHeight = h;

  resizeEvent(nullptr);
  for (auto c : labelList) {
    redrawChild(c);
  }
}

void PreviewCanvas::SetBackground(QColor color) {
  QPixmap pixmap(1, 1);
  pixmap.fill(color);
  bgLabel->setPixmap(pixmap);
  bgLabel->show();
}

int PreviewCanvas::CurrentIndex() {
  return currentIndex;
}
void PreviewCanvas::SetCurrentIndex(int index) {
  qDebug() << "PreviewCanvas::setCurrentIndex " << currentIndex << "->" << index;
  if (index == currentIndex) {
    return;
  }

  if (currentIndex != -1) {
    child(currentIndex)->setSelected(false);
  }

  if (index < 0) {
    return;
  }
  if (static_cast<size_t>(index) >= labelList.size()) {
    throw new std::range_error("Index too large.");
  }
  child(index)->setSelected(true);
  currentIndex = index;
}

void PreviewCanvas::InsertPixmap(int id, int x, int y, const QPixmap& img) {
  PreviewCanvasItem* label = new PreviewCanvasItem(bgLabel);
  labelList.insert(labelList.begin() + id, label);
  label->setPixmap(img);
  label->SetX(x);
  label->SetY(y);
  redrawChild(label);
  connect(label, &PreviewCanvasItem::clicked, this, &PreviewCanvas::on_child_clicked);
}

void PreviewCanvas::replacePixmap(int id, const QPixmap& img) {
  child(id)->setPixmap(img);
  redrawChild(child(id));
}

void PreviewCanvas::MovePixmap(int id, int x, int y) {
  child(id)->setX(x);
  child(id)->setY(y);
  redrawChild(child(id));
}

void PreviewCanvas::RemovePixmap(int id) {
  PreviewCanvasItem* label = child(id);
  labelList.erase(labelList.begin() + id);
  label->setParent(nullptr);
  delete label;
}

QSize PreviewCanvas::PreviewSize() {
  QSize s = bgLabel->size();
  auto effect = bgLabel->graphicsEffect();
  if (effect != nullptr) {
    auto rad = static_cast<QGraphicsDropShadowEffect*>(effect)->blurRadius();
    return s - (QSize(rad, rad) * 2);
  } else {
    return s;
  }
}

void PreviewCanvas::resizeEvent(QResizeEvent* event) {
  QSize contraint = this->size();
  QSizeF renderSize = QSizeF(canvasWidth, canvasHeight)
                          .scaled(QSizeF(contraint) * 4 / 5, Qt::KeepAspectRatio);

  bgLabel->resize(static_cast<int>(renderSize.width()),
                  static_cast<int>(renderSize.height()));
  bgLabel->move(static_cast<int>((contraint.width() - renderSize.width()) / 2),
                static_cast<int>((contraint.height() - renderSize.height()) / 2));

  QWidget::resizeEvent(event);
  for (auto label : labelList) {
    redrawChild(label);
  }
}

int PreviewCanvas::find(PreviewCanvasItem* child) {
  auto it = std::find(labelList.begin(), labelList.end(), child);
  return (it == labelList.end()) ? -1 : it - labelList.begin();
}

void PreviewCanvas::redrawChild(PreviewCanvasItem* child) {
  double scale = static_cast<double>(PreviewSize().width()) / canvasWidth;
  child->move(QPoint(child->X(), child->getY * scale);
  child->resize(child->pixmap()->size() * scale);
  child->show();
}

PreviewCanvasItem* PreviewCanvas::child(int index) {
  if (index < 0 || static_cast<size_t>(index) > labelList.size()) {
    qCritical() << "Index out of range: " << index << ", expecting [0, "
                << labelList.size() << ")";
    throw new std::out_of_range("PreviewCanvas::child out of range");
  }
  return labelList[index];
}

void PreviewCanvas::on_child_clicked(PreviewCanvasItem* sender) {
  int index = find(sender);
  SetCurrentIndex(index);
  emit currentIndexChanged(index);
}
