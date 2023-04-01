cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(TARGET protodb)
set(${TARGET}_VERSION "1.0.3")

project(${TARGET} VERSION ${${TARGET}_VERSION} LANGUAGES CXX)

option(${TARGET}_INSTALL "Generate the install target." ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# ------------ [ Sources ] ------------
set(SOURCES_LIST
    src/Assistant.cpp
    src/CellButtonDelegate.cpp
    src/CellCheckBoxDelegate.cpp
    src/CellSpinBoxDelegate.cpp
    src/SeanseConfigDialog.cpp
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
    src/Sequence.cpp
    src/SequenceScriptFormatter.cpp
    src/SequenceModel.cpp
    src/SequenceEditDialog.cpp
    src/SequenceEditDialog.cpp
    src/SequenceTableView.cpp
    src/SequenceTableWidget.cpp
    src/SequenceFormatSelectionWidget.cpp
    src/ScriptInterfaceGroup.cpp
    src/tool_bar.cpp
    src/ProtodbSessionManager.cpp
    src/ProtodbConfigDialog.cpp
    src/ProtodbConfigStorage.cpp
    src/MainClass.cpp
    src/DelimetersConfigWidget.cpp
    src/Delimeter.cpp
    src/CrcCalculator.cpp
)

set(HEADERS_LIST
    include/Assistant.h
    include/CellButtonDelegate.h
    include/CellCheckBoxDelegate.h
    include/CellSpinBoxDelegate.h
    include/SeanseConfigDialog.h
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
    include/Sequence.h
    include/SequenceFormatter.h
    include/SequenceScriptFormatter.h
    include/SequenceModel.h
    include/SequenceEditDialog.h
    include/SequenceTableView.h
    include/SequenceTableWidget.h
    include/SequenceFormatSelectionWidget.h
    include/ScriptInterfaceGroup.h
    include/tool_bar.h
    include/ProtodbSessionManager.h
    include/ProtodbConfigDialog.h
    include/ProtodbConfigStorage.h
    include/MainClass.h
    include/DelimetersConfigWidget.h
    include/Delimeter.h
    include/CrcCalculator.h
)

set( WIN_RESOURCES
    resources/protodb.rc
)

add_executable(${TARGET} WIN32)
target_sources(${TARGET} PRIVATE ${SOURCES_LIST} ${HEADERS_LIST} ${WIN_RESOURCES})
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

# ------------ [ Depends ] ------------
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
if(NOT DEFINED QT_BUILD_VERSION)
    set(QT_BUILD_VERSION ${QT_VERSION_MAJOR})
endif()

find_package(Qt${QT_BUILD_VERSION} COMPONENTS Core Gui Widgets REQUIRED)

find_package(Lua REQUIRED)

target_link_libraries(${TARGET}

    PRIVATE

    Qt${QT_BUILD_VERSION}::Core
    Qt${QT_BUILD_VERSION}::Gui
    Qt${QT_BUILD_VERSION}::Widgets
    qtadvanceddocking
    ${LUA_LIBRARIES}
    sol::sol
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

