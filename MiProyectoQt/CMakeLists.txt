cmake_minimum_required(VERSION 3.16)

project(MiProyectoQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets 3DCore 3DRender 3DExtras)

qt_standard_project_setup()

qt_add_executable(MiProyectoQt
    main.cpp
)

target_link_libraries(MiProyectoQt PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::3DCore
    Qt6::3DRender
    Qt6::3DExtras
)
