cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets REQUIRED)

set(QNLOHMANN_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/protodb/nlohmann/)
set(QNLOHMANN_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(QNLOHMANN_HEADERS
        ${QNLOHMANN_HEADERS_DIR}/qnlohmann.h
)

set(QNLOHMANN_SOURCES
        ${QNLOHMANN_SOURCE_DIR}/qnlohmann.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(qnlohmann STATIC ${QNLOHMANN_SOURCES} ${QNLOHMANN_HEADERS})
else()
    add_library(qnlohmann STATIC ${QNLOHMANN_SOURCES} ${QNLOHMANN_HEADERS})
endif()

target_include_directories(qnlohmann PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

target_link_libraries(qnlohmann PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(qnlohmann PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
