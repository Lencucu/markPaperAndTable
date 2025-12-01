cmake_minimum_required(VERSION 3.5)

project(markPaperAndTable VERSION 0.1 LANGUAGES CXX)

list(APPEND CMAKE_PREFIX_PATH "E:/Package_Setup/qhotkey/lib/cmake/QHotkey")


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(QHotkey REQUIRED)
add_library(QHotkey::QHotkey ALIAS qhotkey)

set(PROJECT_SOURCES
        design.md
        main.cpp
        mptfloatplane.ui
        mptfloatplane.h
        mptfloatplane.cpp
        preview.ui
        .gitignore
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(markPaperAndTable
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET markPaperAndTable APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(markPaperAndTable SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(markPaperAndTable
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(markPaperAndTable PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    QHotkey::QHotkey
)

set_target_properties(markPaperAndTable PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS markPaperAndTable
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(markPaperAndTable)
endif()
