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

static void DrawBG(QPixmap& dest, QColor color) {
  dest.fill(color);
}

static void DrawRandot(QPixmap& dest, QRect rect, int grainSize, double grainRatio,
                       const QPixmap& shape) {
  QPainter painter;
  if (!painter.begin(&dest)) {
    qFatal("failed to initialize painter in renderStereoImage");
  }

  std::bernoulli_distribution bernoulli(grainRatio);
  std::uniform_int_distribution<> intRNG(0, 3);

  auto shapes = FourAngleShapes(shape, grainSize);

  for (int y = rect.y(); y < rect.y() + rect.height(); y += grainSize) {
    for (int x = rect.x(); x < rect.x() + rect.width(); x += grainSize) {
      if (!bernoulli(kRNG)) {
        continue;
      }
      size_t dir = intRNG(kRNG);
      painter.drawPixmap(x, y, shapes[dir]);
    }
  }
  painter.end();
}

static void DrawPixmap(QPixmap& dest, QRect rect, const QPixmap& imgSrc,
                       QRect rectSrc) {
  QPainter painter;
  if (!painter.begin(&dest)) {
    qFatal("failed to initialize painter in renderStereoImage");
  }

  painter.drawPixmap(rect, imgSrc, rectSrc);
  painter.end();
}
}  // namespace

namespace imaging {

std::optional<QPixmap> kNullPixmap = std::nullopt;

QPixmap RenderShapePreview(const QPixmap& shape, const QColor& foreground) {
  auto mask = shape.createMaskFromColor(QColor(0, 0, 0), Qt::MaskMode::MaskOutColor);
  QPixmap image(mask.width(), mask.height());
  image.fill(foreground);
  image.setMask(mask);
  return image;
}

QPixmap TargetMask(const QPixmap& shape, const Target& target) {
  QPixmap mask =
      shape
          .transformed(
              QTransform().rotate(target.rotate).scale(target.scale, target.scale))
          .createMaskFromColor(QColor(0, 0, 0).rgb(), Qt::MaskMode::MaskOutColor);
  return mask;
}

QPixmap RenderStereoImage(const Canvas& canvas, const std::deque<Target>& targetList,
                          const std::deque<const QPixmap*>& shapeList,
                          const StereoImageType type,
                          const std::optional<QPixmap> grainShape) {
  QPixmap image(canvas.width * 2, canvas.height);
  QRect left{0, 0, canvas.width, canvas.height};
  QRect right = left.translated(canvas.width, 0);
  qDebug() << "left = " << left << "; right = " << right;

  auto dot = RenderShapePreview(grainShape.value_or(Square()), canvas.foreground);

  DrawBG(image, canvas.background);
  if (type == StereoImageType::Randot) {
    DrawRandot(image, left, canvas.grainSize, canvas.grainRatio, dot);
  }
  DrawPixmap(image, right, image, left);

  for (size_t i = 0; i < targetList.size(); ++i) {
    const Target& target = targetList[i];
    const QPixmap mask = TargetMask(*shapeList[i], target);

    QPixmap shape{mask.size()};
    QRect rect{0, 0, shape.width(), shape.height()};
    DrawBG(image, canvas.foreground);
    if (type == StereoImageType::Randot) {
      DrawRandot(image, rect, canvas.grainSize, canvas.grainRatio, dot);
    }
    shape.setMask(mask);

    int parity = (canvas.crossedParity) ? target.parity : -target.parity;
    auto rectL = rect.translated(target.x + parity / 2, target.y);
    auto rectR = rectL.translated(canvas.width - parity, 0);
    DrawPixmap(image, rectL, shape, rect);
    DrawPixmap(image, rectR, shape, rect);

    qDebug() << "rectL = " << rectL << "; rectR = " << rectR;
  }
  return image;
}
}  // namespace imaging
