#include "mainwindowvm.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <sstream>
#include <string>

#include "../utils/logging.h"

using namespace logging;

bool MainWindowViewModel::targetIDValid(int id) {
  return (id >= 0 && id < targetList.size());
}

bool MainWindowViewModel::shapeIDValid(int id) {
  return (id >= 0 && id < shapePathList.size() + kDefaultShapeCount);
}

MainWindowViewModel::MainWindowViewModel()
    : canvas(1920, 1080), targetList(), shapePathList() {
  setObjectName("vm");
}

MainWindowViewModel::MainWindowViewModel(QString configurationPath)
    : MainWindowViewModel() {
  loadFromFile(configurationPath);
}

int MainWindowViewModel::getCurrentTargetID() { return currentTargetID; }

void MainWindowViewModel::setCurrentTargetID(int value) {
  if ((targetIDValid(value) || value == -1) && currentTargetID != value) {
    int oldValue = currentTargetID;
    currentTargetID = value;
    emit currentTargetIDChanged(oldValue, value);
  }
}

int MainWindowViewModel::getCurrentShapeID() { return currentShapeID; }
void MainWindowViewModel::setCurrentShapeID(int value) {
  if (currentShapeID != value) {
    int oldValue = currentShapeID;
    currentShapeID = value;
    emit currentShapeIDChanged(oldValue, value);

    if (currentTargetID != -1) {
      updateTarget(currentTargetID, Target::Property::ShapeID, value);
    }
  }
}

void MainWindowViewModel::setPrevShapeDir(QString&& value) {
  if (value != prevShapeDir) {
    prevShapeDir = value;
  }
}

QString MainWindowViewModel::getPrevShapeDir() { return prevShapeDir; }

void MainWindowViewModel::setPrevConfigDir(QString&& value) {
  if (value != prevConfigDir) {
    prevConfigDir = value;
  }
}

QString MainWindowViewModel::getPrevConfigDir() { return prevConfigDir; }

void MainWindowViewModel::setPrevExportDir(QString&& value) {
  if (value != prevExportDir) {
    prevExportDir = value;
  }
}

QString MainWindowViewModel::getPrevExportDir() { return prevExportDir; }

const Target& MainWindowViewModel::getTarget(int id) {
  if (targetIDValid(id)) {
    return targetList[id];
  } else {
    logError("Target out of range: got ", id, ", expected [0, ",
             targetList.size(), ").");
    throw std::range_error("Target range error. See log for details.");
  }
}

void MainWindowViewModel::createTarget(Target newTarget) {
  targetList.push_back(newTarget);
  int id = targetList.size() - 1;
  emit targetCreated(id);
}

void MainWindowViewModel::updateTarget(int id, Target::Property pname,
                                       Target::PropertyType value) {
  if (targetIDValid(id)) {
    Target& t = targetList[id];
    if (t.getProperty(pname) != value) {
      t.setProperty(pname, value);
      emit targetUpdated(id, pname);
    }
  } else {
    logError("Target out of range: got ", id, ", expected [0, ",
             targetList.size(), ").");
    throw std::range_error("Target range error. See log for details.");
  }
}

Target MainWindowViewModel::removeTarget(int id) {
  if (targetIDValid(id)) {
    Target removedTarget = targetList[id];
    targetList.erase(targetList.begin() + id);  // TODO: check remove target
    emit targetRemoved(id);
    return removedTarget;
  } else {
    logError("Target out of range: got ", id, ", expected [0, ",
             targetList.size(), ").");
    throw std::range_error("Target range error. See log for details.");
  }
}

const QPixmap& MainWindowViewModel::getShape(int id) {
  if (shapeIDValid(id)) {
    auto path = (id < kDefaultShapeCount)
                    ? kDefaultShapePathList[id]
                    : shapePathList[id - kDefaultShapeCount];

    if (loadedShape.find(path) == loadedShape.end()) {
      QPixmap shape =
          QPixmap::fromImage(QImage(path).convertToFormat(QImage::Format_Mono));
      loadedShape[path] = shape;
    }
    return loadedShape[path];
  } else {
    logError("Shape out of range: got ", id, ", expected [0, ",
             shapePathList.size(), ").");
    throw std::range_error("Target range error. See log for details.");
  }
}

