cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(TARGET sol)
set(TARGET_NAMESPACE sol)
set(INCLUDE_PREFIX "")
set(LIBRARY_DIST_SUFFIX "")

set(${TARGET}_VERSION "3.3.0")

project(${TARGET} VERSION ${${TARGET}_VERSION} LANGUAGES CXX)
option(${TARGET}_INSTALL "Generate the install target" OFF)

# ------------ [ Sources ] ------------

set(SOURCES_LIST
    include/sol/sol.hpp
    include/sol/config.hpp
)

add_library(${TARGET} INTERFACE)
target_sources(${TARGET} PRIVATE ${SOURCES_LIST})

add_library(${TARGET_NAMESPACE}::${TARGET} ALIAS ${TARGET})

target_include_directories(
  ${TARGET} INTERFACE
      include/
)

if(${TARGET}_INSTALL)
    message(STATUS "Generating install for ${TARGET}")

    # TODO: install sol

endif()

