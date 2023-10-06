cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(TARGET dev)
set(TARGET_NAMESPACE protodb)
set(INCLUDE_PREFIX "")
set(LIBRARY_DIST_SUFFIX "")

set(${TARGET}_VERSION "0.0.1")

project(${TARGET} VERSION ${${TARGET}_VERSION} LANGUAGES CXX)
option(${TARGET}_INSTALL "Generate the install target" ON)
option(${TARGET}_INSTALL_HEADERS "Generate the install target public headers" OFF)

set(CMAKE_AUTOMOC ON)

# ------------ [ Sources ] ------------

set(
  DevHeaders
      include/protodb/Seance.h
      include/protodb/Connection.h
      include/protodb/IOWidget.h
      include/protodb/ScriptInterface.h
      include/protodb/BytecodeEncoderAbstract.h
)

set(
  CreatorsHeaders
      include/protodb/creators/ConnectionCreator.h
      include/protodb/creators/IOWidgetCreatorInterface.h
      include/protodb/creators/ScriptInterfaceCreator.h
      include/protodb/creators/BytecodeEncoderCreator.h
)

set(
  FactoriesHeaders
      include/protodb/factories/ConnectionFactory.h
      include/protodb/factories/IOWidgetFactory.h
      include/protodb/factories/ScriptInterfaceFactory.h
      include/protodb/factories/BytecodeEncoderFactory.h
)

set( DevSources
    src/Seance.cpp
    src/IOWidgetFactory.cpp
    src/ConnectionFactory.cpp
    src/ScriptInterfaceFactory.cpp
    src/BytecodeEncoderFactory.cpp
)

set(SOURCES_LIST
    ${DevHeaders}
    ${CreatorsHeaders}
    ${FactoriesHeaders}
    ${DevSources}
)

add_library(${TARGET} SHARED)
target_sources(${TARGET} PRIVATE ${SOURCES_LIST})

add_library(${TARGET_NAMESPACE}::${TARGET} ALIAS ${TARGET})

# ------------ [ ExportHeader ] ------------

include(GNUInstallDirs)
include(GenerateExportHeader)

generate_export_header(${TARGET}
        EXPORT_FILE_NAME "${INCLUDE_PREFIX}${TARGET_NAMESPACE}/export/${TARGET}_cfg.hpp"
        EXPORT_MACRO_NAME PROTODB_DEV_EXPORT
)

# ------------ [ Depends ] ------------
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
if(NOT DEFINED QT_BUILD_VERSION)
    set(QT_BUILD_VERSION ${QT_VERSION_MAJOR})
endif()

find_package(Qt${QT_BUILD_VERSION} COMPONENTS Core Gui Widgets REQUIRED)
target_link_libraries(${TARGET}

    PRIVATE

    Qt${QT_BUILD_VERSION}::Core
    Qt${QT_BUILD_VERSION}::Gui
    Qt${QT_BUILD_VERSION}::Widgets
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

target_include_directories(
  ${TARGET} PRIVATE
      include/${TARGET_NAMESPACE}
      ${PROJECT_BINARY_DIR}/${INCLUDE_PREFIX}export/
)


if(${TARGET}_INSTALL)
    message(STATUS "Generating install for ${TARGET}")

    #---------------------------------------------------------------------------------------
    # if we plan to export built library to CMAKE_INSTALL_PREFIX
    # (by default it's /usr/local or C:/Program Files/${PROJECT_NAME})
    #---------------------------------------------------------------------------------------
    if(${TARGET}_INSTALL_HEADERS)
        install(FILES ${DevHeaders} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${INCLUDE_PREFIX}${TARGET_NAMESPACE}" COMPONENT devel)
        install(FILES ${CreatorsHeaders} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${INCLUDE_PREFIX}${TARGET_NAMESPACE}/creators" COMPONENT devel)
        install(FILES ${FactoriesHeaders} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${INCLUDE_PREFIX}${TARGET_NAMESPACE}/factories" COMPONENT devel)
        install(FILES "${PROJECT_BINARY_DIR}/${INCLUDE_PREFIX}${TARGET_NAMESPACE}/export/${TARGET}_cfg.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${INCLUDE_PREFIX}${TARGET_NAMESPACE}/export")
    endif()

    install(TARGETS ${TARGET} EXPORT ${TARGET}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/${TARGET_NAMESPACE}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/${TARGET_NAMESPACE}"
    )

    #---------------------------------------------------------------------------------------
    # installing cmake config file allows importing our library into other projects via "find_package"
    #---------------------------------------------------------------------------------------

    # set(project_config_in "${CMAKE_CURRENT_BINARY_DIR}/cmake/${TARGET}_config.cmake.in")
    set(project_config_out  "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_config.cmake")
    set(version_config_file "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_config_version.cmake")
    set(config_targets_file "${TARGET}_config_targets.cmake")
    set(export_dest_dir     "${CMAKE_INSTALL_LIBDIR}/${TARGET_NAMESPACE}/cmake")

    install(EXPORT ${TARGET}
            DESTINATION ${export_dest_dir}
            NAMESPACE ${TARGET_NAMESPACE}::
            FILE ${config_targets_file})

    include(CMakePackageConfigHelpers)
    # configure_file("${project_config_in}" "${project_config_out}" @ONLY)
    write_basic_package_version_file("${version_config_file}" COMPATIBILITY SameMajorVersion)
    install(FILES "${version_config_file}" DESTINATION "${export_dest_dir}")

endif()

