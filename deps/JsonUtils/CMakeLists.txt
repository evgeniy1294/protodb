cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets REQUIRED)

set(JSON_UTILS_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/protodb/JsonUtils/)
set(JSON_UTILS_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(JSON_UTILS_HEADERS
        ${JSON_UTILS_HEADERS_DIR}/JsonUtils.h
)

set(JSON_UTILS_SOURCES
        ${JSON_UTILS_SOURCE_DIR}/JsonUtils.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(JsonUtils STATIC ${JSON_UTILS_SOURCES} ${JSON_UTILS_HEADERS})
else()
    add_library(JsonUtils STATIC ${JSON_UTILS_SOURCES} ${JSON_UTILS_HEADERS})
endif()

target_include_directories(JsonUtils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

target_link_libraries(JsonUtils PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(JsonUtils PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
