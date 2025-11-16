#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        313, 321, 312, 296, 252, 258, 275, 260,
        70, 62, 69, 49, 20, 21, 25, 28,
        -3, -3, -28, -32, -34, -41, -37, -36,
        -23, -31, -41, -31, -52, -61, -57, -55,
        -20, -25, -47, -53, -53, -67, -45, -55,
        -28, -23, -48, -61, -69, -35, -33, -54,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -45, 1, -14, 16, 22, -43, -12, -38,
        -44, 6, 37, 29, 20, 34, 7, -26,
        -14, 20, 28, 42, 34, 44, 27, 11,
        -11, -5, 26, 43, 44, 63, 26, 9,
        -39, 24, 29, 37, 30, 31, 9, 9,
        -18, 1, 2, 21, 0, 13, -1, -36,
        -29, -19, -10, -4, -15, -3, -41, -9,
        -54, -42, -43, -41, -51, -40, -34, -32,
};

int mg_bishop_table[64] = {
        -23, -9, -6, 0, 36, -23, 0, 6,
        -4, 5, 20, 5, 19, 5, -2, -39,
        -17, 4, 3, 31, 20, 27, 19, -2,
        0, 18, 30, 19, 38, 28, 8, 9,
        0, 6, 28, 38, 14, 12, -4, -15,
        4, 9, 7, 14, -1, 12, -15, -4,
        -28, -7, -3, -10, -4, -7, 5, -49,
        -44, -26, -25, -35, -28, -24, -29, -17,
};

int mg_rook_table[64] = {
        31, 35, 30, 31, 12, 8, 10, 10,
        26, 29, 27, 22, 20, 10, 6, 13,
        17, 24, 26, 22, 27, 16, 11, 8,
        19, 21, 16, 20, 15, 11, 5, 3,
        0, 9, 1, 1, -1, -10, -5, -6,
        -23, -9, -9, -7, -18, -29, -31, -39,
        -32, -10, -22, -7, -29, -44, -43, -70,
        -14, -18, -9, -7, 2, -8, -43, -49,
};

int mg_queen_table[64] = {
        1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 0, 1, 0, -20, 1,
        1, 1, 0, 0, 0, -34, -40, -16,
};

int mg_king_table[64] = {
        16, -28, -6, 11, -22, 4, -10, -33,
        -27, 5, 47, 22, 39, 39, 12, -25,
        8, 39, 29, 35, 21, 53, -1, 32,
        -4, 25, 22, 24, 34, 32, 43, 9,
        3, 10, 23, 7, 20, 16, 10, -6,
        -23, -9, -17, -13, -7, 5, 0, -16,
        -55, -25, -9, -31, -34, -2, 0, -15,
        -43, -49, -67, -63, -13, -32, 3, -26,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        299, 285, 295, 271, 236, 245, 227, 208,
        31, 47, 40, 49, 26, 31, 16, 14,
        -18, -3, -20, -13, -17, -35, -15, -39,
        -34, -27, -37, -16, -33, -45, -39, -58,
        -32, -23, -32, -42, -48, -62, -16, -59,
        -53, -24, -53, -64, -73, -19, -16, -68,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -48, -32, -13, 2, 21, -141, -4, -22,
        -13, -14, 51, 35, 9, 92, -16, 32,
        -21, 56, 48, 61, 90, 36, 69, 7,
        0, 13, 38, 53, 42, 40, 18, 32,
        -13, -13, 24, 20, 23, 20, 14, 2,
        -21, -1, -2, 6, 7, 13, 13, -28,
        -53, -15, -10, -9, -14, -22, -34, -22,
        -70, -32, -43, -47, -51, -54, -35, -67,
};

int eg_bishop_table[64] = {
        22, -58, -12, -43, 3, -92, -54, 12,
        -24, 19, 31, 6, -8, -5, 34, -35,
        -13, 20, 37, 30, 27, 35, 31, -8,
        12, 29, 22, 32, 27, 31, 14, -6,
        -1, 11, 20, 35, 20, 15, -16, 32,
        0, 9, 5, 8, -3, 14, 0, 11,
        -46, 0, -13, -1, 4, 22, 26, -59,
        -30, -45, -18, -43, -35, -22, 0, -41,
};

int eg_rook_table[64] = {
        40, 57, 39, 84, 36, -12, 9, 12,
        33, 46, 69, 76, 47, 38, 22, 21,
        18, 36, 12, 31, 30, 27, 11, -1,
        -6, 10, 10, 11, 5, 6, -17, -4,
        -12, -24, 0, -16, -1, -28, -32, -27,
        -29, -18, -13, -24, -26, -35, -33, -47,
        -28, -22, -35, -38, -15, -26, -36, -86,
        -18, -14, -11, -12, 1, -5, -63, -44,
};

int eg_queen_table[64] = {
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 7, 8, 8, 8,
        7, 8, 8, 8, 8, 8, 8, 8,
        7, -2, 8, 7, 3, 7, -18, 7,
        -2, -8, -6, -13, 2, -6, -16, 7,
        -16, -1, 0, 3, -12, 0, 7, 8,
        3, 2, 8, -4, 7, 2, 7, -14,
        -4, -4, -4, 8, -14, -16, -83, -43,
};

int eg_king_table[64] = {
        -151, 300, -245, 21, 21, 21, 21, 138,
        -75, -27, -95, -54, -37, -81, 93, 27,
        108, 44, -200, -89, 106, 61, 36, 97,
        67, -98, 27, -52, 13, 50, 50, -49,
        47, -9, -45, -1, -4, -30, 0, -35,
        -89, -6, -25, -33, -31, -2, 15, 35,
        -22, 7, 19, -32, -35, 33, 50, 28,
        38, 7, -38, -37, 31, 11, 70, 35,
};

const int mg_piece_values[] = {0, 165, 297, 338, 498, 790, 0};
const int eg_piece_values[] = {0, 147, 323, 358, 506, 784, 0};

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

int eval(const Board *board) {
    uint64_t occupied = GetOccupied(board);
    int num_pieces = __builtin_popcountll(occupied);

    int mg_eval = 0;
    int eg_eval = 0;
    while (occupied){
        const int square = poplsb(&occupied);
        mg_eval += mg_piece_square_values[board->squares[square]][square];
        eg_eval += eg_piece_square_values[board->squares[square]][square];
    }

    // Interpolate based on number of pieces
    int eval = (mg_eval * num_pieces + eg_eval * (32 - num_pieces)) / 32;

    return eval * (board->white_to_move ? 1 : -1);
}
