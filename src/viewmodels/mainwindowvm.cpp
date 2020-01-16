// Copyright (c) 2019 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#include "src/viewmodels/mainwindowvm.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QtDebug>
#include <algorithm>
#include <sstream>
#include <string>

#include "src/utils/errors.h"

bool MainWindowViewModel::TargetIDValid(int id) {
  return (id >= 0 && static_cast<size_t>(id) < targetList.size());
}

bool MainWindowViewModel::ShapeIDValid(int id) {
  return (id >= 0 &&
          static_cast<size_t>(id) < shapePathList.size() + kDefaultShapeCount);
}

MainWindowViewModel::MainWindowViewModel()
    : canvas(1920, 1080), targetList(), shapePathList() {
  setObjectName("vm");
}

MainWindowViewModel::MainWindowViewModel(QString configurationPath)
    : MainWindowViewModel() {
  LoadFromFile(configurationPath);
}

int MainWindowViewModel::CurrentTargetID() {
  return currentTargetID;
}

void MainWindowViewModel::SetCurrentTargetID(int value) {
  if ((TargetIDValid(value) || value == -1) && currentTargetID != value) {
    int oldValue = currentTargetID;
    currentTargetID = value;
    emit currentTargetIDChanged(oldValue, value);
  }
}

int MainWindowViewModel::CurrentShapeID() {
  return currentShapeID;
}
void MainWindowViewModel::SetCurrentShapeID(int value) {
  if (currentShapeID != value) {
    int oldValue = currentShapeID;
    currentShapeID = value;
    emit currentShapeIDChanged(oldValue, value);

    if (currentTargetID != -1) {
      SetTargetShapeID(currentTargetID, value);
    }
  }
}

void MainWindowViewModel::SetPrevShapeDir(QString&& value) {
  if (value != prevShapeDir) {
    prevShapeDir = value;
  }
}

QString MainWindowViewModel::PrevShapeDir() {
  return prevShapeDir;
}

void MainWindowViewModel::SetPrevConfigDir(QString&& value) {
  if (value != prevConfigDir) {
    prevConfigDir = value;
  }
}

QString MainWindowViewModel::PrevConfigDir() {
  return prevConfigDir;
}

void MainWindowViewModel::SetPrevExportDir(QString&& value) {
  if (value != prevExportDir) {
    prevExportDir = value;
  }
}

QString MainWindowViewModel::PrevExportDir() {
  return prevExportDir;
}

const Target& MainWindowViewModel::GetTarget(int id) {
  if (TargetIDValid(id)) {
    return targetList[static_cast<size_t>(id)];
  } else {
    throw errors::RangeError("Target out of range.", id, 0, targetList.size());
  }
}

void MainWindowViewModel::CreateTarget(Target newTarget) {
  targetList.push_back(newTarget);
  int id = static_cast<int>(targetList.size() - 1);
  emit targetCreated(id);
}

void MainWindowViewModel::SetTargetX(int id, int value) {
  if (!TargetIDValid(id)) {
    throw errors::RangeError("Target out of range.", id, 0, targetList.size());
  }
  if (targetList[id].x != value) {
    targetList[id].x = value;
    emit targetUpdated(id, Target::Property::X);
  }
}

void MainWindowViewModel::SetTargetY(int id, int value) {
  if (!TargetIDValid(id)) {
    throw errors::RangeError("Target out of range.", id, 0, targetList.size());
  }
  if (targetList[id].y != value) {
    targetList[id].y = value;
    emit targetUpdated(id, Target::Property::Y);
  }
}

void MainWindowViewModel::SetTargetScale(int id, int value) {
  if (!TargetIDValid(id)) {
    throw errors::RangeError("Target out of range.", id, 0, targetList.size());
  }
  if (targetList[id].scale != value) {
    targetList[id].scale = value;
    emit targetUpdated(id, Target::Property::Scale);
  }
}

void MainWindowViewModel::SetTargetRotate(int id, int value) {
  if (!TargetIDValid(id)) {
    throw errors::RangeError("Target out of range.", id, 0, targetList.size());
  }
  if (targetList[id].rotate != value) {
    targetList[id].rotate = value;
    emit targetUpdated(id, Target::Property::Rotate);
  }
}

