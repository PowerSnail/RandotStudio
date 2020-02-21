// Copyright (c) 2020 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#pragma once

#ifndef COLOR_CHOOSER_H
#define COLOR_CHOOSER_H

#include <QColor>
#include <QPushButton>

class ColorChooser : public QPushButton {
  Q_OBJECT

 public:
  explicit ColorChooser(QWidget* parent = nullptr);

  QColor Color();
  void SetColor(const QColor & color);

 signals:
  void colorChanged(const QColor& color);

 private:
  QColor color;
};

#endif  // COLOR_CHOOSER_H
