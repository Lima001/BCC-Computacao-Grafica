cmake_minimum_required(VERSION 3.7)

project(ComputacaoGrafica)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(dda.out dda.cpp)
add_executable(dda_and_rotation.out dda_and_rotation.cpp)

target_link_libraries(dda.out ${SDL2_LIBRARIES})
target_link_libraries(dda_and_rotation.out ${SDL2_LIBRARIES})