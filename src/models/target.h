// Copyright (c) 2020 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#pragma once

#include <QColor>
#include <variant>

struct Target {
 public:
  enum class Property { X, Y, Scale, Rotate, Parity, ShapeID, Color };

  // Fields
  int x;
  int y;
  int scale;
  int rotate;
  int parity;
  int shapeID;
  QColor color;

  // Constructor
  Target(int x, int y, int scale, int rotate, int parity, int shapeID, QColor color);
};
