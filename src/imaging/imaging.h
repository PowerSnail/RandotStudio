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

extern std::optional<QPixmap> kNullPixmap;

enum class StereoImageType { Regular, Randot };

QPixmap RenderShapePreview(const QPixmap& shape, const QColor& foreground);

QPixmap TargetMask(const QPixmap& shape, const Target& target);

QPixmap RenderStereoImage(const Canvas& canvas, const std::deque<Target>& targetList,
                          const std::deque<const QPixmap*>& shapeList,
                          const StereoImageType type,
                          const std::optional<QPixmap> grainShape = kNullPixmap);

}  // namespace imaging
