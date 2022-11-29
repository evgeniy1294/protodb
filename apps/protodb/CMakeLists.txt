cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(TARGET protodb)
set(${TARGET}_VERSION "0.0.1")

project(${TARGET} VERSION ${${TARGET}_VERSION} LANGUAGES CXX)

option(${TARGET}_INSTALL "Generate the install target." OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# ------------ [ Sources ] ------------
set(SOURCES_LIST
    src/CellButtonDelegate.cpp
    src/CellCheckBoxDelegate.cpp
    src/CellSpinBoxDelegate.cpp
    src/ConnectionConfigDialog.cpp
    src/Logger.cpp
    src/LogFormatter.cpp
    src/LogPrinter.cpp
    src/LogDecorator.cpp
    src/LogDecorationDialog.cpp
    src/LogFormatWidget.cpp
    src/LogItemDelegate.cpp
    src/LogProxyModel.cpp
    src/LogTableView.cpp
    src/LogWidget.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/menu.cpp
    src/PluginDetailDialog.cpp
    src/PluginManagerDialog.cpp
    src/PluginTreeView.cpp
    src/Sequence.cpp
    src/SequenceScriptFormatter.cpp
    src/SequenceModel.cpp
    src/SequenceEditDialog.cpp
    src/SequenceEditDialog.cpp
    src/SequenceTableView.cpp
    src/SequenceTableWidget.cpp
    src/SequenceFormatSelectionWidget.cpp
    src/tool_bar.cpp
    src/ProtodbSessionManager.cpp
    src/MainClass.cpp
)

set(HEADERS_LIST
    include/CellButtonDelegate.h
    include/CellCheckBoxDelegate.h
    include/CellSpinBoxDelegate.h
    include/ConnectionConfigDialog.h
    include/Logger.h
    include/LogFormatter.h
    include/LogPrinter.h
    include/LogDecorator.h
    include/LogDecorationDialog.h
    include/LogFormatWidget.h
    include/LogItemDelegate.h
    include/LogProxyModel.h
    include/LogTableView.h
    include/LogWidget.h
    include/mainwindow.h
    include/menu.h
    include/PluginDetailDialog.h
    include/PluginManagerDialog.h
    include/PluginTreeView.h
    include/Sequence.h
    include/SequenceFormatter.h
    include/SequenceScriptFormatter.h
    include/SequenceModel.h
    include/SequenceEditDialog.h
    include/SequenceTableView.h
    include/SequenceTableWidget.h
    include/SequenceFormatSelectionWidget.h
    include/tool_bar.h
    include/ProtodbSessionManager.h
    include/MainClass.h
)

add_executable(${TARGET})
target_sources(${TARGET} PRIVATE ${SOURCES_LIST} ${HEADERS_LIST})
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

# ------------ [ Depends ] ------------
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_library(ADS NAMES qtadvanceddocking REQUIRED)
find_package(Lua REQUIRED)

target_link_libraries(${TARGET}

    PRIVATE

    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    ${LUA_LIBRARIES}
    qtadvanceddocking
)

target_link_libraries(${TARGET}

    PRIVATE

    protodb::base
    protodb::gui
    protodb::dev
)

# ------------ [ Properties and compile definitions ] ------------
set_target_properties(${TARGET} PROPERTIES
  CXX_STANDARD_REQUIRED ON
  CXX_STANDARD 17
)

#---------------------------------------------------------------------------------------
# Install
#---------------------------------------------------------------------------------------

include(GNUInstallDirs)

if(${TARGET}_INSTALL)
    message(STATUS "Generating install for ${TARGET}")
    install(TARGETS ${TARGET} EXPORT ${TARGET} DESTINATION "${CMAKE_INSTALL_BINDIR}")
endif()

