#pragma once

#ifndef LISTITEMTARGET_H
#define LISTITEMTARGET_H

#include <QWidget>
#include "../models/target.h"

namespace Ui {
class ListItemTarget;
}

class ListItemTarget : public QWidget {
  Q_OBJECT

 public:
  explicit ListItemTarget(QWidget* parent = nullptr);
  ~ListItemTarget();

 private:
  Ui::ListItemTarget* ui;
};

#endif  // LISTITEMTARGET_H
