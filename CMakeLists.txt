cmake_minimum_required(VERSION 3.5)

project(RandotStudio LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

IF(WIN32)
SET(GUI_TYPE WIN32)
ENDIF(WIN32)

QT5_ADD_TRANSLATION(
  QMFILES
  Translation/en_US.ts
  Translation/zh_CN.ts
)

add_executable(RandotStudio
  ${GUI_TYPE}
  Resources/RandotStudio.qrc
  Resources/RandotStudio.rc
  src/widgets/color_chooser.cpp
  src/widgets/color_chooser.h
  src/widgets/previewcanvas.cpp
  src/widgets/previewcanvas.h
  src/widgets/previewcanvasitem.cpp
  src/widgets/previewcanvasitem.h
  src/models/target.cpp
  src/models/target.h
  src/models/canvas.cpp
  src/models/canvas.h
  src/views/mainwindow.cpp
  src/views/mainwindow.h
  src/views/mainwindow.ui
  src/views/dialogabout.cpp
  src/views/dialogabout.h
  src/views/dialogabout.ui
  src/viewmodels/mainwindowvm.cpp
  src/viewmodels/mainwindowvm.h
  src/imaging/imaging.h
  src/imaging/imaging.cpp
  src/main.cpp
  ${QMFILES}
)

target_compile_options(RandotStudio PRIVATE -Wall -pedantic)
target_link_libraries(RandotStudio PRIVATE Qt5::Widgets)
