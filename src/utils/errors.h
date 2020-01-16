// Copyright (c) 2019 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#pragma once

#include <stdexcept>

namespace errors {

std::range_error* RangeError(const char* msg, int index, int lower, int higher);

}  // namespace errors
