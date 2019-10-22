#include "target.h"

Target::Target(int x, int y, int scale, int rotate, int parity, int shapeID,
               QColor color)
    : x(x),
      y(y),
      scale(scale),
      rotate(rotate),
      parity(parity),
      shapeID(shapeID),
      color(color) {}
