cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(ProtodbProject VERSION "0.0.1")

# Libs
add_subdirectory(libs/base)
add_subdirectory(libs/gui)
add_subdirectory(libs/dev)

# Apps
add_subdirectory(apps/protodb)

# Plugins
add_subdirectory(plugins/NetIOWidget)
add_subdirectory(plugins/SerialIOWidget)
add_subdirectory(plugins/SerialIODevice)
