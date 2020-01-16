// Copyright (c) 2020 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#pragma once

#ifndef COLOR_CHOOSER_H
#define COLOR_CHOOSER_H

#include <QColor>
#include <QWidget>

namespace Ui {
class ColorChooser;
}

class ColorChooser : public QWidget {
  Q_OBJECT

 public:
  explicit ColorChooser(QWidget* parent = nullptr);
  ~ColorChooser();

  QColor Color();
  void SetColor(QColor color);

 signals:
  void colorChanged(const QColor& color);

 private:
  Ui::ColorChooser* ui;
  QColor color;

 private slots:
  void on_btnChoose_clicked();
};

#endif  // COLOR_CHOOSER_H
