// Copyright (c) 2020 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#include "imaging.h"

#include <QBitmap>
#include <QtDebug>
#include <random>

namespace {

static std::random_device kRandomDevice{};

static std::mt19937 kRNG(kRandomDevice());

static QPixmap Square() {
  QPixmap img{1, 1};
  img.fill(QColor::fromRgb(0, 0, 0));
  return img;
}

static std::deque<QPixmap> FourAngleShapes(const QPixmap& shape, int size) {
  auto resizedShape =
      shape.scaled(size, size, Qt::KeepAspectRatio, Qt::FastTransformation);
  return {
      resizedShape.transformed(QTransform().rotate(0)),
      resizedShape.transformed(QTransform().rotate(90)),
      resizedShape.transformed(QTransform().rotate(180)),
      resizedShape.transformed(QTransform().rotate(270)),
  };
}

QPixmap RandotPixmap(QSize size, QColor background, int grainSize, double grainRatio,
                     const QPixmap& shape) {
  QPixmap pixmap{size};
  pixmap.fill(background);

  QPainter painter;
  if (!painter.begin(&pixmap)) {
    qFatal("failed to initialize painter in renderStereoImage");
  }

  auto shapes = FourAngleShapes(shape, grainSize);
  std::bernoulli_distribution bernoulli(grainRatio);
  std::uniform_int_distribution<> intRNG(0, 3);
  for (int y = 0; y < pixmap.height(); y += grainSize) {
    for (int x = 0; x < pixmap.width(); x += grainSize) {
      if (!bernoulli(kRNG)) {
        continue;
      }
      size_t dir = intRNG(kRNG);
      painter.drawPixmap(x, y, shapes[dir]);
    }
  }
  painter.end();
  return pixmap;
}

QPixmap ColoredPixmap(QSize size, QColor bg) {
  QPixmap pixmap{size};
  pixmap.fill(bg);
  return pixmap;
}
}  // namespace

namespace imaging {

QPixmap RenderShapePreview(const QPixmap& shape, const QColor& foreground) {
  auto mask = shape.createMaskFromColor(QColor(0, 0, 0), Qt::MaskMode::MaskOutColor);
  QPixmap image(mask.width(), mask.height());
  image.fill(foreground);
  image.setMask(mask);
  return image;
}

QPixmap TargetMask(const QPixmap& shape, const Target& target) {
  return shape
      .transformed(QTransform().rotate(target.rotate).scale(target.scale, target.scale))
      .createMaskFromColor(QColor(0, 0, 0).rgb(), Qt::MaskMode::MaskOutColor);
}

QPixmap RenderStereo(const Canvas& canvas, const std::deque<Target>& targetList,
                     const std::deque<QPixmap>& shapeList, StereoImageType type,
                     const std::optional<QPixmap> grainShape) {
  auto dot = grainShape.value_or(Square());
  auto bg = (type == StereoImageType::Regular) 
    ? ColoredPixmap({canvas.width, canvas.height}, canvas.background)
    : RandotPixmap({canvas.width, canvas.height}, canvas.background,
                           canvas.grainSize, canvas.grainRatio, dot);

  QPixmap image { canvas.width * 2, canvas.height };
  QPainter p;
  p.begin(&image);
  p.drawPixmap(0, 0, bg);
  p.drawPixmap(canvas.width, 0, bg);

  for (auto& target : targetList) {
    auto mask = TargetMask(shapeList[target.shapeID], target);
    auto shape = (type == StereoImageType::Regular)
                     ? ColoredPixmap(mask.size(), target.color)
                     : RandotPixmap(mask.size(), canvas.background, canvas.grainSize,
                                    canvas.grainRatio, dot);
    shape.setMask(mask);
    int parity = (canvas.crossedParity) ? target.parity : -target.parity;
    p.drawPixmap(target.x + parity / 2, target.y, shape);
    p.drawPixmap(target.x - (parity - parity / 2) + canvas.width, target.y, shape);
  }
  p.end();
  return image;
}
}  // namespace imaging
