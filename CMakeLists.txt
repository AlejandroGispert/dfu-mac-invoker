cmake_minimum_required(VERSION 3.16)
project(dfu_gui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    main.cpp
    mainwindow.cpp
    usb/dfu_handler.cpp
)

set(HEADERS
    mainwindow.h
    usb/dfu_handler.h
)

set(UIS
    mainwindow.ui
)

qt6_wrap_ui(UI_HEADERS ${UIS})

add_executable(dfu_gui ${SOURCES} ${HEADERS})

# Add the build directory to the include paths so ui_mainwindow.h can be found
target_include_directories(dfu_gui PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(dfu_gui PRIVATE Qt6::Widgets)

if(APPLE)
    set_target_properties(dfu_gui PROPERTIES
        MACOSX_RPATH ON
    )
endif()
