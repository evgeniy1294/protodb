cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets REQUIRED)
find_package(Lua REQUIRED)

set(APP_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/protodb/)
set(APP_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(APP_HEADERS
        ${APP_HEADERS_DIR}/LogDecorator.h
        ${APP_HEADERS_DIR}/LogFormatter.h
        ${APP_HEADERS_DIR}/LogModel.h
        ${APP_HEADERS_DIR}/LuaApi.h
        ${APP_HEADERS_DIR}/SequenceAbstract.h
        ${APP_HEADERS_DIR}/SequenceModel.h
        ${APP_HEADERS_DIR}/Worker.h
)

set(APP_SOURCES
        ${APP_SOURCE_DIR}/LogDecorator.cpp
        ${APP_SOURCE_DIR}/LogFormatter.cpp
        ${APP_SOURCE_DIR}/LogModel.cpp
        ${APP_SOURCE_DIR}/LuaApi.cpp
        ${APP_SOURCE_DIR}/SequenceModel.cpp
        ${APP_SOURCE_DIR}/Worker.cpp
)
 
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(ProtodbApp STATIC ${APP_SOURCES} ${APP_HEADERS})
else()
    add_library(ProtodbApp STATIC ${APP_SOURCES} ${APP_HEADERS})
endif()

target_include_directories(ProtodbApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_include_directories(ProtodbApp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include" ${LUA_INCLUDE_DIR})

target_link_libraries(ProtodbApp PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(ProtodbApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(ProtodbApp PRIVATE ${LUA_LIBRARIES})
