cmake_minimum_required(VERSION 3.16)

project(MiProyectoQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

qt_standard_project_setup()

qt_add_executable(Tarea7
    main.cpp
    glwindow.h
    glwindow.cpp
    math3d.h
    mesh.h
    mesh.cpp
)
target_link_libraries(Tarea7 PRIVATE
    Qt6::Widgets
    Qt6::OpenGLWidgets
)