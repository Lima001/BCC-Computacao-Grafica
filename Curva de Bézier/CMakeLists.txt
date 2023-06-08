cmake_minimum_required(VERSION 3.7)

project(ComputacaoGrafica)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(run cubic_bezier_curve.cpp)
target_link_libraries(run ${SDL2_LIBRARIES})