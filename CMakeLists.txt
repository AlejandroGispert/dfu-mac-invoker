cmake_minimum_required(VERSION 3.16)
project(dfu_gui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

# List your sources, headers, and UI files
set(SOURCES
    main.cpp
    mainwindow.cpp
    usb/dfu_handler.cpp
    usb/usb_scan.cpp
)
set(HEADERS
    mainwindow.h
    usb/dfu_handler.h
    usb/usb_scan.h
)
set(UIS
    mainwindow.ui
)

qt6_wrap_ui(UI_HEADERS ${UIS})

add_executable(dfu_gui ${SOURCES} ${HEADERS} ${UI_HEADERS})

target_include_directories(dfu_gui PRIVATE ${LIBUSB_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(dfu_gui PRIVATE Qt6::Widgets ${LIBUSB_LIBRARIES})

# Find macOS frameworks
find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
find_library(IOKIT_FRAMEWORK IOKit)

target_link_libraries(dfu_gui PRIVATE
    ${COREFOUNDATION_FRAMEWORK}
    ${IOKIT_FRAMEWORK}
)

if(APPLE)
    set_target_properties(dfu_gui PROPERTIES
        MACOSX_RPATH ON
    )
endif()