void MainWindowViewModel::SetTargetParity(int id, int value) {
  if (!TargetIDValid(id)) {
    throw errors::RangeError("Target out of range.", id, 0, targetList.size());
  }
  if (targetList[id].parity != value) {
    targetList[id].parity = value;
    emit targetUpdated(id, Target::Property::Parity);
  }
}

void MainWindowViewModel::SetTargetShapeID(int id, int value) {
  if (!TargetIDValid(id)) {
    throw errors::RangeError("Target out of range.", id, 0, targetList.size());
  }
  if (targetList[id].shapeID != value) {
    targetList[id].shapeID = value;
    emit targetUpdated(id, Target::Property::ShapeID);
  }
}

void MainWindowViewModel::SetTargetColor(int id, QColor value) {
  if (!TargetIDValid(id)) {
    throw errors::RangeError("Target out of range.", id, 0, targetList.size());
  }
  if (targetList[id].color != value) {
    targetList[id].color = value;
    emit targetUpdated(id, Target::Property::Color);
  }
}

Target MainWindowViewModel::RemoveTarget(int id) {
  if (TargetIDValid(id)) {
    Target removedTarget = targetList[static_cast<size_t>(id)];
    targetList.erase(targetList.begin() + id);
    emit targetRemoved(id);
    return removedTarget;
  } else {
    throw errors::RangeError("Target out of range.", id, 0, targetList.size());
  }
}

const QPixmap& MainWindowViewModel::getShape(int id) {
  if (ShapeIDValid(id)) {
    auto path = (id < kDefaultShapeCount)
                    ? kDefaultShapePathList[static_cast<size_t>(id)]
                    : shapePathList[static_cast<size_t>(id - kDefaultShapeCount)];

    if (loadedShape.find(path) == loadedShape.end()) {
      QPixmap shape =
          QPixmap::fromImage(QImage(path).convertToFormat(QImage::Format_Mono));
      loadedShape[path] = shape;
    }
    return loadedShape[path];
  } else {
    throw errors::RangeError("Shape ID out of range.", id, 0, targetList.size());
  }
}

void MainWindowViewModel::LoadShape(QString filepath) {
  shapePathList.push_back(filepath);
  emit shapeLoaded(static_cast<int>(shapePathList.size()) + kDefaultShapeCount - 1);
}

void MainWindowViewModel::RemoveShape(int id) {
  if (ShapeIDValid(id)) {
    if (id < kDefaultShapeCount) {
      qCritical() << "Cannot delete this shape." << id;
    }
    id -= kDefaultShapeCount;
    loadedShape.erase(shapePathList[static_cast<size_t>(id)]);
    shapePathList.erase(shapePathList.begin() + id);

    for (size_t i = 0; i < targetList.size(); ++i) {
      if (targetList[i].shapeID == id) {
        SetTargetShapeID(i, 0);
      } else if (targetList[i].shapeID > id) {
        SetTargetShapeID(i, id - 1);
      } else {
        // No need to change
      }
    }

    emit shapeRemoved(id);
  } else {
    throw errors::RangeError("Shape ID out of range.", id, 0, targetList.size());
  }
}

const Canvas& MainWindowViewModel::getCanvas() {
  return canvas;
}

void MainWindowViewModel::SetCanvasWidth(int value) {
  if (canvas.Width() != value) {
    canvas.SetWidth(value);
    emit canvasUpdated(Canvas::Property::Width);
  }
}

void MainWindowViewModel::SetCanvasHeight(int value) {
  if (canvas.Height() != value) {
    canvas.SetHeight(value);
    emit canvasUpdated(Canvas::Property::Height);
  }
}

void MainWindowViewModel::SetCanvasForeground(QColor value) {
  if (canvas.Foreground() != value) {
    canvas.SetForeground(value);
    emit canvasUpdated(Canvas::Property::Foreground);
  }
}

void MainWindowViewModel::SetCanvasBackground(QColor value) {
  if (canvas.Background() != value) {
    canvas.SetBackground(value);
    emit canvasUpdated(Canvas::Property::Background);
  }
}

