#include "evaluation.h"

#include <stdlib.h>

#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        164, 140, 98, 116, 102, 66, 26, 32,
        -36, -5, 2, 27, 26, 32, 1, -23,
        -46, -15, -22, -8, 1, -3, -13, -42,
        -51, -30, -26, -12, -8, -14, -11, -41,
        -51, -23, -28, -26, -19, -17, 14, -33,
        -58, -30, -35, -54, -37, -3, 18, -41,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -141, -122, -93, -5, 50, -117, -7, -116,
        -44, -5, 48, 69, 68, 76, -27, -8,
        -14, 32, 47, 79, 106, 116, 51, 34,
        0, 10, 41, 51, 45, 77, 30, 39,
        -18, 0, 18, 19, 28, 21, 23, 5,
        -34, -6, -6, 13, 18, 10, 16, -21,
        -52, -41, -20, -12, -5, 3, -13, -13,
        -60, -43, -44, -37, -28, -20, -33, -41,
};

int mg_bishop_table[64] = {
        -47, -42, -60, -30, -19, -101, -36, -28,
        -31, 0, 3, 9, 17, 14, 1, -2,
        -2, 12, 28, 48, 54, 76, 40, 25,
        -8, 11, 31, 51, 36, 44, 18, 10,
        -4, 2, 18, 29, 28, 8, 7, 0,
        -9, 9, 0, 10, 1, 12, 5, 4,
        -1, -6, 3, -14, -1, 5, 16, -11,
        -51, -26, -23, -27, -33, -32, -21, -19,
};

int mg_rook_table[64] = {
        53, 53, 52, 89, 83, 61, 64, 77,
        15, 22, 47, 71, 75, 75, 28, 43,
        -12, 9, 16, 51, 62, 62, 61, 17,
        -30, -23, -3, 11, 20, 16, 10, -13,
        -44, -44, -30, -18, -18, -26, -16, -29,
        -61, -43, -41, -45, -28, -31, -12, -46,
        -61, -54, -39, -38, -35, -14, -23, -85,
        -41, -37, -30, -24, -18, -27, -47, -52,
};

int mg_queen_table[64] = {
        -33, 19, 61, 75, 97, 104, 56, 21,
        -32, -31, 0, 5, 21, 45, 12, 22,
        -25, -23, -8, 11, 42, 81, 38, 25,
        -26, -21, -12, -7, 4, 15, 0, 5,
        -18, -29, -19, -10, -10, -9, 2, -15,
        -31, -12, -22, -19, -16, -6, -4, -18,
        -35, -25, -7, -11, -12, 2, -4, -23,
        -26, -36, -27, -12, -13, -42, -10, -21,
};

int mg_king_table[64] = {
        160, 73, 56, 47, 60, 125, 99, 44,
        73, 61, 25, 5, 16, 38, 59, 11,
        33, 3, -21, -15, -34, 8, 19, 38,
        2, -11, -52, -65, -57, -32, -19, -45,
        -32, -39, -64, -76, -60, -51, -38, -68,
        -19, -14, -53, -62, -67, -54, -16, -36,
        44, -4, -13, -43, -45, 3, 18, 32,
        -22, 17, 10, -43, 7, -20, 51, 53,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        81, 99, 98, 71, 56, 77, 99, 99,
        61, 46, 30, 16, 5, -6, 28, 26,
        8, -9, -19, -32, -40, -39, -22, -13,
        -10, -18, -35, -39, -45, -40, -30, -30,
        -21, -24, -36, -34, -34, -36, -38, -39,
        -12, -15, -23, -25, -25, -29, -33, -39,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -62, 1, 19, 4, -10, 25, -24, -59,
        -10, 3, -3, 6, 4, -7, 5, -13,
        1, 4, 31, 23, 13, 9, 10, -6,
        -3, 19, 32, 30, 46, 27, 30, -4,
        -8, 16, 37, 37, 37, 37, 14, -10,
        -31, 5, 20, 26, 25, 15, 4, -18,
        -27, -13, -1, 11, 5, -5, -8, -27,
        -55, -57, -19, -19, -13, -22, -35, -69,
};

int eg_bishop_table[64] = {
        4, 15, 6, 10, 4, 9, -4, -32,
        7, 12, 3, 1, 7, 6, -2, -26,
        -4, 15, 11, 3, 8, 18, 8, -8,
        -9, 22, 16, 22, 28, 12, 19, -7,
        -17, 10, 24, 26, 22, 24, 11, -16,
        -11, 2, 19, 17, 23, 8, 3, -18,
        -30, -14, -4, 4, -1, -11, -11, -24,
        -28, -21, -47, -13, -17, -26, -27, -35,
};

int eg_rook_table[64] = {
        -8, 1, 6, 0, 7, 11, 3, 0,
        19, 21, 15, 10, 2, 0, 15, 9,
        23, 17, 17, 2, 2, 5, 3, 8,
        20, 24, 20, 15, 7, 12, 7, 6,
        3, 12, 13, 5, 4, 9, 3, -11,
        -15, -9, -4, -1, -11, -9, -21, -24,
        -28, -16, -11, -16, -21, -20, -27, -28,
        -18, -10, -4, -2, -10, -12, 0, -27,
};

int eg_queen_table[64] = {
        18, -1, -13, -5, -9, -7, -4, 24,
        10, 38, 51, 57, 66, 51, 40, 20,
        -14, 30, 36, 45, 59, 37, 52, 36,
        -9, 20, 47, 65, 84, 71, 64, 35,
        -32, 15, 28, 51, 42, 42, 17, 23,
        -37, -33, 8, -5, 3, 0, -18, -11,
        -52, -42, -47, -38, -34, -66, -87, -68,
        -51, -57, -68, -72, -99, -74, -150, -83,
};

int eg_king_table[64] = {
        -110, -17, -26, -40, -26, -32, -8, -74,
        -17, 2, 10, 11, 13, 21, 18, 12,
        -11, 26, 29, 33, 37, 41, 39, 9,
        2, 32, 42, 44, 52, 49, 44, 16,
        -8, 20, 34, 45, 45, 39, 28, 6,
        -19, 0, 19, 28, 34, 22, 4, -7,
        -39, -18, -7, 4, 5, -9, -16, -33,
        -54, -48, -38, -38, -57, -42, -49, -76,
};

const int mg_piece_values[] = {0, 97, 324, 349, 453, 1012, 0};
const int eg_piece_values[] = {0, 156, 293, 322, 561, 972, 0};

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
