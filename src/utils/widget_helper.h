// Copyright (c) 2019 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#pragma once

#include <QComboBox>
#include <QIcon>
#include <QLineEdit>
#include <QListWidget>
#include <QSize>
#include <QString>
#include <iostream>
#include <string>
#include <utility>

namespace widget_helper {

void operator<<(QLineEdit& edit, int val) {
  edit.setText(QString::number(val));
}

int editGetInt(QLineEdit& edit) {
  return edit.text().toInt();
}

void setImageToItem(QListWidgetItem& item, QPixmap image) {
  item.setIcon(QIcon(image.scaled(15, 15, Qt::AspectRatioMode::KeepAspectRatio)));
}

void insertIconItem(QListWidget& listWidget, QPixmap image, int index) {
  QListWidgetItem* item = new QListWidgetItem();
  setImageToItem(*item, std::move(image));
  item->setSizeHint(QSize(20, 20));
  listWidget.insertItem(index, item);
}

int listWidgetRemoveCurrentRow(QListWidget& listWidget) {
  int id = listWidget.currentRow();
  if (id != -1) {
    listWidget.removeItemWidget(listWidget.item(id));
  }
  return id;
}

int getRotateAngle(int id) {
  switch (id) {
    case 0:
      return 0;
    case 1:
      return 90;
    case 2:
      return 180;
    case 3:
      return 270;
    default:
      throw "Unexpected selected ID for rotate combobox";
  }
}

std::istream& operator>>(std::istream& in, QColor& color) {
  std::string s;
  in >> s;
  color = QColor(s.c_str());
  return in;
}
}  // namespace widget_helper
