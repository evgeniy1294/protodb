# External Projects
message(STATUS "Subproject: Qt ADS...")

# Download and unpack Qt ADS at configure time
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/qt_ads.CMakeLists.txt
  ${CMAKE_BINARY_DIR}/qt_ads_download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/qt_ads_download)

if(result)
  message(FATAL_ERROR "CMake step for Qt ADS failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/qt_ads_download)

if(result)
  message(FATAL_ERROR "Build step for Qt ADS failed: ${result}")
endif()

add_subdirectory(
        ${CMAKE_BINARY_DIR}/qt_ads_src
        ${CMAKE_BINARY_DIR}/qt_ads_build)
message(STATUS "Subproject: Qt ADS...DONE")
