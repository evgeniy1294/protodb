cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

set(IODEVICE_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(IODEVICE_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR})

set(IODEVICE_HEADERS
        ${IODEVICE_HEADERS_DIR}/IODeviceCreatorInterface.h
        ${IODEVICE_HEADERS_DIR}/IODeviceFactory.h
)

set(IODEVICE_SOURCE
        ${IODEVICE_SOURCE_DIR}/IODeviceFactory.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(ProtodbIoDevice STATIC ${IODEVICE_SOURCE} ${IODEVICE_HEADERS})
else()
    add_library(ProtodbIoDevice STATIC ${IODEVICE_SOURCE} ${IODEVICE_HEADERS})
endif()

target_include_directories(ProtodbIoDevice PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ProtodbIoDevice PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(ProtodbIoDevice PUBLIC Configurable Factories)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/plugins)
