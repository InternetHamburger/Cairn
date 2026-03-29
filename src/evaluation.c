#include "evaluation.h"

#include <stdlib.h>

#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        174, 158, 136, 127, 118, 116, 103, 104,
        -6, 12, 4, 25, 16, 18, 10, -9,
        -24, -9, -17, -12, -5, -10, -12, -28,
        -29, -21, -21, -14, -14, -17, -10, -30,
        -32, -16, -24, -25, -18, -17, 0, -28,
        -36, -18, -26, -44, -32, -5, 4, -34,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -100, -20, -12, 9, 16, -58, -22, -86,
        -26, 1, 21, 36, 26, 36, -6, -9,
        -2, 17, 40, 50, 61, 64, 34, 10,
        4, 8, 33, 40, 34, 53, 24, 26,
        -14, 6, 17, 21, 22, 20, 18, 0,
        -28, -6, -4, 12, 12, 6, 8, -21,
        -41, -38, -16, -5, -5, 0, -14, -13,
        -58, -37, -34, -30, -26, -20, -33, -33,
};

int mg_bishop_table[64] = {
        -26, -1, -1, -2, 2, -34, -37, -36,
        -10, 2, 5, 6, 14, 10, 1, -14,
        -5, 8, 20, 26, 37, 50, 30, 13,
        -8, 10, 21, 36, 28, 33, 14, 1,
        -10, 0, 16, 22, 21, 5, 4, -4,
        -8, 4, 0, 8, 0, 4, -2, -2,
        -4, -10, -2, -12, -2, -5, 2, -25,
        -46, -18, -25, -22, -28, -29, -30, -22,
};

int mg_rook_table[64] = {
        26, 29, 29, 37, 42, 25, 14, 41,
        17, 26, 37, 42, 45, 48, 13, 30,
        4, 14, 21, 32, 38, 30, 41, 14,
        -5, -2, 4, 9, 16, 14, 5, -6,
        -22, -17, -14, -5, -9, -14, -5, -22,
        -41, -28, -25, -26, -22, -22, -12, -37,
        -42, -37, -26, -26, -28, -14, -25, -58,
        -26, -21, -16, -12, -9, -14, -29, -40,
};

int mg_queen_table[64] = {
        -9, 10, 26, 36, 44, 40, 13, -4,
        -14, -9, 12, 14, 30, 41, 13, 17,
        -14, -6, 8, 20, 33, 53, 40, 24,
        -17, -10, 4, 14, 20, 24, 16, 10,
        -17, -14, -4, 2, 1, 4, 4, -5,
        -26, -10, -9, -10, -8, -5, -2, -13,
        -32, -24, -8, -13, -13, -6, -18, -30,
        -21, -34, -25, -14, -22, -46, -45, -34,
};

int mg_king_table[64] = {
        32, 0, -68, 61, 0, -113, -152, -26,
        -30, 56, 50, 124, 0, -81, -28, -20,
        22, 74, 73, 49, 32, 42, 44, 4,
        2, 32, 9, 54, 22, 25, 14, -12,
        -28, -8, 0, 4, 17, 5, 6, -20,
        -26, -8, -9, -8, -5, -2, 1, -16,
        9, -10, -10, -18, -17, -1, 4, 1,
        -61, -8, -4, -37, -16, -26, 16, 8,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        150, 156, 151, 140, 133, 138, 143, 139,
        37, 30, 21, 16, 9, 0, 20, 16,
        -2, -8, -17, -20, -26, -28, -17, -14,
        -17, -17, -29, -29, -30, -29, -18, -26,
        -24, -20, -28, -26, -26, -28, -21, -29,
        -17, -14, -18, -28, -21, -16, -12, -29,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -56, -9, 0, -2, -1, -6, -17, -34,
        -17, -1, 12, 20, 10, 12, -2, -6,
        -13, 6, 24, 28, 28, 28, 14, 0,
        -5, 10, 26, 28, 34, 33, 25, 12,
        -10, 5, 21, 28, 26, 25, 17, 0,
        -28, 0, 14, 17, 17, 14, 10, -12,
        -36, -20, -2, 4, 4, 2, -9, -16,
        -65, -48, -26, -16, -12, -10, -29, -54,
};

int eg_bishop_table[64] = {
        -2, 2, -5, 4, 0, -8, -18, -24,
        -1, 5, 0, -1, 6, 2, -8, -18,
        -4, 9, 6, 9, 10, 24, 10, 0,
        -6, 13, 12, 21, 21, 16, 16, -5,
        -16, 4, 16, 21, 17, 17, 5, -8,
        -12, 2, 9, 14, 14, 6, 4, -5,
        -16, -8, -2, 0, 0, -4, 0, -18,
        -20, -20, -30, -13, -14, -18, -22, -30,
};

int eg_rook_table[64] = {
        10, 16, 14, 18, 22, 20, 17, 16,
        14, 17, 20, 22, 21, 17, 14, 13,
        10, 12, 16, 14, 16, 16, 14, 8,
        4, 9, 9, 9, 10, 12, 6, 1,
        -8, -2, 0, 0, 0, 1, -2, -13,
        -21, -13, -13, -12, -13, -13, -12, -24,
        -28, -25, -17, -21, -21, -17, -20, -33,
        -22, -14, -8, -8, -8, -10, -13, -32,
};

int eg_queen_table[64] = {
        -4, 10, 21, 25, 30, 37, 21, 29,
        -14, 1, 16, 21, 30, 42, 36, 29,
        -17, -5, 10, 25, 38, 46, 38, 26,
        -21, -2, 12, 20, 36, 37, 30, 18,
        -18, -13, 0, 14, 10, 13, 9, 5,
        -30, -13, -2, -9, -5, -2, -4, -9,
        -36, -25, -16, -17, -22, -21, -40, -28,
        -30, -33, -33, -28, -42, -52, -74, -33,
};

int eg_king_table[64] = {
        -44, -6, -13, -10, -13, 4, 0, -44,
        -9, 10, 6, 5, 6, 12, 14, 1,
        0, 18, 17, 20, 24, 26, 22, 6,
        0, 17, 20, 22, 29, 28, 25, 1,
        -12, 6, 12, 18, 21, 17, 10, -8,
        -13, -2, 0, 6, 9, 2, -1, -12,
        -9, -10, -10, -8, -6, -6, -4, -9,
        -36, -29, -24, -34, -36, -32, -12, -25,
};

const int mg_piece_values[] = {0, 100, 280, 302, 427, 802, 0};
const int eg_piece_values[] = {0, 120, 238, 261, 423, 788, 0};

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
