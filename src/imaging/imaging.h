#pragma once
#ifndef IMAGING_H
#define IMAGING_H

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

namespace imaging {

extern std::random_device kRandomDevice;
extern TRng kRandomNumberGenerator;
extern std::bernoulli_distribution kBernoulliDistribution;

bool getRandomBool();

enum class StereoImageType { Regular, Randot };

QPixmap renderShapePreview(const QPixmap& shape, const QColor& foreground);
QPixmap targetMask(const QPixmap& shape, const Target& target);
QPixmap fillRandot(QSize size, int grainSize, QColor background, QColor foreground);

QPixmap renderStereoImage(const Canvas& canvas,
                          const std::deque<Target>& targetList,
                          const std::deque<QPixmap>& targetImgList,
                          StereoImageType type);

QPixmap renderTarget(const Canvas& canvas, const Target& target, const QPixmap& shape, StereoImageType type);

}  // namespace imaging

#endif  // IMAGING_H