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
    GrainRatio
  };

 public:
  int width, height;
  QColor foreground = QColor(0, 0, 0);
  QColor background = QColor(255, 255, 255);
  int grainSize = 1;
  bool crossedParity = true;
  double grainRatio = 0.5;

  Canvas(int width, int height);

  int Width();
  int Height();
  QColor Foreground();
  QColor Background();
  int GrainSize();
  bool CrossedParity();
  double GrainRatio();

  void SetWidth(int value);
  void SetHeight(int value);
  void SetForeground(QColor value);
  void SetBackground(QColor value);
  void SetGrainSize(int value);
  void SetCrossedParity(bool value);
  void SetGrainRatio(double value);
};
