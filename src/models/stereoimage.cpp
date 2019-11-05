#include "stereoimage.h"

#include <QFile>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QPainter>

StereoImage::StereoImage(int width, int height) : width(width), height(height) {
  for (auto s : kDefaultShapes) {
    shapeList.push_back(s);
  }
}

void StereoImage::loadFromFile(QString fname) {
  QFile file(fname);
  file.open(QFile::ReadOnly);
  QJsonDocument d = QJsonDocument::fromJson(file.readAll());

  width = d["width"].toInt();
  height = d["height"].toInt();
  foreground = QColor(d["foreground"].toString());
  background = QColor(d["background"].toString());
  grainSize = d["grainSize"].toInt();
  crossedParity = d["crossedParity"].toBool();

  shapeList.clear();
  for (auto s : kDefaultShapes) {
    shapeList.push_back(s);
  }
  for (auto s : d["shapeList"].toArray()) {
    shapeList.push_back(s.toString());
  }

  targetList.clear();
  for (auto t : d["targetList"].toArray()) {
    QJsonObject targetObj = t.toObject();
    Target target(targetObj["x"].toInt(), targetObj["y"].toInt(),
                  targetObj["scale"].toInt(), targetObj["rotate"].toInt(),
                  targetObj["parity"].toInt(), targetObj["shapeID"].toInt(),
                  QColor(targetObj["color"].toString()));
    // TODO: Deal with data error
    targetList.push_back(target);
  }
}

void StereoImage::saveToFile(QString filename) {
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
  for (auto it = shapeList.begin() + kDefaultShapes.size();
       it != shapeList.end(); ++it) {
    jsonShapeList.append(QJsonValue(*it));
  }

  QJsonObject jsonMain;
  jsonMain.insert("width", QJsonValue(width));
  jsonMain.insert("height", QJsonValue(height));
  jsonMain.insert("foreground", QJsonValue(foreground.name()));
  jsonMain.insert("background", QJsonValue(background.name()));
  jsonMain.insert("grainSize", QJsonValue(grainSize));
  jsonMain.insert("crossedParity", QJsonValue(crossedParity));
  jsonMain.insert("targetList", QJsonValue(jsonTargetList));
  jsonMain.insert("shapeList", QJsonValue(jsonShapeList));

  file.write(QJsonDocument(jsonMain).toJson());
}

const vector<QString> StereoImage::kDefaultShapes{
    QString(":randot_studio/default_shapes/e.png"),
    QString(":randot_studio/default_shapes/c.png")};