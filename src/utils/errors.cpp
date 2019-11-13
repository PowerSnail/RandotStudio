// Copyright (c) 2019 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#include "src/utils/errors.h"
#include <sstream>

std::range_error* errors::RangeError(const char* msg, int index, int lower, int higher) {
  std::stringstream ss;
  ss << msg << " Expected [" << lower << "," << higher << "), got " << index;
  return new std::range_error(ss.str());
}



