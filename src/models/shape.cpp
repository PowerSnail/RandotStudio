#include "shape.h"

Shape::Shape(int size)
    : size(size),
      data(static_cast<size_t>(size * size), static_cast<byte>(0)) {}

int Shape::getSize() const { return size; }

const vector<byte>& Shape::getData() const { return data; }

bool Shape::isColored(int x, int y) const {
  return data[static_cast<size_t>(x * getSize() + y)];
}
