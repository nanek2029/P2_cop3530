cmake_minimum_required(VERSION 3.20)
project(P2_cop3530)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)

add_executable(P2_cop3530
        src/main.cpp
        src/SearchAlgorithms.cpp
        src/SearchAlgorithms.h
        src/logic.cpp
        src/logic.h
        src/MainWindow.cpp
        src/MainWindow.h)

target_link_libraries(P2_cop3530 PRIVATE Qt6::Widgets)

target_include_directories(P2_cop3530 PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/bridges
        ${CMAKE_SOURCE_DIR}/src/bridges/data_src
        ${CMAKE_SOURCE_DIR}/rapidjson
)


