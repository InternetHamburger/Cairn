cmake_minimum_required(VERSION 4.0)
project(Cairn C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_CXX_FLAGS_RELEASE -flto)

add_executable(Cairn main.c
        src/board.c
        src/board.h
        src/moveGeneration.c
        src/moveGeneration.h
        src/move.c
        src/move.h
        src/utility.c
        src/utility.h
        src/preComputedData.c
        src/preComputedData.h
        src/perft.c
        src/perft.h
        src/uci.c
        src/uci.h
        src/uci.c
        src/search.c
        src/search.h
        src/evaluation.c
        src/evaluation.h
        src/datagen.c
        src/datagen.h
        src/zobrist.c
        src/zobrist.h
)