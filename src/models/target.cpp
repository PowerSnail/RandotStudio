// Copyright (c) 2020 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#include "src/models/target.h"

Target::Target(int x, int y, int scale, int rotate, int parity, int shapeID,
               QColor color)
    : x(x),
      y(y),
      scale(scale),
      rotate(rotate),
      parity(parity),
      shapeID(shapeID),
      color(color) {}

int Target::X() {
  return x;
}

int Target::Y() {
  return y;
}

int Target::Scale() {
  return scale;
}

int Target::Rotate() {
  return rotate;
}

int Target::Parity() {
  return parity;
}

int Target::ShapeID() {
  return shapeID;
}

QColor Target::Color() {
  return color;
}

void Target::SetX(int value) {
  x = value;
}

void Target::SetY(int value) {
  y = value;
}

void Target::SetScale(int value) {
  scale = value;
}

void Target::SetRotate(int value) {
  rotate = value;
}

void Target::SetParity(int value) {
  parity = value;
}

void Target::SetShapeID(int value) {
  shapeID = value;
}

void Target::SetColor(QColor value) {
  color = value;
}
