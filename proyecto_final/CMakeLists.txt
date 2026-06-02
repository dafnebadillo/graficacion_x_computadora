cmake_minimum_required(VERSION 3.16)

project(RayTracingShaders LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

qt_standard_project_setup()

qt_add_executable(RayTracingShaders
    main.cpp
    glwindow.cpp
    glwindow.h
    resources.qrc
)

target_link_libraries(RayTracingShaders
    PRIVATE
    Qt6::Widgets
    Qt6::OpenGLWidgets
)

