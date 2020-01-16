// Copyright (c) 2020 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#pragma once

#include <QObject>
#include <QPixmap>
#include <array>
#include <deque>
#include <exception>
#include <map>

#include "../imaging/imaging.h"
#include "../models/canvas.h"
#include "../models/target.h"

class MainWindowViewModel : public QObject {
  Q_OBJECT

 public:  // Static Consts
  constexpr static int kDefaultShapeCount = 2;
  constexpr static std::array<const char*, kDefaultShapeCount> kDefaultShapePathList{
      ":randot_studio/default_shapes/e.png", ":randot_studio/default_shapes/c.png"};

 private:
  // VM Fields
  int currentTargetID = -1;
  int currentShapeID = -1;
  QString prevShapeDir = ".";
  QString prevConfigDir = ".";
  QString prevExportDir = ".";

  // Data Fields
  Canvas canvas;
  std::deque<Target> targetList;
  std::deque<QString> shapePathList;
  std::map<QString, QPixmap> loadedShape;

 private:
  bool TargetIDValid(int id);
  bool ShapeIDValid(int id);

 public:
  MainWindowViewModel();

  explicit MainWindowViewModel(QString configuration);

  int CurrentTargetID();
  void SetCurrentTargetID(int value);

  int CurrentShapeID();
  void SetCurrentShapeID(int value);

  QString PrevShapeDir();
  void SetPrevShapeDir(QString&& value);

  QString PrevConfigDir();
  void SetPrevConfigDir(QString&& value);

  QString PrevExportDir();
  void SetPrevExportDir(QString&& value);

  const Target& GetTarget(int id);
  void CreateTarget(Target newTarget);
  void SetTargetX(int id, int value);
  void SetTargetY(int id, int value);
  void SetTargetScale(int id, int value);
  void SetTargetRotate(int id, int value);
  void SetTargetParity(int id, int value);
  void SetTargetShapeID(int id, int value);
  void SetTargetColor(int id, QColor value);
  Target RemoveTarget(int id);

  const QPixmap& getShape(int id);
  void LoadShape(QString filepath);
  void RemoveShape(int id);

  const Canvas& getCanvas();
  void SetCanvasWidth(int value);
  void SetCanvasHeight(int value);
  void SetCanvasForeground(QColor value);
  void SetCanvasBackground(QColor value);
  void SetCanvasGrainSize(int value);
  void SetCanvasCrossedParity(bool value);
  void SetCanvasGrainRatio(double value);

  void SaveToFile(QString filename);
  void LoadFromFile(QString filename);

  void ExportImage(QString filename, imaging::StereoImageType type);

 signals:
  // UI Controls
  void currentTargetIDChanged(int oldID, int newID);
  void currentShapeIDChanged(int oldID, int newID);

  // Target
  void targetCreated(int targetID);
  void targetUpdated(int targetID, Target::Property pname);
  void targetRemoved(int targetID);

  // Shape
  void shapeLoaded(int shapeID);
  void shapeRemoved(int shapeID);

  // Canvas
  void canvasUpdated(Canvas::Property pname);
};  // class MainWindowViewModel
