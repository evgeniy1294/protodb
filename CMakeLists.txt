cmake_minimum_required(VERSION 3.5)

project(protodb VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0079 NEW)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/app/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gui/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/)

add_dependencies(ProtodbGui ProtodbApp)
add_dependencies(ProtodbApp Configurable Factories Global qnlohmann qsol2)

target_link_libraries(ProtodbApp PUBLIC Configurable Factories Global qnlohmann qsol2)
target_link_libraries(ProtodbGui PUBLIC ProtodbApp)

#find_package(Lua REQUIRED)
#find_package(QT NAMES Qt5 COMPONENTS Core Widgets SerialPort REQUIRED)
#find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets SerialPort Network REQUIRED)
#find_library(ADS NAMES qtadvanceddocking REQUIRED)
#
#
#if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#    qt_add_executable(protodb
#        MANUAL_FINALIZATION
#        ${PROJECT_SOURCES}
#    )
## Define target properties for Android with Qt 6 as:
##    set_property(TARGET protodb APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
##                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
## For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
#else()
#    if(ANDROID)
#        add_library(protodb SHARED
#            ${PROJECT_SOURCES}
#        )
## Define properties for Android with Qt 5 after find_package() calls as:
##    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    else()
#        add_executable(protodb
#            ${PROJECT_SOURCES}
#        )
#    endif()
#endif()
#
#target_link_libraries(protodb PRIVATE ${LUA_LIBRARIES})
#target_link_libraries(protodb PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
#target_link_libraries(protodb PRIVATE Qt${QT_VERSION_MAJOR}::SerialPort)
#target_link_libraries(protodb PRIVATE Qt${QT_VERSION_MAJOR}::Network)
#target_link_libraries(protodb PRIVATE qtadvanceddocking)
#target_include_directories(protodb PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include" ${LUA_INCLUDE_DIR})
#
#set_target_properties(protodb PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
#    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#)

#if(QT_VERSION_MAJOR EQUAL 6)
#    qt_finalize_executable(protodb)
#endif()
