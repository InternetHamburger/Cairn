cmake_minimum_required(VERSION 4.0)
project(Cairn C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -flto")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_executable(Cairn
        main.c
        src/board.c
        src/moveGeneration.c
        src/move.c
        src/utility.c
        src/preComputedData.c
        src/perft.c
        src/uci.c
        src/search.c
        src/evaluation.c
        src/datagen.c
        src/zobrist.c
)