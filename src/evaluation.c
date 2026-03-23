#include "evaluation.h"

#include <stdlib.h>

#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        214, 191, 181, 163, 167, 168, 131, 149,
        -9, 11, 11, 26, 25, 22, 10, -9,
        -32, -16, -20, -16, -8, -11, -13, -31,
        -36, -26, -25, -18, -17, -17, -10, -31,
        -36, -20, -26, -27, -21, -19, 1, -28,
        -41, -23, -29, -47, -34, -7, 3, -32,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -83, 2, -8, 18, 16, -51, -27, -51,
        -19, 1, 26, 41, 31, 47, -1, 3,
        3, 18, 32, 50, 63, 63, 36, 13,
        -2, 6, 28, 33, 25, 46, 19, 23,
        -13, 1, 13, 15, 18, 14, 14, -1,
        -27, -7, -5, 5, 7, 1, 6, -19,
        -36, -32, -11, -8, -5, 1, -18, -10,
        -58, -32, -38, -36, -28, -21, -30, -32,
};

int mg_bishop_table[64] = {
        -22, -2, -2, 1, 0, -43, -4, -20,
        0, 6, 6, 0, 15, 27, 2, -6,
        -3, 6, 18, 27, 34, 46, 32, 9,
        -6, 7, 15, 31, 22, 27, 10, 2,
        -7, 0, 11, 17, 17, 3, 3, -2,
        -5, 3, -1, 3, 0, 1, -1, 0,
        -9, -8, -3, -9, -3, -1, 6, -22,
        -48, -22, -18, -21, -26, -22, -41, -29,
};

int mg_rook_table[64] = {
        32, 30, 29, 43, 46, 35, 30, 46,
        21, 27, 36, 38, 45, 53, 21, 31,
        3, 10, 20, 31, 47, 34, 42, 11,
        -9, -4, 2, 6, 9, 11, 6, -7,
        -26, -21, -19, -10, -10, -15, -10, -23,
        -39, -26, -25, -25, -22, -26, -11, -32,
        -45, -36, -30, -28, -30, -20, -26, -52,
        -28, -23, -18, -16, -13, -21, -29, -36,
};

int mg_queen_table[64] = {
        -5, 20, 28, 34, 39, 38, 33, 28,
        -13, -6, 15, 15, 38, 39, 36, 36,
        -16, -7, 2, 15, 38, 39, 39, 22,
        -18, -9, 1, 6, 19, 20, 8, 8,
        -16, -14, -8, 0, 0, 1, 0, -5,
        -26, -13, -13, -15, -13, -7, -6, -9,
        -31, -22, -11, -16, -15, -7, -16, -25,
        -25, -33, -25, -14, -17, -51, -56, -34,
};

int mg_king_table[64] = {
        4, -139, 47, 16, 23, -218, -144, -118,
        1, 67, 17, 100, -8, 6, 54, -19,
        1, 52, 59, 5, 73, 36, 18, 19,
        -7, 27, 29, 30, 8, 20, 21, -6,
        -20, -1, 5, 4, 20, 6, 9, -8,
        -11, 0, -4, 1, 0, 3, 5, -8,
        15, -4, -5, -10, -6, 2, 7, 7,
        -41, -2, 0, -34, -13, -19, 18, 13,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        212, 211, 195, 184, 178, 187, 208, 201,
        51, 39, 28, 16, 9, 1, 32, 30,
        -1, -11, -20, -27, -33, -32, -21, -15,
        -19, -22, -35, -38, -36, -36, -25, -32,
        -25, -25, -34, -32, -31, -34, -30, -35,
        -19, -19, -23, -29, -27, -23, -21, -33,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -34, -6, -5, 0, 0, -7, -9, -39,
        -16, 0, 7, 18, 14, 9, -9, -10,
        -6, 3, 20, 30, 29, 30, 15, -4,
        -8, 9, 28, 27, 32, 33, 26, 8,
        -19, 3, 22, 27, 29, 27, 16, 0,
        -28, 0, 14, 18, 17, 16, 4, -17,
        -33, -29, -3, 4, 5, 0, -2, -20,
        -54, -54, -19, -10, -11, -13, -30, -52,
};

int eg_bishop_table[64] = {
        6, 10, 2, 6, -4, -2, -21, -13,
        2, 7, 0, -3, 2, -2, -8, -16,
        -3, 6, 4, 6, 8, 15, 6, 0,
        -9, 13, 8, 17, 20, 14, 16, -1,
        -11, 5, 13, 19, 17, 17, 5, -10,
        -11, 3, 11, 13, 14, 4, 6, -7,
        -16, -9, -5, 1, -3, -6, 0, -25,
        -14, -13, -32, -10, -14, -20, -17, -23,
};

int eg_rook_table[64] = {
        14, 14, 15, 15, 16, 15, 15, 10,
        13, 14, 17, 19, 19, 10, 10, 9,
        10, 11, 11, 13, 13, 11, 8, 9,
        8, 11, 9, 7, 9, 9, 4, 3,
        -3, 0, 1, 0, -2, 1, -3, -7,
        -15, -11, -9, -8, -10, -11, -10, -20,
        -21, -18, -15, -16, -17, -16, -19, -27,
        -21, -13, -7, -8, -8, -14, -8, -31,
};

