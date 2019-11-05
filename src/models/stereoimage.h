#pragma once

#ifndef STEREOIMAGE_H
#define STEREOIMAGE_H

#include <QImage>
#include <vector>
#include "target.h"

using std::vector;

class StereoImage {
 public:
  int width, height;
  QColor foreground = QColor(0, 0, 0);
  QColor background = QColor(255, 255, 255);
  int grainSize = 1;
  bool crossedParity = true;
  vector<Target> targetList{};
  vector<QString> shapeList{};

  StereoImage(int width, int height);
  void loadFromFile(QString);
  void saveToFile(QString);

  static const vector<QString> kDefaultShapes;
};

#endif  // STEREOIMAGE_H
