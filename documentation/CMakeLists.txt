cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(TARGET docs)
set(TARGET_NAMESPACE protodb)
set(DOC_PREFIX "/usr/share/doc")

set(${TARGET}_VERSION "0.0.1")

project(${TARGET} VERSION ${${TARGET}_VERSION} LANGUAGES CXX)
option(${TARGET}_INSTALL "Generate the install target" ON)

# ------------ [ Sources ] ------------

set(
  Images
      content/images/handbook.png
      content/images/mainwindow.png
      content/images/mainwindow_mod.png
      content/images/mainwindow_label.png
      content/images/seance.png
      content/images/seance_config.png
      content/images/log.png
      content/images/log_decoration.png
      content/images/log_filtered.png
)

set(
  ContentFiles
      content/about.txt
      content/index.html
      content/mainwindow.html
      content/options.html
      content/plugins.html
      content/scripting.html
      content/seance.html
      content/seance_config.html
      content/session.html
      content/log.html
      content/tables.html
)

set(SOURCES_LIST
    ${Images}
    ${ContentFiles}
    protodb.qhp
    protodb.qhcp
)

set(GeneratedHelpFiles
    ${CMAKE_CURRENT_BINARY_DIR}/protodb.qch
    ${CMAKE_CURRENT_BINARY_DIR}/protodb.qhc
)

add_custom_command(OUTPUT ${GeneratedHelpFiles}
                        COMMAND qhelpgenerator ARGS ${CMAKE_CURRENT_LIST_DIR}/protodb.qhcp -o ${CMAKE_CURRENT_LIST_DIR}/protodb.qhc
                        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/protodb.qch ${CMAKE_CURRENT_LIST_DIR}/protodb.qhc ${CMAKE_CURRENT_BINARY_DIR}/
                        COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_CURRENT_LIST_DIR}/protodb.qch ${CMAKE_CURRENT_LIST_DIR}/protodb.qhc
                   COMMENT "generating docs"
)

add_custom_target(${TARGET} ALL DEPENDS ${GeneratedHelpFiles})
target_sources(${TARGET} PRIVATE ${SOURCES_LIST})

if(${TARGET}_INSTALL)
    message(STATUS "Generating install for ${TARGET}")
    install(FILES ${GeneratedHelpFiles} DESTINATION "${CMAKE_INSTALL_PREFIX}/${DOC_PREFIX}/${TARGET_NAMESPACE}/" COMPONENT devel)
endif()
