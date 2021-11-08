cmake_minimum_required(VERSION 3.5)

project(protodb VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Lua REQUIRED)
find_package(QT NAMES Qt5 COMPONENTS Core Widgets SerialPort REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets SerialPort REQUIRED)
find_library(ADS NAMES qtadvanceddocking REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp
        src/Core.cpp
        src/Worker.cpp
        src/CellSpinBoxDelegate.cpp
        src/CellButtonDelegate.cpp
        src/CellCheckBoxDelegate.cpp
        src/mainwindow.cpp
        src/script_widget.cpp
        src/menu.cpp
        src/tool_bar.cpp
        src/singleton.cpp
        src/sq_table_widget.cpp
        src/SequenceTableModel.cpp
        src/sq_table_dialog.cpp
        src/LogConfigWidget.cpp
        src/settings_dialog.cpp
        src/Logger.cpp
        src/LogWidget.cpp
        src/LogTableModel.cpp
        src/LogItemDelegate.cpp
        src/LogItemFormatter.cpp
        src/SerialConfigWidget.cpp
        src/ConnectionConfigDialog.cpp
        src/CodeEditorWidget.cpp

        include/Core.h
        include/Worker.h
        include/mainwindow.h
        include/script_widget.h
        include/menu.h
        include/tool_bar.h
        include/SequenceAbstract.h
        include/singleton.h
        include/sq_table_widget.h
        include/SequenceTableModel.h
        include/sq_table_dialog.h
        include/LogConfigWidget.h
        include/settings_dialog.h
        include/Logger.h
        include/LogTableModel.h
        include/LogItemDelegate.h
        include/LogWidget.h
        include/LogItemFormatter.h
        include/SerialConfigWidget.h
        include/ConnectionConfigDialog.h
        include/CellButtonDelegate.h
        include/CellSpinBoxDelegate.h
        include/CellCheckBoxDelegate.h
        include/CodeEditorWidget.h
        resources/resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(protodb
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET protodb APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(protodb SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(protodb
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(protodb PRIVATE ${LUA_LIBRARIES})
target_link_libraries(protodb PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(protodb PRIVATE Qt${QT_VERSION_MAJOR}::SerialPort)
target_link_libraries(protodb PRIVATE qtadvanceddocking)
target_include_directories(protodb PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include" ${LUA_INCLUDE_DIR})

set_target_properties(protodb PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(protodb)
endif()
