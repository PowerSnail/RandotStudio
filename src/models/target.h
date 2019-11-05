#pragma once

#ifndef TARGET_H
#define TARGET_H

#include <QColor>
#include <iostream>

using std::istream;
using std::ostream;

class Target {
 public:
  // Fields
  int x;
  int y;
  int scale;
  int rotate;
  int parity;
  int shapeID;
  QColor color;

  // Constructor
  Target(int x, int y, int scale, int rotate, int parity, int symbol_id,
         QColor color);

};

#endif  // TARGET_H
