#include "canvas.h"

Canvas::Canvas(int width, int height) : width(width), height(height) {
}

void Canvas::setProperty(Property pname, PropertyType value) {
  switch (pname) {
    case Property::Width:
      this->width = std::get<int>(value);
      break;
    case Property::Height:
      this->height = std::get<int>(value);
      break;
    case Property::Foreground:
      this->foreground = std::get<QColor>(value);
      break;
    case Property::Background:
      this->background = std::get<QColor>(value);
      break;
    case Property::GrainSize:
      this->grainSize = std::get<int>(value);
      break;
    case Property::CrossedParity:
      this->grainSize = std::get<bool>(value);
      break;
    default:
      exit(-1);
  }
}

Canvas::PropertyType Canvas::getProperty(Property pname) {
  switch (pname) {
    case Property::Width:
      return this->width;
    case Property::Height:
      return this->height;
    case Property::Foreground:
      return this->foreground;
    case Property::Background:
      return this->background;
    case Property::GrainSize:
      return this->grainSize;
    case Property::CrossedParity:
      return this->grainSize;
    default:
      exit(-1);
  }
}
