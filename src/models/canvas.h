// Copyright (c) 2019 Han Jin
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
    GrainRatio
  };

  using PropertyType = std::variant<int, bool, QColor, double>;

 public:
  int width, height;
  QColor foreground = QColor(0, 0, 0);
  QColor background = QColor(255, 255, 255);
  int grainSize = 1;
  bool crossedParity = true;
  double grainRatio = 0.5;

  Canvas(int width, int height);
  void setProperty(Property pname, PropertyType value);
  PropertyType getProperty(Property pname);
};
