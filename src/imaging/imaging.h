#pragma once
#ifndef IMAGING_H
#define IMAGING_H

#include <QColor>
#include <QIcon>
#include <QImage>
#include <QPixmap>
#include <random>
#include <optional>

#include "../models/stereoimage.h"
#include "../models/target.h"

using TRng = std::mt19937;

namespace imaging {

extern std::random_device kRandomDevice;
extern TRng kRandomNumberGenerator;
extern std::bernoulli_distribution kBernoulliDistribution;

bool getRandomBool();

QPixmap renderShapePreview(const QPixmap& shape, const QColor& foreground);
QPixmap targetMask(const QPixmap& shape, const Target &target);
QPixmap randotCanvas(int width, int height, int grainSize, QColor background, QColor Foreground);

QPixmap renderPreview(const StereoImage& image);
QPixmap renderStereo(const StereoImage& image);
QPixmap renderRandot(const StereoImage& image);
}  // namespace imaging

#endif  // IMAGING_H