cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

set(FACTORIES_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/protodb/factories/)
set(FACTORIES_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(FACTORIES_HEADERS
        ${FACTORIES_HEADERS_DIR}/CreatorAbstract.h
        ${FACTORIES_HEADERS_DIR}/FactoryAbstract.h
        ${FACTORIES_HEADERS_DIR}/GlobalFactoryStorage.h
)

set(FACTORIES_SOURCES
        ${FACTORIES_SOURCE_DIR}/FactoryAbstract.cpp
        ${FACTORIES_SOURCE_DIR}/GlobalFactoryStorage.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(Factories STATIC ${FACTORIES_SOURCES} ${FACTORIES_HEADERS})
else()
    add_library(Factories STATIC ${FACTORIES_SOURCES} ${FACTORIES_HEADERS})
endif()

target_include_directories(Factories PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

target_link_libraries(Factories PRIVATE Qt${QT_VERSION_MAJOR}::Core)

