cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Lua REQUIRED)

set(QSOL2_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/protodb/sol2/)
set(QSOL2_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(QSOL2_HEADERS
        ${QSOL2_HEADERS_DIR}/SolByteArrayWrapper.h
)

set(QSOL2_SOURCES
        ${QSOL2_SOURCE_DIR}/SolByteArrayWrapper.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(qsol2 STATIC ${QSOL2_SOURCES} ${QSOL2_HEADERS})
else()
    add_library(qsol2 STATIC ${QSOL2_SOURCES} ${QSOL2_HEADERS})
endif()

target_include_directories(qsol2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_include_directories(ProtodbApp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include" ${LUA_INCLUDE_DIR})

target_link_libraries(qsol2 PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(qsol2 PRIVATE ${LUA_LIBRARIES})
