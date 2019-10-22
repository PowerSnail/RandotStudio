cmake_minimum_required(VERSION 3.5)

project(RandotStudio LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(RandotStudio
  src/main.cpp
  src/models/target.cpp
  src/models/target.h
  src/models/stereoimage.cpp
  src/models/stereoimage.h
  src/models/shape.cpp
  src/models/shape.h
  src/views/mainwindow.cpp
  src/views/mainwindow.h
  src/views/mainwindow.ui
  src/views/canvas_setting_panel.cpp
  src/views/canvas_setting_panel.h
  src/views/canvas_setting_panel.ui
  src/views/target_setting_panel.cpp
  src/views/target_setting_panel.h
  src/views/target_setting_panel.ui
  src/widgets/color_chooser.cpp
  src/widgets/color_chooser.h
  src/widgets/color_chooser.ui
  Resources/RandotStudio.qrc
  )

target_link_libraries(RandotStudio PRIVATE Qt5::Widgets)
