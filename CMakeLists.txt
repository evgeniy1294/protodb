cmake_minimum_required(VERSION 3.5)

project(protodb VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0079 NEW)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/app/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gui/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/plugins/)

add_dependencies(ProtodbGui ProtodbApp)
add_dependencies(ProtodbApp ProtodbIoDevice ProtodbIoWidget Configurable Factories Global JsonUtils qsol2)
add_dependencies(ProtodbIoDevice Configurable Factories)
add_dependencies(ProtodbIoWidget Configurable Factories)

target_link_libraries(ProtodbApp PUBLIC  Configurable Factories Global JsonUtils qsol2)
target_link_libraries(ProtodbGui PRIVATE ProtodbApp ProtodbIoWidget)
