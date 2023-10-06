cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(TARGET docs)
set(TARGET_NAMESPACE protodb)
#set(DOC_PREFIX "/usr/share/doc")

set(${TARGET}_VERSION "0.0.1")

project(${TARGET} VERSION ${${TARGET}_VERSION} LANGUAGES CXX)
option(${TARGET}_INSTALL "Generate the install target" ON)

# ------------ [ Sources ] ------------

set(
  ContentFiles
      content/about.txt
      content/index.html
      content/mainwindow.html
      content/options.html
      content/plugins.html
      content/scripting.html
      content/scripting_lua.html
      content/seance.html
      content/seance_config.html
      content/session.html
      content/log.html
      content/tables.html
      content/crc_calculator.html
)

set(SOURCES_LIST
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

add_custom_target(${TARGET} ALL DEPENDS ${GeneratedHelpFiles} SOURCES ${SOURCES_LIST})

if(${TARGET}_INSTALL)
    message(STATUS "Generating install for ${TARGET}")
    if (WIN32)
        install(FILES ${GeneratedHelpFiles} DESTINATION "${CMAKE_INSTALL_BINDIR}/doc/" COMPONENT devel)
    else()
        install(FILES ${GeneratedHelpFiles} DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/doc/${TARGET_NAMESPACE}/" COMPONENT devel)
    endif (WIN32)
endif()
