// Copyright (c) 2020 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#pragma once

#include <QDialog>

namespace Ui {
class DialogAbout;
}

class DialogAbout : public QDialog {
  Q_OBJECT

 public:
  explicit DialogAbout(QWidget *parent = nullptr);
  ~DialogAbout();

 private:
  Ui::DialogAbout *ui;
};
