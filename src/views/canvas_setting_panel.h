#ifndef CANVAS_SETTING_PANEL_H
#define CANVAS_SETTING_PANEL_H

#include <QIntValidator>
#include <QWidget>
#include "../models/stereoimage.h"

namespace Ui {
class CanvasSettingPanel;
}

class CanvasSettingPanel : public QWidget {
  Q_OBJECT

 public:
  explicit CanvasSettingPanel(QWidget* parent = nullptr);
  ~CanvasSettingPanel();

  void bindImage(StereoImage* image) { this->image = image; }

  int getWidth();
  void setWidth(int value);

  int getHeight();
  void setHeight(int value);

  int getGrainSize();
  void setGrainSize(int value);

  QColor getForeground();
  void setForeground(QColor value);

  QColor getBackground();
  void setBackground(QColor value);

  bool getCrossed();
  void setCrossed(bool value);

 signals:
  void canvasChanged();

 private:
  Ui::CanvasSettingPanel* ui;
  QIntValidator canvasSizeValidator;
  QIntValidator grainSizeValidator;
  StereoImage* image;

 private slots:
  void on_lineEditWidth_editingFinished();
  void on_lineEditHeight_editingFinished();
  void on_lineEditGrainSize_editingFinished();
  void on_colorChooserForeground_colorChanged(const QColor& color);
  void on_colorChooserBackground_colorChanged(const QColor& color);
  void on_checkBoxCrossed_stateChanged(int state);
};

#endif  // CANVAS_SETTING_PANEL_H
