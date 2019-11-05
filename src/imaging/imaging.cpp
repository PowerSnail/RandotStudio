#include "imaging.h"

#include <QBitmap>
#include <QPainter>
#include <QPixmap>

std::random_device imaging::kRandomDevice{};

TRng imaging::kRandomNumberGenerator(imaging::kRandomDevice());

std::bernoulli_distribution imaging::kBernoulliDistribution(0.5);

bool imaging::getRandomBool() {
  return kBernoulliDistribution(kRandomNumberGenerator);
}

QPixmap imaging::renderShapePreview(const QPixmap& shape,
                                    const QColor& foreground) {
  auto mask =
      shape.createMaskFromColor(QColor(0, 0, 0), Qt::MaskMode::MaskOutColor);
  QPixmap image(mask.width(), mask.height());
  image.fill(foreground);
  image.setMask(mask);
  return image;
}

QPixmap imaging::targetMask(const QPixmap& shape, const Target& target) {
  QPixmap mask =
      shape.transformed(QTransform().rotate(target.rotate))
          .transformed(QTransform().scale(target.scale, target.scale))
          .createMaskFromColor(QColor(0, 0, 0).rgb(),
                               Qt::MaskMode::MaskOutColor);
  return mask;
}

QPixmap imaging::randotCanvas(int width, int height, int grainSize,
                              QColor background, QColor foreground) {
  QPixmap canvas(width, height);
  canvas.fill(background);

  QPainter painter;
  painter.begin(&canvas);

  for (int y = 0; y < canvas.height() ; y += grainSize) {
    for (int x = 0; x < canvas.width() ; x += grainSize) {
      if (getRandomBool()) {
        painter.fillRect(x, y, grainSize, grainSize, foreground);
      }
    }
  }

  painter.end();
  return canvas;
}

QPixmap imaging::renderPreview(const StereoImage& image) {
  QPixmap canvas(image.width, image.height);
  canvas.fill(image.background);

  QPainter painter;
  painter.begin(&canvas);
  for (auto& target : image.targetList) {
    const QPixmap& shape = image.shapeList[target.shapeID];
    QPixmap targetImg(shape.width() * target.scale,
                      shape.height() * target.scale);
    targetImg.fill(target.color);
    targetImg.setMask(imaging::targetMask(shape, target));
    painter.drawPixmap(QPoint(target.x, target.y), targetImg);
  }
  painter.end();

  return canvas;
}

QPixmap imaging::renderStereo(const StereoImage& image) {
  QPixmap canvas(image.width * 2, image.height);
  canvas.fill(image.background);
  int parityDirection = (image.crossedParity) ?   1 : -1;

  QPainter painter;
  painter.begin(&canvas);
  for (auto& target : image.targetList) {
    const QPixmap& shape = image.shapeList[target.shapeID];
    QPixmap targetImg(shape.width() * target.scale,
                      shape.height() * target.scale);
    targetImg.fill(target.color);
    targetImg.setMask(imaging::targetMask(shape, target));

    int leftParity = target.parity << 1;
    int rightParity = target.parity - leftParity;
    painter.drawPixmap(
        QPoint(target.x + leftParity * parityDirection, target.y), targetImg);
    painter.drawPixmap(
        QPoint(target.x + image.width - rightParity * parityDirection,
               target.y),
        targetImg);
  }
  painter.end();

  return canvas;
}

QPixmap imaging::renderRandot(const StereoImage& image) {
  QPixmap canvas(image.width * 2, image.height);
  QPixmap background = randotCanvas(image.width, image.height, image.grainSize,
                                    image.background, image.foreground);

  int parityDirection = (image.crossedParity) ?   1 : -1;

  QPainter painter;
  painter.begin(&canvas);
  painter.drawPixmap(0, 0, background);
  painter.drawPixmap(image.width, 0, background);

  for (auto& target : image.targetList) {
    const QPixmap& shape = image.shapeList[target.shapeID];
    QPixmap targetImg = randotCanvas(shape.width() * target.scale,
                      shape.height() * target.scale, image.grainSize, image.background, image.foreground);
    targetImg.setMask(imaging::targetMask(shape, target));

    int leftParity = target.parity << 1;
    int rightParity = target.parity - leftParity;
    painter.drawPixmap(
        QPoint(target.x + leftParity * parityDirection, target.y), targetImg);
    painter.drawPixmap(
        QPoint(target.x + image.width - rightParity * parityDirection,
               target.y),
        targetImg);
  }

  painter.end();

  return canvas;
}