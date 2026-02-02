#include "evaluation.h"

#include <stdlib.h>

#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        263, 214, 165, 196, 206, 120, 88, 96,
        -52, -12, -2, 24, 30, 21, -6, -48,
        -63, -38, -42, -26, -15, -20, -19, -64,
        -63, -45, -38, -23, -27, -27, -29, -53,
        -58, -41, -40, -43, -33, -31, -8, -52,
        -71, -45, -58, -68, -54, -26, -13, -56,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -160, -103, -81, -25, 14, -115, -76, -114,
        -23, -2, 42, 77, 53, 80, -12, 1,
        -4, 34, 46, 89, 121, 100, 61, 10,
        0, 7, 38, 39, 30, 72, 21, 22,
        -10, -2, 14, 12, 20, 12, 23, 6,
        -18, -1, -3, 13, 13, 7, 17, -12,
        -32, -17, -14, -8, -1, 14, -17, -3,
        -55, -32, -30, -38, -23, -20, -30, -23,
};

int mg_bishop_table[64] = {
        -64, -65, -76, -51, -18, -74, -17, -17,
        -31, -13, 6, -4, 32, 36, 7, -14,
        1, 20, 34, 53, 63, 105, 38, 19,
        1, 6, 24, 49, 33, 35, 9, 6,
        -18, 16, 14, 30, 31, 8, 6, 5,
        0, 5, -4, 11, 2, 6, 4, 2,
        -14, -1, 0, -10, 0, 4, 14, -13,
        -66, -21, -7, -15, -22, -15, -45, -45,
};

int mg_rook_table[64] = {
        80, 59, 58, 62, 61, 22, 28, 81,
        20, 25, 68, 72, 63, 81, 21, 61,
        0, 12, 13, 61, 92, 73, 62, 15,
        -25, -25, -1, 4, 18, 3, 0, -1,
        -51, -40, -41, -14, -31, -23, -23, -31,
        -59, -38, -39, -41, -40, -30, -16, -30,
        -61, -58, -45, -35, -41, -26, -35, -68,
        -31, -38, -32, -23, -21, -24, -45, -31,
};

int mg_queen_table[64] = {
        -33, -8, -9, 32, 66, 42, 42, 46,
        -22, -32, -9, -3, 1, 60, 42, 64,
        -25, -20, -9, 18, 26, 56, 44, 11,
        -30, -33, -12, -7, -10, -2, -10, -3,
        -12, -28, -14, -12, -11, -18, -6, -18,
        -7, -12, -22, -17, -10, -15, 4, -2,
        -27, -6, -1, -6, -8, 9, 4, 1,
        -10, -25, -5, 3, 3, -12, -6, 21,
};

int mg_king_table[64] = {
        17, 11, 45, 4, 2, 14, 3, -60,
        12, 37, -1, -6, -4, 0, -19, -52,
        31, -20, -32, -54, 6, -69, -39, -8,
        0, -4, -61, -26, -16, -29, -12, -41,
        -29, 2, -40, -36, -35, -20, 0, -47,
        15, 19, -3, -38, -25, -7, 27, 14,
        59, 49, 40, -9, -2, 45, 60, 62,
        -42, 50, 55, -19, 54, 13, 84, 77,
};

int eg_pawn_table[64] = {
        -164, -164, -164, -164, -164, -164, -164, -164,
        -16, 62, 85, 38, 0, 89, 114, 117,
        107, 70, 43, 9, -14, -18, 30, 55,
        32, 0, -12, -33, -44, -45, -30, -9,
        8, -22, -35, -41, -46, -46, -43, -29,
        -2, -14, -37, -28, -38, -49, -46, -36,
        15, 0, -13, -12, -17, -31, -29, -32,
        -164, -164, -164, -164, -164, -164, -164, -164,
};

int eg_knight_table[64] = {
        -2, 59, 57, 29, 0, 91, 73, -9,
        -5, 9, -9, -14, -14, -31, 14, -33,
        8, -5, 25, -13, -27, -15, -3, -4,
        -4, 12, 30, 23, 43, 12, 30, -8,
        -11, 27, 44, 41, 31, 34, -2, -5,
        -34, 0, 8, 22, 14, 11, -10, -31,
        -45, -10, -7, 8, 0, -22, -2, -50,
        -41, -62, -25, -7, -19, -39, -39, -92,
};

int eg_bishop_table[64] = {
        64, 81, 76, 49, 18, 44, 11, -10,
        42, 39, 14, 25, -12, -24, -15, -13,
        -1, 11, -4, -14, -36, -52, -10, -23,
        -13, 20, 1, -5, 4, -3, 13, -11,
        -7, -4, 23, 16, 4, 14, 0, -15,
        -21, -1, 13, 9, 22, 5, -3, -17,
        -16, -13, -8, 3, -2, -20, -25, -64,
        18, -9, -57, -14, -28, -32, -14, -19,
};

int eg_rook_table[64] = {
        -52, -18, -10, -14, -13, 22, 13, -28,
        20, 22, -4, -17, -1, -21, 22, -9,
        34, 23, 27, -11, -45, -22, -21, 6,
        30, 45, 31, 15, 10, 10, 8, 1,
        27, 27, 35, 20, 17, 16, 4, -4,
        8, 3, 6, 10, 10, -7, -23, -25,
        -10, -9, -8, -16, -15, -24, -14, -16,
        -12, 0, 3, 5, -10, -11, -4, -34,
};

int eg_queen_table[64] = {
        54, 58, 73, 32, 16, 37, 34, 20,
        21, 65, 75, 82, 94, 56, 34, -10,
        9, 41, 64, 57, 95, 64, 45, 43,
        16, 51, 53, 88, 108, 109, 64, 47,
        -41, 19, 10, 38, 38, 54, 25, 10,
        -72, -38, 12, -1, -9, 5, -38, -53,
        -65, -73, -52, -55, -41, -85, -129, -155,
        -100, -65, -117, -82, -125, -146, -184, -187,
};

int eg_king_table[64] = {
        -3, -2, -41, 0, -12, -5, -7, -71,
        -5, 6, 22, 19, 28, 41, 54, 18,
        -20, 60, 63, 80, 39, 107, 93, 30,
        -8, 36, 77, 63, 60, 76, 62, 27,
        -13, 11, 42, 59, 49, 45, 13, 10,
        -42, -17, 7, 33, 27, 9, -31, -40,
        -80, -58, -51, -19, -16, -53, -66, -86,
        5, -78, -81, -54, -102, -67, -106, -136,
};

const int mg_piece_values[] = {0, 110, 363, 386, 484, 1021, 0};
const int eg_piece_values[] = {0, 164, 227, 254, 504, 795, 0};

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
