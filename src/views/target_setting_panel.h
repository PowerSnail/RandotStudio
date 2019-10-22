#ifndef TARGET_SETTING_PANEL_H
#define TARGET_SETTING_PANEL_H

#include <QIntValidator>
#include <QWidget>
#include "../models/target.h"

namespace Ui {
class TargetSettingPanel;
}

class TargetSettingPanel : public QWidget {
  Q_OBJECT

 public:
  explicit TargetSettingPanel(QWidget* parent = nullptr);
  ~TargetSettingPanel();

  void bindTarget(Target* target);

  int getX() const;
  void setX(int value);

  int getY() const;
  void setY(int value);

  int getScale() const;
  void setScale(int value);

  int getRotate() const;
  void setRotate(int value);

  int getParity() const;
  void setParity(int value);

  int getShapeId() const;
  void setShapeId(int value);

  QColor getColor() const;
  void setColor(const QColor color);

 signals:
  void targetModified(const Target& target);

 private:
  Ui::TargetSettingPanel* ui;
  QIntValidator coordinateValidator;
  QIntValidator parityValidator;
  QIntValidator scaleValidator;
  Target* target;

 private slots:
  void on_lineEditX_editingFinished();
  void on_lineEditY_editingFinished();
  void on_lineEditScale_editingFinished();
  void on_lineEditParity_editingFinished();
  void on_comboBoxRotate_currentIndexChanged(int index);
  void on_comboBoxShapeChooser_currentIndexChanged(int index);
  void on_colorChooser_colorChanged(const QColor& color);
};

#endif  // TARGET_SETTING_PANEL_H
