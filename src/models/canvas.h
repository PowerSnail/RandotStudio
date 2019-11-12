// Copyright (c) 2019 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#pragma once

#include <QImage>
#include <variant>
#include <vector>

#include "target.h"

using std::vector;

struct Canvas {
  enum class Property { Width, Height, Foreground, Background, GrainSize, CrossedParity };

  using PropertyType = std::variant<int, bool, QColor>;

 public:
  int width, height;
  QColor foreground = QColor(0, 0, 0);
  QColor background = QColor(255, 255, 255);
  int grainSize = 1;
  bool crossedParity = true;

  Canvas(int width, int height);
  void setProperty(Property pname, PropertyType value);
  PropertyType getProperty(Property pname);
};
