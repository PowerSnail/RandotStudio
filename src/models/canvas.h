// Copyright (c) 2020 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#pragma once

#include <QImage>
#include <variant>
#include <vector>

#include "src/models/target.h"

using std::vector;

struct Canvas {
  enum class Property {
    Width,
    Height,
    Foreground,
    Background,
    GrainSize,
    CrossedParity,
    GrainRatio,
    GrainShapeID
  };

 public:
  int width = 1920;
  int height = 1080;
  QColor foreground = QColor(0, 0, 0);
  QColor background = QColor(255, 255, 255);
  int grainSize = 1;
  bool crossedParity = true;
  double grainRatio = 0.5;
  int grainShapeID = -1;

  Canvas(int width, int height);
};
