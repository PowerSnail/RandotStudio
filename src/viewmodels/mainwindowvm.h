// Copyright (c) 2019 Han Jin
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
  bool targetIDValid(int id);
  bool shapeIDValid(int id);

 public:
  MainWindowViewModel();

  explicit MainWindowViewModel(QString configuration);

  int getCurrentTargetID();
  void setCurrentTargetID(int value);

  int getCurrentShapeID();
  void setCurrentShapeID(int value);

  void setPrevShapeDir(QString&& value);
  QString getPrevShapeDir();

  void setPrevConfigDir(QString&& value);
  QString getPrevConfigDir();

  void setPrevExportDir(QString&& value);
  QString getPrevExportDir();

  const Target& getTarget(int id);
  void createTarget(Target newTarget);
  void updateTarget(int id, Target::Property pname, Target::PropertyType value);
  Target removeTarget(int id);

  const QPixmap& getShape(int id);
  void loadShape(QString filepath);
  void removeShape(int id);

  const Canvas& getCanvas();
  void updateCanvas(Canvas::Property pname, Canvas::PropertyType value);

  void saveToFile(QString filename);
  void loadFromFile(QString filename);

  void exportImage(QString filename, imaging::StereoImageType type);

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
