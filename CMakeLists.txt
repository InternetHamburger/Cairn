cmake_minimum_required(VERSION 4.0)
project(Cairn C)

set(CMAKE_C_STANDARD 23)

add_executable(Cairn main.c
        src/board.c
        src/board.h
        src/moveGeneration.c
        src/moveGeneration.h
        src/move.c
        src/move.h
        src/boardUtility.c
        src/boardUtility.h)
