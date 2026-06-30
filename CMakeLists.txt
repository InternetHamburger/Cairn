cmake_minimum_required(VERSION 4.0)
set(CMAKE_C_STANDARD 23)
project(Cairn C)

include(CheckCCompilerFlag)
include(CheckCSourceRuns)

check_c_compiler_flag("-mavx512f" COMPILER_SUPPORTS_AVX512)

if(COMPILER_SUPPORTS_AVX512)
    check_c_source_runs("
        int main() {
            __builtin_cpu_init();
            return !(__builtin_cpu_supports(\"avx512f\")  &&
                     __builtin_cpu_supports(\"avx512bw\") &&
                     __builtin_cpu_supports(\"avx512vl\") &&
                     __builtin_cpu_supports(\"avx512dq\"));
        }
    " HAS_AVX512)
endif()

add_compile_options(-mavx2)
if(HAS_AVX512)
    add_compile_options(-mavx512f)
    message("avx512 yeaaaaaah")
endif()

set(NETWORK_NAME "net13")
set(NETWORK "C:/c/${NETWORK_NAME}.bin")

set(CMAKE_AR "gcc-ar")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG -flto -Wall -Wextra -Wl,--stack,16777216")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -Wall -Wextra -Wl,--stack,16777216")
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
        src/moveOrderer.c
        src/transposition.c
        src/movepicker.c
        src/nnue.c
)

target_compile_definitions(Cairn PRIVATE NETWORK="${NETWORK}")