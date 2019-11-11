#include "imaging.h"

#include <QBitmap>
#include <QPainter>
#include <QPixmap>

#include "../utils/logging.h"

using namespace logging;

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
  QTransform transform;
  QPixmap mask =
      shape.transformed(QTransform().rotate(target.rotate))
          .transformed(QTransform().scale(target.scale, target.scale))
          .createMaskFromColor(QColor(0, 0, 0).rgb(),
                               Qt::MaskMode::MaskOutColor);
  return mask;
}

QPixmap imaging::fillRandot(QSize size, int grainSize, QColor background,
                            QColor foreground) {
  QPixmap image(size);
  image.fill(background);
  QPainter painter;
  if (!painter.begin(&image)) {
    logError("Failed to initialize painter in fillRandot");
    throw new std::runtime_error("QPainter Initialization Error. Failed to initialize painter in fillRandot");
  }
  for (int y = 0; y < size.height(); y += grainSize) {
    for (int x = 0; x < size.width(); x += grainSize) {
      if (getRandomBool()) {
        painter.fillRect(x, y, grainSize, grainSize, foreground);
      }
    }
  }
  painter.end();
  return image;
}

QPixmap imaging::renderStereoImage(const Canvas& canvas,
                                   const std::deque<Target>& targetList,
                                   const std::deque<QPixmap>& targetImgList,
                                   StereoImageType type) {
  QPixmap image(canvas.width * 2, canvas.height);
  QPainter painter;
  if (!painter.begin(&image)) {
    logError("failed to initialize painter in renderStereoImage");
    exit(-1);
  }

  if (type == StereoImageType::Randot) {
    logDebug("Painting Randot");
    QPixmap bg =
        fillRandot(QSize(canvas.width, canvas.height), canvas.grainSize,
                   canvas.background, canvas.foreground);
    painter.drawPixmap(QRect(0, 0, canvas.width, canvas.height), bg);
    painter.drawPixmap(QRect(canvas.width, 0, canvas.width, canvas.height), bg);
  } else {
    logDebug("Painting Regular");
    painter.fillRect(QRect(0, 0, image.width(), image.height()),
                     canvas.background);
  }

  int parityDirection = (canvas.crossedParity) ? 1 : -1;

  logDebug("target list: ", targetList.size());
  for (size_t i = 0; i < targetList.size(); ++i) {
    auto target = targetList[i];
    auto targetImg = targetImgList[i];

    int leftParity = target.parity << 1;
    int rightParity = target.parity - leftParity;
    painter.drawPixmap(
        QPoint(target.x + leftParity * parityDirection, target.y), targetImg);
    painter.drawPixmap(
        QPoint(target.x + canvas.width - rightParity * parityDirection,
               target.y),
        targetImg);
  }
  painter.end();

  return image;
}

QPixmap imaging::renderTarget(const Canvas& canvas, const Target& target,
                              const QPixmap& shape, StereoImageType type) {
  auto mask = imaging::targetMask(shape, target);
  QPixmap targetImg;
  if (type == StereoImageType::Regular) {
    targetImg = QPixmap(mask.size());
    targetImg.fill(target.color);
  } else {
    targetImg = fillRandot(mask.size(), canvas.grainSize, canvas.background,
                           canvas.foreground);
  }
  assert(mask.size() == targetImg.size());
  targetImg.setMask(mask);
  return targetImg;
}