void MainWindowViewModel::loadShape(QString filepath) {
  shapePathList.push_back(filepath);
  emit shapeLoaded(shapePathList.size() + kDefaultShapeCount - 1);
}

void MainWindowViewModel::removeShape(int id) {
  if (shapeIDValid(id)) {
    if (id < kDefaultShapeCount) {
      logError("Cannot delete this shape.", id);
    }
    id -= kDefaultShapeCount;
    loadedShape.erase(shapePathList[id]);
    shapePathList.erase(shapePathList.begin() + id);

    for (int i = 0; i < targetList.size(); ++i) {
      if (targetList[i].shapeID == id) {
        updateTarget(i, Target::Property::ShapeID, 0);
      } else if (targetList[i].shapeID > id) {
        updateTarget(i, Target::Property::ShapeID, id - 1);
      } else {
        // No need to change
      }
    }

    emit shapeRemoved(id);
  } else {
    logError("Shape out of range: got ", id, ", expected [0, ",
             shapePathList.size(), ").");
    throw std::range_error("Target range error. See log for details.");
  }
}

const Canvas& MainWindowViewModel::getCanvas() { return canvas; }

void MainWindowViewModel::updateCanvas(Canvas::Property pname,
                                       Canvas::PropertyType value) {
  if (canvas.getProperty(pname) != value) {
    canvas.setProperty(pname, value);
    emit canvasUpdated(pname);
  }
}

void MainWindowViewModel::saveToFile(QString filename) {
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
  jsonMain.insert("crossedParity", QJsonValue(canvas.crossedParity));
  jsonMain.insert("targetList", QJsonValue(jsonTargetList));
  jsonMain.insert("shapeList", QJsonValue(jsonShapeList));

  file.write(QJsonDocument(jsonMain).toJson());
}

void MainWindowViewModel::loadFromFile(QString filename) {
  QFile file(filename);
  file.open(QFile::ReadOnly);

  QJsonDocument d = QJsonDocument::fromJson(file.readAll());

  updateCanvas(Canvas::Property::Width, d["width"].toInt());
  updateCanvas(Canvas::Property::Height, d["height"].toInt());
  updateCanvas(Canvas::Property::Foreground,
               QColor(d["foreground"].toString()));
  updateCanvas(Canvas::Property::Background,
               QColor(d["background"].toString()));
  updateCanvas(Canvas::Property::GrainSize, d["grainSize"].toInt());
  updateCanvas(Canvas::Property::CrossedParity, d["crossedParity"].toBool());

  for (int i = 0; i < targetList.size(); ++i) {
    removeTarget(0);
  }

  for (int i = 0; i < shapePathList.size(); ++i) {
    removeShape(0);
  }

  for (auto s : d["shapeList"].toArray()) {
    loadShape(s.toString());
  }

  for (auto t : d["targetList"].toArray()) {
    QJsonObject targetObj = t.toObject();

    createTarget(Target(targetObj["x"].toInt(), targetObj["y"].toInt(),
                        targetObj["scale"].toInt(), targetObj["rotate"].toInt(),
                        targetObj["parity"].toInt(),
                        targetObj["shapeID"].toInt(),
                        QColor(targetObj["color"].toString())));
  }
}

void MainWindowViewModel::exportImage(QString filename,
                                      imaging::StereoImageType type) {
  std::deque<QPixmap> targetImgList;
  targetImgList.resize(targetList.size());
  std::transform(targetList.begin(), targetList.end(), targetImgList.begin(),
                 [&](const Target& target) {
                   return imaging::renderTarget(canvas, target,
                                                getShape(target.shapeID), type);
                 });
  QPixmap img =
      imaging::renderStereoImage(canvas, targetList, targetImgList, type);
  img.save(filename, "PNG", 100);
}