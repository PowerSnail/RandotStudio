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

  QColor getColor();
  void setColor(QColor color);

 signals:
  void colorChanged(const QColor& color);

 private:
  Ui::ColorChooser* ui;
  QColor color;
  static const QString kBtnStyleTemplate;

 private slots:
  void on_btnChoose_clicked();
};

#endif  // COLOR_CHOOSER_H
