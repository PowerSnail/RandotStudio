// Copyright (c) 2020 Han Jin
// Licensed under the MIT License <http://opensource.org/licenses/MIT>

#include <QApplication>
#include <QTranslator>
#include <QtDebug>
#include <QtGlobal>
#include <iostream>
#include "viewmodels/mainwindowvm.h"
#include "views/mainwindow.h"

void LoadTranslator(QApplication& app) {
  auto translator = new QTranslator(&app);

  if (!translator->load(QLocale::system(), "", "",
                        QApplication::applicationDirPath())) {
    qWarning() << "Failed to load translation: " << QLocale::system().name();
    delete translator;
    return;
  }

  QApplication::installTranslator(translator);
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
  LoadTranslator(a);

  MainWindowViewModel vm;
  MainWindow w(&vm);
  w.show();

  return QApplication::exec();
}
