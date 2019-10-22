#include "stereoimage.h"
#include <QPainter>

int StereoImage::getWidth() { return width; }

void StereoImage::setWidth(int width) { this->width = width; }

void StereoImage::setHeight(int height) { this->height = height; }

int StereoImage::getHeight() { return height; }

QColor StereoImage::getForeground() const { return foreground; }

void StereoImage::setForeground(const QColor& value) { foreground = value; }

QColor StereoImage::getBackground() const { return background; }

void StereoImage::setBackground(const QColor& value) { background = value; }

int StereoImage::getGrainSize() const { return grainSize; }

void StereoImage::setGrainSize(int value) { grainSize = value; }

bool StereoImage::getCrossedParity() const { return crossedParity; }

void StereoImage::setCrossedParity(bool value) { crossedParity = value; }

vector<Target> StereoImage::getTargetList() const { return targetList; }

void StereoImage::setTargetList(const vector<Target>& value) {
  targetList = value;
}

vector<Shape> StereoImage::getShapeList() const { return shapeList; }

void StereoImage::setShapeList(const vector<Shape>& value) {
  shapeList = value;
}

StereoImage::StereoImage(int width, int height)
    : width(width), height(height) {}

QImage StereoImage::renderPreview() {
  QImage canvas(this->width, this->height, QImage::Format_RGB32);
  canvas.fill(this->background);

  QPainter painter;
  painter.begin(&canvas);
  for (auto& target : targetList) {
    painter.setPen(target.color);
    Shape& shape = shapeList[static_cast<size_t>(target.shapeID)];
    for (int x = 0; x < shape.getSize(); x++) {
      for (int y = 0; y < shape.getSize(); y++) {
        if (shape.isColored(x, y)) {
          painter.drawRect(x * target.scale + target.x,
                           y * target.scale + target.y, target.scale,
                           target.scale);
        }
      }
    }
  }
  painter.end();

  return canvas;
}

QImage StereoImage::renderStereo() {
  QImage canvas(this->width * 2, this->height, QImage::Format_RGB32);
  // TODO: Implement renderStereo
  canvas.fill(this->background);

  return canvas;
}

QImage StereoImage::renderRandot() {
  QImage canvas(this->width * 2, this->height, QImage::Format_RGB32);
  // TODO: Implement renderRandot
  return canvas;
}
