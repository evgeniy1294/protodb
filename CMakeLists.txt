project(ProtodbProject)

# Libs
add_subdirectory(libs/base)
add_subdirectory(libs/gui)

# Apps
add_subdirectory(apps/protodb)

# Plugins
add_subdirectory(plugins/NetIOWidget)
add_subdirectory(plugins/SerialIOWidget)
add_subdirectory(plugins/SerialIODevice)
