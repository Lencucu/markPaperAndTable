cmake_minimum_required(VERSION 3.5)

project(markPaperAndTable VERSION 0.1 LANGUAGES CXX)

list(APPEND CMAKE_PREFIX_PATH "E:/Package_Setup/qhotkey/lib/cmake/QHotkey")


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)
find_package(QHotkey REQUIRED)
add_library(QHotkey::QHotkey ALIAS qhotkey)

set(PROJECT_SOURCES
        program_assets/notes/design.md
        program_assets/notes/timetest.md
        program_assets/logo/logo.png
        program_assets/recorder.db
        wrapper/include/interface_database/sql_wrapper.hpp
        wrapper/include/interface_resource/text_box.hpp
        wrapper/include/widgets/XHorizontalScrollArea.hpp
        wrapper/include/widgets/XHoverPlane.hpp
        wrapper/include/widgets/XPreview.hpp
        wrapper/include/global_uniforms.hpp
        wrapper/src/widgets/XHoverPlane.cpp
        wrapper/src/widgets/XPreview.cpp
        wrapper/src/global_uniforms.cpp
        main.cpp
        readme.md
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

target_include_directories(markPaperAndTable PRIVATE
    ${CMAKE_SOURCE_DIR}/3rd/lcctoolbox/include
    ${CMAKE_SOURCE_DIR}/wrapper/include
)

target_link_libraries(markPaperAndTable PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    QHotkey::QHotkey
    ${CMAKE_SOURCE_DIR}/3rd/lcctoolbox/theBuilder/output/dll/lcctoolbox.lib
)

add_custom_command(TARGET markPaperAndTable POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/3rd/lcctoolbox/theBuilder/output/dll/lcctoolbox.dll
    $<TARGET_FILE_DIR:markPaperAndTable>
COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/program_assets/notes
    $<TARGET_FILE_DIR:markPaperAndTable>/notes
COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/program_assets/logo
    $<TARGET_FILE_DIR:markPaperAndTable>
COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/program_assets/recorder.db
    $<TARGET_FILE_DIR:markPaperAndTable>
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
