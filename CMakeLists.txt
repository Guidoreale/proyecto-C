project(proyecto C)
set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(proyecto colacp.c planificador.c)
target_link_libraries(proyecto m)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -lm")