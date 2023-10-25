cmake_minimum_required(VERSION 3.10)

project(mipt-llvm VERSION 1.0)

set(CMAKE_C_STANDARD 17)
set(COMPILE_OPT -O2)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(GAME_LIFE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/GameLife)

add_subdirectory(GameLife)
add_subdirectory(InstrumentationPass)
