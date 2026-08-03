#ifndef CAIRN_STRUCTS_H
#define CAIRN_STRUCTS_H

#include <stdint.h>

#define INPUT_SIZE 768
#define HL_SIZE 1024
#define NUM_INPUT_BUCKETS 4
#define NUM_OUTPUT_BUCKETS 8

static const int BUCKET_LAYOUT[] = {
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    0, 0, 1, 1, 1, 1, 0, 0,
};

#define BUCKET_DIVISOR (32 / NUM_OUTPUT_BUCKETS)

#define QA 255
#define QB 64

#define EVAL_SCALE 400

#if (__AVX512F__)
    #define FULL_VECTOR_SIZE 64
    #define HALF_VECTOR_SIZE 32
#elif (__AVX2__)
    #define FULL_VECTOR_SIZE 32
    #define HALF_VECTOR_SIZE 16
#endif

#define MAX_NUM_PLY 17697
#define NEG_INF (-32767)
#define CHECKMATE (-32000)

typedef int16_t vfsi16 __attribute__ ((vector_size (FULL_VECTOR_SIZE)));
typedef int16_t vhsi16 __attribute__ ((vector_size (HALF_VECTOR_SIZE)));
typedef int32_t vfsi32 __attribute__ ((vector_size (FULL_VECTOR_SIZE)));

typedef struct{
    alignas(64) int16_t feature_weights[NUM_INPUT_BUCKETS][INPUT_SIZE][HL_SIZE];
    alignas(64) int16_t feature_bias[HL_SIZE];
    alignas(64) int16_t out_weights[NUM_OUTPUT_BUCKETS][2 * HL_SIZE];
    alignas(64) int16_t out_bias[NUM_OUTPUT_BUCKETS];
} Parameters;

typedef struct{
    alignas(64) int16_t white_accumulator[HL_SIZE];
    alignas(64) int16_t black_accumulator[HL_SIZE];
} nnue_t;

typedef enum{
    none = 0b0000,
    Castle = 0b0001,
    EnPassant = 0b0010,
    DoublePush = 0b0011,
    PromoteQueen = 0b0100,
    PromoteKnight = 0b0101,
    PromoteRook = 0b0110,
    PromoteBishop = 0b0111,
} MoveFlag;

typedef struct{
    uint16_t value; // Format ffffsssssstttttt, f - flag, s - start square, t - target square
} Move;

enum{
    None = 0b0000,
    WhitePawn = 0b0001,
    WhiteKnight = 0b0010,
    WhiteBishop = 0b0011,
    WhiteRook = 0b0100,
    WhiteQueen = 0b0101,
    WhiteKing = 0b0110,
    BlackPawn = 0b1001,
    BlackKnight = 0b1010,
    BlackBishop = 0b1011,
    BlackRook = 0b1100,
    BlackQueen = 0b1101,
    BlackKing = 0b1110,
};

typedef uint8_t Piece;

enum{
    Pawn = 0b0001,
    Knight = 0b0010,
    Bishop = 0b0011,
    Rook = 0b0100,
    Queen = 0b0101,
    King = 0b0110,
};

typedef uint8_t PieceType;

typedef struct{
    Piece squares[64];
    bool white_to_move;
    bool white_kingside;
    bool white_queenside;
    bool black_kingside;
    bool black_queenside;
    int en_passant_square;
    int white_king_square;
    int black_king_square;
    uint64_t piece_bbs[King + 1];
    uint64_t color_bbs[2];
    uint64_t zobrist_hash;
    uint64_t pawn_key;
    uint64_t minor_key;
    uint64_t non_pawn_key[2];
    uint64_t pins;
    uint64_t threat_bb;
    int fifty_move_counter;
    int game_ply;
} Board;

typedef struct{
    Move tt_move, killer;
    Move moves[218];
    int stage;
    int move_num;
    int scores[218];
} MovePicker;

typedef struct{
    int static_eval, to_square;
    Piece moved_piece;
    MovePicker mp;
    Move excluded;
    Board board;
} Stack;

typedef struct{
    nnue_t nnue_stack[256];
    bool dirty[256];
    Move move_stack[256];
}nnue_stack_t;

typedef struct{
    uint64_t hash;
    Move best_move;
    int16_t score;
    uint8_t depth_node_type; // ttdddddd  t - type (0-exact, 1-upper bound, 2-lower bound)  d - depth
} Entry;

typedef struct{
    int num_entries;
    Entry* entries;
} TT;

typedef struct {
    uint64_t nodes;
    double start_time;
    uint64_t time_limit;
    uint64_t soft_time_limit;
    uint64_t node_limit;
    int depth_limit;
    uint64_t soft_node_limit;
    bool print_info;
    uint64_t hashes[MAX_NUM_PLY]; // For threefold repetition
    int seldepth;
    nnue_t nnue;

    Board board;
    Stack ss[512];
    nnue_stack_t nnue_stack;

    int quiet_history[BlackKing + 1][64];
    int capture_history[BlackKing + 1][64][BlackKing + 1];
    int cont_hist[BlackKing + 1][64][BlackKing + 1][64];
    Move killer_moves[256];

    int16_t pawn_corr_hist[2][16384];
    int16_t minor_corr_hist[2][16384];
    int16_t non_pawn_corr_hist[2][2][16384];
    int16_t cont_corr_hist[BlackKing + 1][64][BlackKing + 1][64];

    TT tt;
} Thread;

typedef struct {
    int length, score;
    Move line[256];
} PVariation;

typedef struct{
    Move best_move;
    int score;
    uint64_t nodes;
    int depth;
} SearchResult;

#endif //CAIRN_STRUCTS_H
