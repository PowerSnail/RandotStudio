// Copyright (c) 2019 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#include "src/models/canvas.h"
#include <QDebug>

Canvas::Canvas(int width, int height) : width(width), height(height) {}

int Canvas::Width() {
  return width;
}

int Canvas::Height() {
  return height;
}

QColor Canvas::Foreground() {
  return foreground;
}

QColor Canvas::Background() {
  return background;
}

int Canvas::GrainSize() {
  return grainSize;
}

bool Canvas::CrossedParity() {
  return crossedParity;
}

double Canvas::GrainRatio() {
  return grainRatio;
}

void Canvas::SetWidth(int value) {
  width = value;
}

void Canvas::SetHeight(int value) {
  height = value;
}

void Canvas::SetForeground(QColor value) {
  foreground = value;
}

void Canvas::SetBackground(QColor value) {
  background = value;
}

void Canvas::SetGrainSize(int value) {
  grainSize = value;
}

void Canvas::SetCrossedParity(bool value) {
  crossedParity = value;
}

void Canvas::SetGrainRatio(double value) {
  grainRatio = value;
}
