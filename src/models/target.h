#pragma once

#ifndef TARGET_H
#define TARGET_H

#include <QColor>
#include <variant>

struct Target {
 public:
  enum Property { X, Y, Scale, Rotate, Parity, ShapeID, Color };
  using PropertyType = std::variant<int, QColor>;

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

  void setProperty(Property pname, PropertyType value);
  PropertyType getProperty(Property pname);
};

#endif  // TARGET_H
