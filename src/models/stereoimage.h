#ifndef STEREOIMAGE_H
#define STEREOIMAGE_H

#include <QImage>
#include <vector>
#include "shape.h"
#include "target.h"

using std::vector;

class StereoImage {
 private:
  int width, height;
  QColor foreground = QColor(255, 255, 255);
  QColor background = QColor(0, 0, 0);
  int grainSize = 1;
  bool crossedParity = true;
  vector<Target> targetList{};
  vector<Shape> shapeList{};

 public:
  StereoImage(int width, int height);

  QImage renderPreview();
  QImage renderStereo();
  QImage renderRandot();

  void setWidth(int width);
  int getWidth();
  void setHeight(int height);
  int getHeight();
  QColor getForeground() const;
  void setForeground(const QColor& value);
  QColor getBackground() const;
  void setBackground(const QColor& value);
  int getGrainSize() const;
  void setGrainSize(int value);
  bool getCrossedParity() const;
  void setCrossedParity(bool value);
  vector<Target> getTargetList() const;
  void setTargetList(const vector<Target>& value);
  vector<Shape> getShapeList() const;
  void setShapeList(const vector<Shape>& value);
};

#endif  // STEREOIMAGE_H
