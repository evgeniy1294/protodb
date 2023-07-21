cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(ProtodbProject VERSION "1.0.2")

# Libs
add_subdirectory(libs/base)
add_subdirectory(libs/gui)
add_subdirectory(libs/dev)
add_subdirectory(libs/sol)
add_subdirectory(cmake)

# Apps
add_subdirectory(apps/protodb)

# Plugins
add_subdirectory(plugins/NetIOWidget)
add_subdirectory(plugins/NetworkConnection)
add_subdirectory(plugins/SerialIOWidget)
add_subdirectory(plugins/SerialConnection)
add_subdirectory(plugins/LuaScriptInterface)

# docs
add_subdirectory(documentation/)
