cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(TARGET base)
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
  ConfigurableHeaders
      include/protodb/configurable/Configurable.h
      include/protodb/configurable/ConfigAggregator.h
      include/protodb/configurable/ConfigManager.h
)

set(
  FactoriesHeaders
      include/protodb/factories/CreatorAbstract.h
      include/protodb/factories/FactoryAbstract.h
      include/protodb/factories/GlobalFactoryStorage.h
)

set(
  UtilsHeaders
      include/protodb/utils/JsonBaseUtils.h
      include/protodb/utils/SolByteArrayWrapper.h
      include/protodb/utils/FsIcon.h
      include/protodb/utils/JsonKeeper.h
      include/protodb/utils/ziputils.h
      include/protodb/utils/crc_logic.h
)

set(
  PluginsHeaders
      include/protodb/plugins/PluginManager.h
)

set( BaseSources
    src/Configurable.cpp
    src/ConfigAggregator.cpp
    src/ConfigManager.cpp
    src/FactoryAbstract.cpp
    src/GlobalFactoryStorage.cpp
    src/JsonBaseUtils.cpp
    src/PluginManager.cpp
    src/SolByteArrayWrapper.cpp
    src/ziputils.cpp
    src/crc_logic.cpp
)

set(SOURCES_LIST
    ${ConfigurableHeaders}
    ${FactoriesHeaders}
    ${UtilsHeaders}
    ${PluginsHeaders}
    ${BaseSources}
)

add_library(${TARGET} SHARED)
target_sources(${TARGET} PRIVATE ${SOURCES_LIST})

add_library(${TARGET_NAMESPACE}::${TARGET} ALIAS ${TARGET})

# ------------ [ ExportHeader ] ------------

include(GNUInstallDirs)
include(GenerateExportHeader)

generate_export_header(${TARGET}
        EXPORT_FILE_NAME "${INCLUDE_PREFIX}${TARGET_NAMESPACE}/export/${TARGET}_cfg.hpp"
        EXPORT_MACRO_NAME PROTODB_BASE_EXPORT
)

# ------------ [ Depends ] ------------
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
if(NOT DEFINED QT_BUILD_VERSION)
    set(QT_BUILD_VERSION ${QT_VERSION_MAJOR})
endif()

find_package(Qt${QT_BUILD_VERSION} COMPONENTS Core Gui Widgets REQUIRED)
find_package(libzip REQUIRED)
target_link_libraries(${TARGET}

    PRIVATE

    Qt${QT_BUILD_VERSION}::Core
    libzip::zip
    ${LUA_LIBRARIES}
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
        install(FILES ${ConfigurableHeaders} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${INCLUDE_PREFIX}${TARGET_NAMESPACE}/configurable/" COMPONENT devel)
        install(FILES ${FactoriesHeaders} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${INCLUDE_PREFIX}${TARGET_NAMESPACE}/factories" COMPONENT devel)
        install(FILES ${UtilsHeaders} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${INCLUDE_PREFIX}${TARGET_NAMESPACE}/utils" COMPONENT devel)
        install(FILES ${PluginsHeaders} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${INCLUDE_PREFIX}${TARGET_NAMESPACE}/plugins" COMPONENT devel)
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
