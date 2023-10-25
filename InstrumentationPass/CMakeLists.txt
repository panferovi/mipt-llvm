project(instrumentation_pass VERSION 1.0)

set(SFML_LIB -lcsfml-graphics -lcsfml-window -lcsfml-system)

set(PASS_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(PASS_SOURCES app_route_pass.cpp)
set(PASS_TARGETS
    ${GAME_LIFE_DIR}/game_life.c
    ${GAME_LIFE_DIR}/cell_field.c
    ${GAME_LIFE_DIR}/graphic_lib.c
)

add_library(routepass SHARED ${PASS_SOURCES})

add_custom_target(app_route_pass
    COMMAND clang++ -c ${CMAKE_CURRENT_SOURCE_DIR}/route_method.cpp -std=c++17
    COMMAND clang -Xclang -load -Xclang ${PASS_BIN_DIR}/libroutepass.so ${CMAKE_CURRENT_BINARY_DIR}/route_method.o
            ${COMPILE_OPT} -flegacy-pass-manager ${PASS_TARGETS} ${SFML_LIB} -o app_route_pass -lstdc++
    DEPENDS ${PASS_SOURCES} ${PASS_TARGETS} routepass
    VERBATIM
)
