cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(ProtodbProject VERSION "0.0.1")

# Libs
add_subdirectory(libs/base)
add_subdirectory(libs/gui)
add_subdirectory(libs/dev)
add_subdirectory(libs/sol)

# Apps
add_subdirectory(apps/protodb)

# Plugins
add_subdirectory(plugins/NetIOWidget)
add_subdirectory(plugins/NetIODevice)
add_subdirectory(plugins/SerialIOWidget)
add_subdirectory(plugins/SerialIODevice)
add_subdirectory(plugins/LuaScriptInterface)

# docs
add_subdirectory(documentation/)

# External Projects
message(STATUS "Subproject: Qt ADS...")

# Download and unpack Qt ADS at configure time
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/qt_ads.CMakeLists.txt
  ${CMAKE_BINARY_DIR}/qt-ads-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/qt-ads-download)

if(result)
  message(FATAL_ERROR "CMake step for Qt ADS failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/qt-ads-download)

if(result)
  message(FATAL_ERROR "Build step for Qt ADS failed: ${result}")
endif()
