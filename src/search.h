#ifndef CAIRN_SEARCH_H
#define CAIRN_SEARCH_H

#include "board.h"
#include "transposition.h"

#define NEG_INF (-32767)
#define CHECKMATE (-32000)

typedef struct{
    int static_eval, to_square;
    Piece moved_piece;
} Stack;

typedef struct {
    uint64_t nodes;
    double start_time;
    uint64_t time_limit;
    uint64_t node_limit;
    int depth_limit;
    uint64_t soft_node_limit;
    bool print_info;
    uint64_t hashes[MAX_NUM_PLY]; // For threefold repetition

    Board board;
    Stack ss[512];

    int quiet_history[BlackKing + 1][64];
    int capture_history[BlackKing + 1][64][BlackKing + 1];
    int cont_hist[BlackKing + 1][64][BlackKing + 1][64];
    Move killer_moves[256];

    int pawn_corr_hist[2][16384];
    int minor_corr_hist[2][16384];
    int non_pawn_corr_hist[2][2][16384];

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

int correct_eval(Thread* thread, int eval);
SearchResult search(Thread *thread);

#endif //CAIRN_SEARCH_H