cmake_minimum_required(VERSION 3.10)

project(gamelife VERSION 1.0)

set(CMAKE_C_STANDARD 17)
set(COMPILE_OPT -O2)
set(SFML_LIB -lcsfml-graphics -lcsfml-window -lcsfml-system)

add_library(libgraphic STATIC graphic_lib.c)
target_link_libraries(libgraphic PUBLIC ${SFML_LIB})

add_executable(game_life
    game_life.c
    cell_field.c
)

add_compile_options(game_life ${COMPILE_OPT})

target_link_libraries(game_life libgraphic)
