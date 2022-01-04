cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets REQUIRED)

set(IOWIDGET_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(IOWIDGET_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR})

set(IOWIDGET_HEADERS
        ${IOWIDGET_HEADERS_DIR}/IOWidgetCreatorInterface.h
        ${IOWIDGET_HEADERS_DIR}/IOWidgetFactory.h
        ${IOWIDGET_HEADERS_DIR}/IOWidget.h
)

set(IOWIDGET_SOURCE
        ${IOWIDGET_SOURCE_DIR}/IOWidgetFactory.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(ProtodbIoWidget STATIC ${IOWIDGET_SOURCE} ${IOWIDGET_HEADERS})
else()
    add_library(ProtodbIoWidget STATIC ${IOWIDGET_SOURCE} ${IOWIDGET_HEADERS})
endif()

target_include_directories(ProtodbIoWidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ProtodbIoWidget PUBLIC Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(ProtodbIoWidget PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(ProtodbIoWidget PUBLIC Configurable Factories qnlohmann)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/plugins)

