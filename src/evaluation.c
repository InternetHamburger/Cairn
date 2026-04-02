#include "evaluation.h"

#include <stdlib.h>

#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        206, 170, 130, 115, 109, 102, 76, 54,
        -40, -9, -5, 26, 20, 33, 6, -29,
        -52, -21, -30, -17, -1, -10, -21, -50,
        -53, -35, -30, -20, -16, -20, -15, -45,
        -54, -29, -33, -35, -27, -23, 10, -38,
        -63, -34, -41, -64, -47, -6, 13, -47,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -150, -54, -56, -11, 58, -123, 12, -129,
        -33, -15, 40, 61, 57, 81, -15, -20,
        -4, 30, 50, 78, 103, 120, 53, 26,
        -6, 9, 45, 50, 42, 77, 31, 41,
        -20, 2, 15, 18, 24, 22, 25, -2,
        -34, -13, -8, 13, 11, 9, 14, -28,
        -58, -53, -21, -13, -9, 2, -12, -12,
        -66, -44, -43, -39, -29, -23, -35, -38,
};

int mg_bishop_table[64] = {
        -59, -52, -59, -39, -2, -66, 2, -29,
        -33, -5, 11, 19, 17, 24, 8, -15,
        -10, 6, 27, 45, 63, 86, 46, 23,
        -12, 13, 33, 56, 36, 50, 17, 6,
        -12, -1, 21, 29, 30, 6, 6, -3,
        -6, 12, 0, 12, 0, 10, 1, 3,
        -2, -8, 3, -18, -3, 0, 12, -25,
        -66, -20, -24, -30, -34, -34, -41, -27,
};

int mg_rook_table[64] = {
        63, 49, 59, 88, 80, 36, 42, 81,
        12, 22, 49, 84, 77, 87, 29, 42,
        -12, 10, 25, 58, 67, 63, 79, 18,
        -22, -20, -2, 10, 26, 21, 12, -9,
        -45, -45, -31, -15, -17, -26, -12, -35,
        -65, -51, -43, -53, -33, -34, -11, -49,
        -64, -60, -41, -40, -41, -19, -25, -85,
        -42, -36, -29, -24, -17, -27, -50, -55,
};

int mg_queen_table[64] = {
        -12, 27, 47, 82, 95, 116, 47, 24,
        -33, -29, 1, 9, 34, 59, 13, 21,
        -22, -19, -9, 12, 42, 77, 55, 23,
        -26, -23, -13, 2, 8, 14, 7, 6,
        -21, -32, -15, -9, -10, -8, 0, -15,
        -32, -15, -20, -17, -18, -11, -9, -25,
        -39, -30, -8, -13, -15, -2, -10, -27,
        -21, -45, -27, -12, -15, -47, -27, -30,
};

int mg_king_table[64] = {
        155, 55, -40, 21, 21, 1, 103, 44,
        103, 87, 31, 28, 7, 1, 84, 37,
        33, -11, -20, -30, -25, 2, 16, 38,
        -7, -6, -33, -55, -38, -24, 5, -36,
        -63, -32, -52, -58, -38, -41, -28, -53,
        -20, -12, -41, -53, -56, -38, -7, -29,
        45, -1, -13, -35, -32, 10, 23, 32,
        -38, 21, 16, -48, 8, -21, 56, 53,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        60, 81, 85, 73, 53, 66, 81, 87,
        62, 45, 32, 14, 5, -10, 19, 25,
        12, -7, -16, -26, -38, -42, -23, -12,
        -6, -14, -33, -37, -40, -36, -26, -28,
        -15, -19, -33, -30, -30, -35, -36, -33,
        -4, -9, -18, -23, -24, -26, -31, -32,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -46, -2, 12, 5, -13, 25, -24, -37,
        -17, 3, 2, 14, 14, -6, 4, -3,
        0, 3, 28, 22, 12, 13, 11, -5,
        0, 16, 31, 32, 43, 29, 30, -3,
        -12, 11, 34, 37, 34, 38, 16, -8,
        -27, 1, 23, 26, 25, 16, 6, -18,
        -23, -23, -2, 10, 8, -6, -12, -21,
        -66, -62, -28, -17, -15, -15, -39, -89,
};

int eg_bishop_table[64] = {
        7, 19, 4, 12, 4, 0, -23, -29,
        12, 17, 0, 0, 9, 5, -8, -17,
        -2, 20, 10, 7, 3, 11, 7, -8,
        -7, 21, 14, 18, 25, 13, 21, -10,
        -10, 12, 22, 23, 22, 22, 11, -10,
        -10, 2, 15, 17, 24, 5, 3, -13,
        -33, -16, -8, 6, -1, -10, -11, -35,
        -11, -27, -44, -9, -13, -24, -24, -35,
};

int eg_rook_table[64] = {
        -7, 6, 9, 4, 7, 20, 15, 1,
        17, 18, 14, 5, 5, 0, 13, 9,
        19, 15, 16, 6, 5, 4, 0, 7,
        15, 22, 21, 15, 7, 10, 2, 5,
        2, 8, 10, 4, 3, 7, -1, -10,
        -14, -8, -8, 0, -12, -10, -20, -24,
        -27, -13, -16, -19, -19, -24, -31, -21,
        -16, -11, -1, 0, -9, -9, 0, -22,
};

int eg_queen_table[64] = {
        0, -1, 2, -4, -15, -17, -1, 30,
        16, 35, 45, 55, 50, 36, 28, 19,
        -13, 18, 46, 55, 55, 30, 38, 28,
        -5, 16, 47, 57, 70, 71, 68, 23,
        -25, 9, 14, 46, 35, 40, 20, 24,
        -38, -30, 1, -6, -2, 3, -9, -1,
        -42, -40, -41, -41, -27, -60, -72, -68,
        -70, -42, -71, -60, -96, -62, -111, -63,
};

int eg_king_table[64] = {
        -89, -22, 1, -20, -19, -3, -26, -75,
        -35, -7, 4, -2, 9, 17, 10, -3,
        -12, 27, 28, 33, 39, 40, 35, 9,
        -2, 28, 34, 41, 48, 48, 36, 16,
        -2, 19, 32, 41, 41, 35, 27, 6,
        -13, 0, 14, 28, 32, 20, 2, -6,
        -34, -14, -9, 3, 4, -10, -14, -30,
        -41, -48, -33, -30, -57, -38, -46, -69,
};

const int mg_piece_values[] = {0, 110, 345, 369, 485, 1078, 0};
const int eg_piece_values[] = {0, 146, 261, 289, 503, 858, 0};

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
