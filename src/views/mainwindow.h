#pragma once

#include <QMainWindow>
#include <QLabel>
#include <map>
#include "../viewmodels/mainwindowvm.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(MainWindowViewModel* vm, QWidget* parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow* ui;
  MainWindowViewModel* vm;

  constexpr static int kPreviewShadowSize = 20;

  void loadCanvasProperties();
  void loadTargetProperties();
  
  Target& getTarget(int id);
  QPixmap getTargetPreview(const Target&);

  void renderPreview();

 private slots:
  // Slots for target list
  void on_listWidgetTarget_currentRowChanged(int currentRow);
  void on_btnAddTarget_clicked();
  void on_btnRemoveTarget_clicked();

  // Slots for shape list
  void on_listWidgetShape_currentRowChanged(int currentRow);
  void on_btnAddShape_clicked();
  void on_btnRemoveShape_clicked();

  // Slots for canvas properties
  void on_lineEditWidth_editingFinished();
  void on_lineEditHeight_editingFinished();
  void on_lineEditGrainSize_editingFinished();
  void on_colorChooserForeground_colorChanged(const QColor& color);
  void on_colorChooserBackground_colorChanged(const QColor& color);
  void on_checkBoxCrossed_stateChanged(int state);

  // Slots for target properties
  void on_lineEditX_editingFinished();
  void on_lineEditY_editingFinished();
  void on_lineEditScale_editingFinished();
  void on_lineEditParity_editingFinished();
  void on_comboBoxRotate_currentIndexChanged(int index);
  void on_colorChooserTarget_colorChanged(const QColor& color);

  void on_previewCanvas_currentIndexChanged(int index);

  // VM signals
  void on_vm_currentTargetIDChanged(int oldID, int newID);
  void on_vm_currentShapeIDChanged(int oldID, int newID);

  void on_vm_targetCreated(int targetID);
  void on_vm_targetUpdated(int targetID, Target::Property pname);
  void on_vm_targetRemoved(int targetID);

  void on_vm_shapeLoaded(int shapeID);
  void on_vm_shapeRemoved(int shapeID);

  void on_vm_canvasUpdated(Canvas::Property pname);

  // Actions
  void on_actionAbout_triggered();
  void on_actionSave_triggered();
  void on_actionLoad_triggered();
  void on_actionRandot_triggered();
  void on_actionStereo_PNG_triggered();
};
