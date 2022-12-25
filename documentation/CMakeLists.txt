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
      images/browse.png
      images/fadedfilemenu.png
      images/filedialog.png
      images/handbook.png
      images/icon.png
      images/mainwindow.png
      images/open.png
      images/wildcard.png
)

set(
  ContentFiles
      content/about.txt
      content/browse.html
      content/filedialog.html
      content/findfile.html
      content/index.html
      content/intro.html
      content/openfile.html
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
                        COMMAND qhelpgenerator ARGS ${CMAKE_CURRENT_LIST_DIR}/protodb.qhp -o ${CMAKE_CURRENT_BINARY_DIR}/protodb.qch
                        COMMAND qhelpgenerator ARGS ${CMAKE_CURRENT_LIST_DIR}/protodb.qhcp -o ${CMAKE_CURRENT_BINARY_DIR}/protodb.qhc
                   COMMENT "generating docs"
)

add_custom_target(${TARGET} ALL DEPENDS ${GeneratedHelpFiles})
target_sources(${TARGET} PRIVATE ${SOURCES_LIST})

if(${TARGET}_INSTALL)
    message(STATUS "Generating install for ${TARGET}")
    install(FILES ${GeneratedHelpFiles} DESTINATION "${CMAKE_INSTALL_PREFIX}/${DOC_PREFIX}/${TARGET_NAMESPACE}/" COMPONENT devel)
endif()
