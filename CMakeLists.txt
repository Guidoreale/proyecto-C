cmake_minimum_required(VERSION 3.23)
project(proyecto_orga C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(proyecto_orga
        main.c)
target_link_libraries(proyecto_orga m)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -lm")


