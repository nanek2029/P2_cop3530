cmake_minimum_required(VERSION 3.20)
project(P2_cop3530)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(P2_cop3530
        src/main.cpp
        src/SearchAlgorithms.cpp
        src/SearchAlgorithms.h)

target_include_directories(P2_cop3530 PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/bridges
        ${CMAKE_SOURCE_DIR}/src/bridges/data_src
        ${CMAKE_SOURCE_DIR}/rapidjson
)


