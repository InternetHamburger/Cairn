cmake_minimum_required(VERSION 4.0)
project(Cairn C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -flto -Wall -Wextra")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -Wall -Wextra")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

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
        src/moveGeneration.h
)