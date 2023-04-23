cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(TARGET SerialIoDevice)
set(TARGET_NAMESPACE protodb)
set(INCLUDE_PREFIX "")
set(LIBRARY_DIST_SUFFIX "")
set(${TARGET}_VERSION "0.0.1")

project(${TARGET} VERSION ${${TARGET}_VERSION} LANGUAGES CXX)
option(${TARGET}_INSTALL "Generate the install target" ON)

set(CMAKE_AUTOMOC ON)

# ------------ [ Sources ] ------------
set( BaseHeaders
    include/protodb/SerialIODeviceCreator.h
    include/protodb/SerialIoSeance.h
)

set( BaseSources
    src/SerialIODeviceCreator.cpp
    src/SerialIoSeance.cpp
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
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
if(NOT DEFINED QT_BUILD_VERSION)
    set(QT_BUILD_VERSION ${QT_VERSION_MAJOR})
endif()

find_package(Qt${QT_BUILD_VERSION} COMPONENTS Core SerialPort REQUIRED)

target_link_libraries(${TARGET}

    PRIVATE

    Qt${QT_BUILD_VERSION}::Core
    Qt${QT_BUILD_VERSION}::SerialPort
)

target_link_libraries(${TARGET}

    PRIVATE

    protodb::base
    protodb::dev
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

    install(TARGETS ${TARGET} EXPORT ${TARGET}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/plugins"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/${TARGET_NAMESPACE}/plugins"
    )


endif()
