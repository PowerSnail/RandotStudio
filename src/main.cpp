// Copyright (c) 2019 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#include <QApplication>
#include <QTranslator>
#include <QtDebug>
#include <QtGlobal>
#include <iostream>
#include "viewmodels/mainwindowvm.h"
#include "views/mainwindow.h"

void loadTranslator(QApplication& app) {
  QTranslator* translator = new QTranslator(&app);

  bool success = translator->load(QLocale::system(), "", "", app.applicationDirPath());
  if (!success) {
    qWarning() << "Failed to load translation: " << QLocale::system().name();
    delete translator;
    return;
  }

  app.installTranslator(translator);
  qDebug() << "Loaded translator: " << QLocale::system().name();

#ifdef Q_OS_WINDOWS
  if (QLocale::system().language() == QLocale::Chinese) {
    qDebug() << "Running in Windows with Chinese, set default font.";
    QFont font;
    font.setFamily("微软雅黑");
    app.setFont(font);
  }
#endif  // ifdef Q_OS_WINDOWS
}

int main(int argc, char* argv[]) {
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  QApplication a(argc, argv);
  loadTranslator(a);

  MainWindowViewModel vm;
  MainWindow w(&vm);
  w.show();

  return a.exec();
}
