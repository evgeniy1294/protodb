cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(TARGET NetIoWidget)
set(TARGET_NAMESPACE protodb)
set(INCLUDE_PREFIX "")
set(LIBRARY_DIST_SUFFIX "")
set(${TARGET}_VERSION "0.0.1")

project(${TARGET} VERSION ${${TARGET}_VERSION} LANGUAGES CXX)
option(${TARGET}_INSTALL "Generate the install target" OFF)

set(CMAKE_AUTOMOC ON)

# ------------ [ Sources ] ------------
set( BaseHeaders
    include/NetIOWidget.h
    include/NetIOWidgetCreator.h
)

set( BaseSources
    src/NetIOWidget.cpp
    src/NetIOWidgetCreator.cpp
)

set(SOURCES_LIST
    ${BaseHeaders}
    ${BaseSources}
)

add_library(${TARGET} SHARED)
target_sources(${TARGET} PRIVATE ${SOURCES_LIST})

add_library(${TARGET_NAMESPACE}::${TARGET} ALIAS ${TARGET})

# ------------ [ ExportHeader ] ------------
include(GNUInstallDirs)
include(GenerateExportHeader)

generate_export_header(${TARGET} EXPORT_FILE_NAME ${INCLUDE_PREFIX}${TARGET}/${TARGET}_cfg.hpp)

# ------------ [ Depends ] ------------
find_package(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)

target_link_libraries(${TARGET}

    PRIVATE

    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Network
)

target_link_libraries(${TARGET}

    PRIVATE

    protodb::base
    protodb::gui
)

# ------------ [ Properties and compile definitions ] ------------
set_target_properties(${TARGET} PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_STANDARD 17
)

target_include_directories(${TARGET} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/>
)

#---------------------------------------------------------------------------------------
# install
#---------------------------------------------------------------------------------------
if(${TARGET}_INSTALL)
    message(STATUS "Generating install for ${TARGET}")

    #---------------------------------------------------------------------------------------
    # if we plan to export built library to CMAKE_INSTALL_PREFIX
    # (by default it's /usr/local or C:/Program Files/${PROJECT_NAME})
    #---------------------------------------------------------------------------------------

    install(FILES ${PROJECT_BINARY_DIR}/${INCLUDE_PREFIX}${TARGET}/${TARGET}_cfg.hpp DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${INCLUDE_PREFIX}${TARGET}")
    install(TARGETS ${TARGET} EXPORT ${TARGET}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_BINDIR}${LIBRARY_DIST_SUFFIX}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/${TARGET}"
    )

    #---------------------------------------------------------------------------------------
    # installing cmake config file allows importing our library into other projects via "find_package"
    #---------------------------------------------------------------------------------------

    # set(project_config_in "${CMAKE_CURRENT_BINARY_DIR}/cmake/${TARGET}Config.cmake.in")
    set(project_config_out  "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}Config.cmake")
    set(version_config_file "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}ConfigVersion.cmake")
    set(config_targets_file "${TARGET}ConfigTargets.cmake")
    set(export_dest_dir     "${CMAKE_INSTALL_LIBDIR}/${TARGET}/cmake")

    install(EXPORT ${TARGET}
            DESTINATION ${export_dest_dir}
            NAMESPACE ${TARGET_NAMESPACE}::
            FILE ${config_targets_file})

    include(CMakePackageConfigHelpers)
    # configure_file("${project_config_in}" "${project_config_out}" @ONLY)
    write_basic_package_version_file("${version_config_file}" COMPATIBILITY SameMajorVersion)
    install(FILES "${version_config_file}" DESTINATION "${export_dest_dir}")

endif()
