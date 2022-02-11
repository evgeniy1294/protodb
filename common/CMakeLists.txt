cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(TARGET common)
set(TARGET_NAMESPACE protodb)
set(INCLUDE_PREFIX "")
set(LIBRARY_DIST_SUFFIX "")

set(${TARGET}_VERSION "0.0.1")

project(${TARGET} VERSION ${${TARGET}_VERSION} LANGUAGES CXX)
option(${TARGET}_INSTALL "Generate the install target" OFF)

set(CMAKE_AUTOMOC ON)

# ------------ [ Sources ] ------------

set( BaseHeaders
    include/protodb/configurable/Configurable.h
    include/protodb/factories/CreatorAbstract.h
    include/protodb/factories/FactoryAbstract.h
    include/protodb/factories/GlobalFactoryStorage.h
    include/protodb/shared_types/LogTypes.h
    include/protodb/shared_types/Sequence.h
    include/protodb/utils/JsonUtils.h
    include/protodb/utils/SolByteArrayWrapper.h
    include/protodb/Logger.h
    include/protodb/LuaApi.h
    include/protodb/PluginManager.h
    include/protodb/SequenceModel.h
    include/protodb/Worker.h
)

set( BaseSources
    src/Configurable.cpp
    src/FactoryAbstract.cpp
    src/GlobalFactoryStorage.cpp
    src/JsonUtils.cpp
    src/Logger.cpp
    src/LuaApi.cpp
    src/PluginManager.cpp
    src/SequenceModel.cpp
    src/SolByteArrayWrapper.cpp
    src/Worker.cpp
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

find_package(Qt5 COMPONENTS Core REQUIRED)
target_link_libraries(${TARGET} PRIVATE Qt5::Core)

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

    # set(project_config_in "${CMAKE_CURRENT_BINARY_DIR}/cmake/${TARGET}_config.cmake.in")
    set(project_config_out  "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_config.cmake")
    set(version_config_file "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_config_version.cmake")
    set(config_targets_file "${TARGET}_config_targets.cmake")
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
