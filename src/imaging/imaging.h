// Copyright (c) 2020 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#pragma once

#include <QColor>
#include <QIcon>
#include <QImage>
#include <QPainter>
#include <QPixmap>

#include <deque>
#include <functional>
#include <memory>
#include <optional>

#include "../models/canvas.h"
#include "../models/target.h"

namespace imaging {

struct TargetImg {
  int x;
  int y;
  int width;
  int height;
};

enum class StereoImageType { Regular, Randot };

constexpr int kAngle0 = 0;
constexpr int kAngle90 = 90;
constexpr int kAngle180 = 180;
constexpr int kAngle270 = 270;

QPixmap RenderShapePreview(const QPixmap& shape, const QColor& foreground);

QPixmap TargetMask(const QPixmap& shape, const Target& target);

QPixmap RenderStereo(const Canvas& canvas, const std::deque<Target>& targetList,
                          const std::deque<QPixmap>& shapeList, StereoImageType type,
                          const std::optional<QPixmap> &grainShape);
}  // namespace imaging
