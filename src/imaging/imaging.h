// Copyright (c) 2019 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#pragma once

#include <QColor>
#include <QIcon>
#include <QImage>
#include <QPixmap>
#include <deque>
#include <functional>
#include <optional>
#include <random>

#include "../models/canvas.h"
#include "../models/target.h"

using TRng = std::mt19937;
using TBernoulli = std::bernoulli_distribution;

namespace imaging {

extern std::random_device kRandomDevice;
extern TRng kRNG;

enum class StereoImageType { Regular, Randot };

QPixmap renderShapePreview(const QPixmap& shape, const QColor& foreground);
QPixmap targetMask(const QPixmap& shape, const Target& target);
QPixmap fillRandot(QSize size, int grainSize, double grainRatio, QColor background,
                   QColor foreground);

QPixmap renderStereoImage(const Canvas& canvas, const std::deque<Target>& targetList,
                          const std::deque<QPixmap>& targetImgList, StereoImageType type);

QPixmap renderTarget(const Canvas& canvas, const Target& target, const QPixmap& shape,
                     StereoImageType type);

}  // namespace imaging
