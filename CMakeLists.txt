cmake_minimum_required(VERSION 4.0)
set(CMAKE_C_STANDARD 23)
project(Cairn C)

include(CheckCCompilerFlag)
include(CheckCSourceCompiles)

# --- Detect AVX2 ---
check_c_compiler_flag("-mavx2" COMPILER_SUPPORTS_AVX2)

if(COMPILER_SUPPORTS_AVX2)
    set(CMAKE_REQUIRED_FLAGS "-mavx2")
    check_c_source_compiles("
        #include <immintrin.h>
        int main() {
            __m256i a = _mm256_setzero_si256();
            (void)a;
            return 0;
        }
    " HAS_AVX2)
    unset(CMAKE_REQUIRED_FLAGS)
endif()

# --- Detect AVX-512 (F + common subsets) ---
check_c_compiler_flag("-mavx512f" COMPILER_SUPPORTS_AVX512)

if(COMPILER_SUPPORTS_AVX512)
    set(CMAKE_REQUIRED_FLAGS "-mavx512f -mavx512bw -mavx512vl -mavx512dq")
    check_c_source_compiles("
        #include <immintrin.h>
        int main() {
            __m512i a = _mm512_setzero_si512();
            (void)a;
            return 0;
        }
    " HAS_AVX512)
    unset(CMAKE_REQUIRED_FLAGS)
endif()

if (HAS_AVX2)
    add_compile_options(-mavx2)
    message("avx2 whoooooooooo")
endif()
if(HAS_AVX512)
    add_compile_options(-mavx512f)
    message("avx512 yeaaaaaah")
endif()

set(NETWORK_NAME "net10")
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