void MainWindowViewModel::SetCanvasGrainSize(int value) {
  if (canvas.GrainSize() != value) {
    canvas.SetGrainSize(value);
    emit canvasUpdated(Canvas::Property::GrainSize);
  }
}

void MainWindowViewModel::SetCanvasCrossedParity(bool value) {
  if (canvas.CrossedParity() != value) {
    canvas.SetCrossedParity(value);
    emit canvasUpdated(Canvas::Property::CrossedParity);
  }
}

void MainWindowViewModel::SetCanvasGrainRatio(double value) {
  if (canvas.GrainRatio() != value) {
    canvas.SetGrainRatio(value);
    emit canvasUpdated(Canvas::Property::GrainRatio);
  }
}

void MainWindowViewModel::SaveToFile(QString filename) {
  QFile file(filename);
  file.open(QFile::WriteOnly);

  QJsonArray jsonTargetList;
  for (auto& t : targetList) {
    QJsonObject jsonTarget;
    jsonTarget.insert("x", QJsonValue(t.x));
    jsonTarget.insert("y", QJsonValue(t.y));
    jsonTarget.insert("scale", QJsonValue(t.scale));
    jsonTarget.insert("rotate", QJsonValue(t.rotate));
    jsonTarget.insert("parity", QJsonValue(t.parity));
    jsonTarget.insert("shapeID", QJsonValue(t.shapeID));
    jsonTarget.insert("color", QJsonValue(t.color.name()));
    jsonTargetList.append(jsonTarget);
  }

  QJsonArray jsonShapeList;
  for (auto& shapePath : shapePathList) {
    jsonShapeList.append(QJsonValue(shapePath));
  }

  QJsonObject jsonMain;
  jsonMain.insert("width", QJsonValue(canvas.width));
  jsonMain.insert("height", QJsonValue(canvas.height));
  jsonMain.insert("foreground", QJsonValue(canvas.foreground.name()));
  jsonMain.insert("background", QJsonValue(canvas.background.name()));
  jsonMain.insert("grainSize", QJsonValue(canvas.grainSize));
  jsonMain.insert("grainRatio", QJsonValue(canvas.grainRatio));
  jsonMain.insert("crossedParity", QJsonValue(canvas.crossedParity));
  jsonMain.insert("targetList", QJsonValue(jsonTargetList));
  jsonMain.insert("shapeList", QJsonValue(jsonShapeList));

  file.write(QJsonDocument(jsonMain).toJson());
}

void MainWindowViewModel::LoadFromFile(QString filename) {
  QFile file(filename);
  file.open(QFile::ReadOnly);

  QJsonDocument d = QJsonDocument::fromJson(file.readAll());

  SetCanvasWidth(d["width"].toInt());
  SetCanvasHeight(d["height"].toInt());
  SetCanvasForeground(QColor(d["foreground"].toString()));
  SetCanvasBackground(QColor(d["background"].toString()));
  SetCanvasGrainSize(d["grainSize"].toInt());
  SetCanvasGrainRatio(d["grainRatio"].toDouble());
  SetCanvasCrossedParity(d["crossedParity"].toBool());

  for (size_t i = 0; i < targetList.size(); ++i) {
    RemoveTarget(0);
  }

  for (size_t i = 0; i < shapePathList.size(); ++i) {
    RemoveShape(0);
  }

  for (auto s : d["shapeList"].toArray()) {
    LoadShape(s.toString());
  }

  for (auto t : d["targetList"].toArray()) {
    QJsonObject targetObj = t.toObject();
    CreateTarget(Target(targetObj["x"].toInt(), targetObj["y"].toInt(),
                        targetObj["scale"].toInt(), targetObj["rotate"].toInt(),
                        targetObj["parity"].toInt(), targetObj["shapeID"].toInt(),
                        QColor(targetObj["color"].toString())));
  }
}

void MainWindowViewModel::ExportImage(QString filename, imaging::StereoImageType type) {
  std::deque<const QPixmap*> shapeList;
  for (auto& target : targetList) {
    shapeList.push_back(&getShape(target.shapeID));
  }
  QPixmap img = imaging::RenderStereoImage(canvas, targetList, shapeList, type);
  if (!img.save(filename, "PNG", 100)) {
    qDebug() << "Failed to save file" << filename;
    return;
  }
}
