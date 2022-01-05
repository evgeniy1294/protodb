cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets REQUIRED)
find_library(ADS NAMES qtadvanceddocking REQUIRED)
find_package(Lua REQUIRED)

set(GUI_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/protodb/)
set(GUI_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(GUI_HEADERS
        ${GUI_HEADERS_DIR}/CellButtonDelegate.h
        ${GUI_HEADERS_DIR}/CellCheckBoxDelegate.h
        ${GUI_HEADERS_DIR}/CellSpinBoxDelegate.h
        ${GUI_HEADERS_DIR}/ConnectionConfigDialog.h
        ${GUI_HEADERS_DIR}/LogDecorationDialog.h
        ${GUI_HEADERS_DIR}/LogFormatWidget.h
        ${GUI_HEADERS_DIR}/LogItemDelegate.h
        ${GUI_HEADERS_DIR}/LogProxyModel.h
        ${GUI_HEADERS_DIR}/LogStyleConfigWidget.h
        ${GUI_HEADERS_DIR}/LogTableView.h
        ${GUI_HEADERS_DIR}/LogWidget.h
        ${GUI_HEADERS_DIR}/mainwindow.h
        ${GUI_HEADERS_DIR}/menu.h
        ${GUI_HEADERS_DIR}/SequenceEditDialog.h
        ${GUI_HEADERS_DIR}/SequenceTableView.h
        ${GUI_HEADERS_DIR}/SequenceTableWidget.h
        ${GUI_HEADERS_DIR}/PluginManagerDialog.h
        ${GUI_HEADERS_DIR}/PluginDetailDialog.h
        ${GUI_HEADERS_DIR}/PluginTreeView.h
        ${GUI_HEADERS_DIR}/tool_bar.h
)

set(GUI_SOURCES
        ${GUI_SOURCE_DIR}/main.cpp
        ${GUI_SOURCE_DIR}/mainwindow.cpp
        ${GUI_SOURCE_DIR}/CellButtonDelegate.cpp
        ${GUI_SOURCE_DIR}/CellCheckBoxDelegate.cpp
        ${GUI_SOURCE_DIR}/CellSpinBoxDelegate.cpp
        ${GUI_SOURCE_DIR}/ConnectionConfigDialog.cpp
        ${GUI_SOURCE_DIR}/LogDecorationDialog.cpp
        ${GUI_SOURCE_DIR}/LogFormatWidget.cpp
        ${GUI_SOURCE_DIR}/LogItemDelegate.cpp
        ${GUI_SOURCE_DIR}/LogProxyModel.cpp
        ${GUI_SOURCE_DIR}/LogStyleConfigWidget.cpp
        ${GUI_SOURCE_DIR}/LogTableView.cpp
        ${GUI_SOURCE_DIR}/LogWidget.cpp
        ${GUI_SOURCE_DIR}/menu.cpp
        ${GUI_SOURCE_DIR}/SequenceEditDialog.cpp
        ${GUI_SOURCE_DIR}/SequenceTableView.cpp
        ${GUI_SOURCE_DIR}/SequenceTableWidget.cpp
        ${GUI_SOURCE_DIR}/PluginManagerDialog.cpp
        ${GUI_SOURCE_DIR}/PluginDetailDialog.cpp
        ${GUI_SOURCE_DIR}/PluginTreeView.cpp
        ${GUI_SOURCE_DIR}/tool_bar.cpp

        ${CMAKE_SOURCE_DIR}/resources/resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ProtodbGui ${GUI_SOURCES} ${GUI_HEADERS})
else()
    add_executable(ProtodbGui ${GUI_SOURCES} ${GUI_HEADERS})
endif()

target_include_directories(ProtodbGui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

target_link_libraries(ProtodbGui PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(ProtodbGui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(ProtodbGui PRIVATE qtadvanceddocking)
target_link_libraries(ProtodbGui PRIVATE ${LUA_LIBRARIES})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ProtodbGui)
endif()
