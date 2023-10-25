project(gamelife VERSION 1.0)

set(SFML_LIB -lcsfml-graphics -lcsfml-window -lcsfml-system)

add_library(graphic STATIC graphic_lib.c)
target_link_libraries(graphic PUBLIC ${SFML_LIB})

add_executable(game_life
    game_life.c
    cell_field.c
)

add_compile_options(game_life ${COMPILE_OPT})

target_link_libraries(game_life graphic)
