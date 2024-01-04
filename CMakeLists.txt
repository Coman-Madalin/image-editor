cmake_minimum_required(VERSION 3.26)
project(Tema3 C)

set(CMAKE_C_STANDARD 23)

add_executable(Tema3 main.c
        read_utils.c
        read_utils.h
        options_header.c
        options_header.h
        structure_header.h
        options_utils.c
        options_utils.h
        main_menu.c
        main_menu.h)

target_link_libraries(Tema3 -lm)

