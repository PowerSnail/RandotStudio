// Copyright (c) 2019 Han Jin
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
  Target(int x, int y, int scale, int rotate, int parity, int symbol_id, QColor color);

  int X();
  int Y();
  int Scale();
  int Rotate();
  int Parity();
  int ShapeID();
  QColor Color();

  void SetX(int value);
  void SetY(int value);
  void SetScale(int value);
  void SetRotate(int value);
  void SetParity(int value);
  void SetShapeID(int value);
  void SetColor(QColor value);
};
