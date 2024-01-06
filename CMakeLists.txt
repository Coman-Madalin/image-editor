cmake_minimum_required(VERSION 3.26)
project(Tema3 C)

set(CMAKE_C_STANDARD 23)

add_executable(Tema3 image_editor.c
        structure.h
        main_menu.c
        main_menu.h
        commands.c
        commands.h
        checks.c
        checks.h
        read_utils.c
        read_utils.h
        apply_utils.c
        apply_utils.h
)

target_link_libraries(Tema3 -lm)

