#ifndef SHAPE_H
#define SHAPE_H

#include <vector>

using std::vector;
using byte = unsigned char;

class Shape {
 public:
  Shape(int size);
  int getSize() const;
  const vector<byte>& getData() const;
  bool isColored(int x, int y) const;

 private:
  int size;
  vector<byte> data;
};

#endif  // SHAPE_H
