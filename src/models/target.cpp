#include "target.h"

Target::Target(int x, int y, int scale, int rotate, int parity, int shapeID, QColor color)
    : x(x), y(y), scale(scale), rotate(rotate), parity(parity), shapeID(shapeID), color(color) {
}

void Target::setProperty(Property pname, PropertyType value) {
  switch (pname) {
    case X:
      this->x = std::get<decltype(x)>(value);
      break;
    case Y:
      this->y = std::get<decltype(y)>(value);
      break;
    case Scale:
      this->scale = std::get<decltype(scale)>(value);
      break;
    case Rotate:
      this->rotate = std::get<decltype(rotate)>(value);
      break;
    case Parity:
      this->parity = std::get<decltype(parity)>(value);
      break;
    case ShapeID:
      this->shapeID = std::get<decltype(shapeID)>(value);
      break;
    case Color:
      this->color = std::get<decltype(color)>(value);
      break;
    default:
      exit(-1);
  }
}
Target::PropertyType Target::getProperty(Property pname) {
  switch (pname) {
    case Property::X:
      return x;
    case Property::Y:
      return y;
    case Property::Scale:
      return scale;
    case Property::Rotate:
      return rotate;
    case Property::Parity:
      return parity;
    case Property::ShapeID:
      return shapeID;
    case Property::Color:
      return color;
    default:
      exit(-1);
  };
}