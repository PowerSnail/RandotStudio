#include "views/mainwindow.h"
#include "viewmodels/mainwindowvm.h"
#include "utils/logging.h"

#include <iostream>
#include <QApplication>
#include <QTranslator>
#include <QtGlobal>

using namespace logging;

void loadTranslator(QApplication& app) {
  QTranslator* translator = new QTranslator(&app);

  bool success = translator->load(
    QLocale::system(), "", "", app.applicationDirPath()
  );
  if (!success) {
    logError("Failed to load translation: ", QLocale::system().name());
    delete translator;
    return;
  }
  
  app.installTranslator(translator);
  logDebug("Loaded translator: ", QLocale::system().name());


  #ifdef Q_OS_WINDOWS
    if (QLocale::system().language() == QLocale::Chinese) {
      logDebug("Running in Windows with Chinese, set default font.");
      QFont font;
      font.setFamily("微软雅黑");
      app.setFont(font);
    }
  #endif  // ifdef Q_OS_WINDOWS


};


int main(int argc, char *argv[]) {
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  QApplication a(argc, argv);
  loadTranslator(a);

  MainWindowViewModel vm;
  MainWindow w(&vm);
  w.show();

  return a.exec();
}