int eg_queen_table[64] = {
        -8, 10, 22, 31, 35, 36, 34, 35,
        -16, 0, 20, 29, 35, 35, 35, 31,
        -23, -5, 9, 27, 35, 36, 36, 32,
        -21, -5, 9, 22, 34, 36, 30, 19,
        -23, -10, -1, 16, 11, 13, 10, 2,
        -27, -15, -5, -10, -7, -4, -7, -11,
        -31, -28, -19, -21, -23, -29, -40, -36,
        -27, -35, -42, -33, -42, -61, -78, -40,
};

int eg_king_table[64] = {
        -68, -17, -18, -6, -14, 1, -3, -63,
        -3, 26, 19, 9, 16, 20, 20, 7,
        3, 25, 26, 31, 28, 35, 28, 8,
        1, 22, 26, 27, 34, 34, 29, 4,
        -10, 8, 15, 20, 22, 20, 11, -3,
        -16, -6, 2, 7, 10, 3, 0, -13,
        -20, -16, -14, -7, -5, -7, -7, -10,
        -35, -36, -31, -33, -43, -33, -16, -28,
};

const int mg_piece_values[] = {0, 102, 300, 318, 447, 821, 0};
const int eg_piece_values[] = {0, 133, 234, 257, 439, 823, 0};

int mg_piece_square_values[BlackKing + 1][64];
int eg_piece_square_values[BlackKing + 1][64];

__attribute__((constructor))
static void init_table(void) {
    for (int i = 0; i < 64; i++)
    {
        mg_piece_square_values[WhitePawn][i] = mg_pawn_table[i] + mg_piece_values[Pawn];
        mg_piece_square_values[WhiteKnight][i] = mg_knight_table[i] + mg_piece_values[Knight];
        mg_piece_square_values[WhiteBishop][i] = mg_bishop_table[i] + mg_piece_values[Bishop];
        mg_piece_square_values[WhiteRook][i] = mg_rook_table[i] + mg_piece_values[Rook];
        mg_piece_square_values[WhiteQueen][i] = mg_queen_table[i] + mg_piece_values[Queen];
        mg_piece_square_values[WhiteKing][i] = mg_king_table[i] + mg_piece_values[King];

        mg_piece_square_values[BlackPawn][i] = -mg_pawn_table[FlipSquare(i)] - mg_piece_values[Pawn];
        mg_piece_square_values[BlackKnight][i] = -mg_knight_table[FlipSquare(i)] - mg_piece_values[Knight];
        mg_piece_square_values[BlackBishop][i] = -mg_bishop_table[FlipSquare(i)] - mg_piece_values[Bishop];
        mg_piece_square_values[BlackRook][i] = -mg_rook_table[FlipSquare(i)] - mg_piece_values[Rook];
        mg_piece_square_values[BlackQueen][i] = -mg_queen_table[FlipSquare(i)] - mg_piece_values[Queen];
        mg_piece_square_values[BlackKing][i] = -mg_king_table[FlipSquare(i)] - mg_piece_values[King];

        eg_piece_square_values[WhitePawn][i] = eg_pawn_table[i] + eg_piece_values[Pawn];
        eg_piece_square_values[WhiteKnight][i] = eg_knight_table[i] + eg_piece_values[Knight];
        eg_piece_square_values[WhiteBishop][i] = eg_bishop_table[i] + eg_piece_values[Bishop];
        eg_piece_square_values[WhiteRook][i] = eg_rook_table[i] + eg_piece_values[Rook];
        eg_piece_square_values[WhiteQueen][i] = eg_queen_table[i] + eg_piece_values[Queen];
        eg_piece_square_values[WhiteKing][i] = eg_king_table[i] + eg_piece_values[King];

        eg_piece_square_values[BlackPawn][i] = -eg_pawn_table[FlipSquare(i)] - eg_piece_values[Pawn];
        eg_piece_square_values[BlackKnight][i] = -eg_knight_table[FlipSquare(i)] - eg_piece_values[Knight];
        eg_piece_square_values[BlackBishop][i] = -eg_bishop_table[FlipSquare(i)] - eg_piece_values[Bishop];
        eg_piece_square_values[BlackRook][i] = -eg_rook_table[FlipSquare(i)] - eg_piece_values[Rook];
        eg_piece_square_values[BlackQueen][i] = -eg_queen_table[FlipSquare(i)] - eg_piece_values[Queen];
        eg_piece_square_values[BlackKing][i] = -eg_king_table[FlipSquare(i)] - eg_piece_values[King];
    }
}

const int piece_game_phase[] = {0, 0, 1, 1, 2, 4, 0};

int eval(const Board *board) {
    uint64_t occupied = GetOccupied(board);

    int mg_eval = 0;
    int eg_eval = 0;
    int phase = 0;
    while (occupied){
        const int square = poplsb(&occupied);
        mg_eval += mg_piece_square_values[board->squares[square]][square];
        eg_eval += eg_piece_square_values[board->squares[square]][square];
        phase += piece_game_phase[GetType(board->squares[square])];
    }

    // Interpolate based on number of pieces
    phase = __min(24, phase);
    int eval = (mg_eval * phase + eg_eval * (24 - phase)) / 24;

    return eval * (board->white_to_move ? 1 : -1);
}
