#include "evaluation.h"

#include <stdlib.h>

#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        220, 175, 140, 126, 109, 123, 63, 64,
        -40, -3, -7, 25, 22, 23, 6, -30,
        -54, -23, -31, -18, -7, -9, -22, -49,
        -56, -38, -32, -22, -17, -21, -14, -47,
        -57, -28, -36, -38, -26, -23, 7, -40,
        -65, -36, -43, -65, -49, -8, 12, -47,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -143, -26, -74, 0, 56, -113, -2, -116,
        -40, -10, 34, 65, 46, 77, -11, -15,
        -20, 27, 49, 80, 103, 111, 49, 14,
        2, 10, 44, 48, 39, 75, 29, 39,
        -18, 6, 15, 17, 25, 23, 26, 0,
        -35, -12, -9, 12, 13, 9, 14, -25,
        -53, -52, -20, -11, -9, 5, -14, -12,
        -48, -44, -43, -42, -35, -22, -38, -50,
};

int mg_bishop_table[64] = {
        -59, -56, -38, -8, 8, -27, -37, -27,
        -20, -6, 7, 22, 13, 16, 1, -9,
        -9, 5, 25, 43, 59, 76, 49, 21,
        -15, 13, 30, 57, 36, 50, 17, 4,
        -7, 0, 18, 27, 30, 5, 10, -5,
        -11, 10, -1, 11, -4, 11, 0, 0,
        0, -11, 0, -19, -1, 1, 13, -28,
        -69, -20, -26, -33, -37, -35, -49, -24,
};

int mg_rook_table[64] = {
        69, 59, 47, 72, 80, 41, 49, 72,
        9, 23, 51, 82, 76, 88, 20, 49,
        -9, 10, 24, 57, 64, 62, 66, 18,
        -25, -18, -2, 16, 23, 26, 9, -11,
        -45, -45, -32, -16, -15, -23, -18, -34,
        -65, -44, -43, -49, -30, -34, -13, -48,
        -62, -57, -38, -36, -38, -20, -25, -87,
        -40, -36, -28, -24, -15, -26, -51, -53,
};

int mg_queen_table[64] = {
        -17, 15, 58, 58, 75, 68, 31, -4,
        -29, -26, 7, 17, 28, 42, 9, 13,
        -18, -13, -6, 24, 46, 75, 42, 25,
        -22, -20, -4, 6, 10, 13, 9, 11,
        -17, -24, -13, -5, -4, -5, 2, -14,
        -34, -8, -17, -12, -14, -7, -4, -21,
        -43, -27, -7, -10, -10, 5, -3, -29,
        -16, -40, -24, -11, -11, -46, -26, -20,
};

int mg_king_table[64] = {
        74, 50, -3, 106, 87, 23, 125, 86,
        89, 88, 40, 24, 14, 23, 68, 64,
        62, 0, -28, -32, -42, 13, 31, 38,
        -26, 0, -44, -64, -44, -13, -7, -49,
        -56, -30, -55, -60, -39, -41, -27, -52,
        -30, -15, -52, -62, -61, -43, -15, -36,
        30, -9, -14, -44, -43, 1, 14, 26,
        -49, 7, 5, -52, -2, -29, 43, 41,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        55, 73, 80, 67, 50, 60, 74, 76,
        59, 43, 34, 16, 8, -10, 18, 26,
        14, -5, -15, -25, -37, -38, -22, -11,
        -4, -12, -32, -35, -38, -37, -24, -26,
        -11, -17, -31, -28, -29, -33, -35, -31,
        -4, -8, -13, -25, -22, -25, -29, -32,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -47, -14, 21, 2, -10, 13, -24, -49,
        -8, 2, 4, 5, 13, -9, 3, -8,
        0, 4, 28, 20, 10, 12, 15, -2,
        2, 19, 30, 34, 44, 25, 30, -6,
        -14, 13, 36, 39, 34, 35, 18, -3,
        -25, 3, 24, 28, 26, 15, 7, -16,
        -29, -19, -3, 10, 8, -6, -8, -19,
        -90, -56, -25, -18, -12, -16, -31, -68,
};

int eg_bishop_table[64] = {
        9, 20, 2, 7, -3, -1, -16, -22,
        1, 13, 3, -6, 8, 4, -8, -24,
        -1, 18, 13, 7, 7, 16, 5, -6,
        -2, 21, 14, 19, 26, 10, 19, -9,
        -16, 13, 23, 25, 20, 24, 11, -8,
        -10, 2, 18, 17, 24, 6, 3, -13,
        -27, -15, -10, 5, 1, -11, -10, -29,
        -14, -24, -42, -10, -15, -23, -23, -36,
};

int eg_rook_table[64] = {
        -8, 5, 10, 8, 8, 20, 12, 1,
        18, 21, 12, 4, 5, -2, 19, 3,
        21, 17, 17, 5, 3, 4, 4, 10,
        17, 22, 19, 11, 7, 11, 4, 4,
        1, 10, 11, 8, 2, 8, 0, -6,
        -12, -10, -6, -3, -13, -10, -22, -23,
        -27, -17, -17, -23, -23, -23, -28, -21,
        -15, -11, -3, -4, -9, -11, 2, -23,
};

int eg_queen_table[64] = {
        8, 2, -1, 7, -3, 13, 1, 31,
        6, 28, 41, 32, 40, 48, 32, 16,
        -12, 16, 45, 37, 50, 29, 32, 26,
        -2, 20, 43, 49, 69, 66, 55, 23,
        -22, 13, 20, 42, 37, 40, 20, 14,
        -35, -29, 4, -9, 2, 3, -10, -8,
        -46, -31, -33, -37, -32, -71, -69, -51,
        -66, -43, -61, -61, -91, -70, -111, -61,
};

int eg_king_table[64] = {
        -77, -19, -18, -37, -29, -7, -22, -73,
        -27, -6, 2, 0, 12, 10, 15, -11,
        -15, 23, 28, 38, 41, 36, 32, 3,
        5, 28, 37, 42, 49, 46, 40, 13,
        1, 19, 31, 43, 43, 37, 25, 1,
        -12, 0, 18, 29, 32, 20, 5, -6,
        -33, -15, -5, 5, 7, -9, -13, -29,
        -39, -44, -35, -29, -53, -39, -45, -70,
};

const int mg_piece_values[] = {0, 112, 352, 377, 499, 1096, 0};
const int eg_piece_values[] = {0, 145, 266, 292, 509, 867, 0};

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
