#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        67, 85, 96, 121, 15, 48, 7, 70,
        -16, 38, -1, 28, -3, 52, 18, -14,
        2, -7, -11, 14, -9, -36, -4, -32,
        -9, -26, -16, -17, -25, -20, -23, -29,
        -14, -31, -41, -44, -31, -48, -44, -30,
        8, -8, -4, -7, 8, -1, -52, -24,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        32, -29, -44, 2, 35, -48, -28, -19,
        -58, 10, 6, -10, -25, 16, -42, -22,
        -60, 18, 22, 40, 8, 9, 25, -25,
        27, 19, -13, 2, 42, 36, 14, 0,
        29, 12, 7, 11, 20, 6, 7, -4,
        54, 19, 36, -17, -17, 9, -47, -14,
        26, 11, -8, 29, 43, -14, -31, 14,
        -109, 90, -86, -45, -22, 41, 27, -16,
};

int mg_bishop_table[64] = {
        32, -22, 14, 7, 38, -33, -14, -17,
        7, 35, 0, 24, 27, 13, -33, -14,
        -40, 14, 21, 23, 27, 13, -12, 47,
        -69, 12, -5, -8, 2, -4, 28, -14,
        22, 17, -9, 9, 3, 17, -17, -7,
        -40, -26, -29, 21, -4, -3, -15, 3,
        7, -35, 29, -8, -36, -36, 12, 3,
        17, 77, 1, -5, -9, 13, -63, -8,
};

int mg_rook_table[64] = {
        12, 30, 25, 13, 15, 29, 15, 5,
        33, 39, 29, 6, 20, 33, -2, 25,
        4, -1, 8, 3, 6, 26, 15, 6,
        19, 10, 7, 14, -20, 11, -11, 4,
        -6, -5, -6, -2, 0, -4, -5, -5,
        3, 2, -2, -18, -8, 3, 3, -24,
        -10, -18, 1, -22, -9, -12, -20, -41,
        -28, -30, -26, -26, -5, -17, -66, -23,
};

int mg_queen_table[64] = {
        2, 2, -1, 2, -4, 2, 1, 2,
        -14, 2, 2, 2, 2, 2, 2, 1,
        1, 2, 2, 2, 2, 2, 2, 2,
        1, 2, 2, 1, 2, 2, 2, 2,
        2, 2, 2, 2, 2, -2, 1, 2,
        1, 2, 2, 2, -1, 2, 2, 1,
        -17, 2, 1, 0, 1, 2, 1, 2,
        1, 2, -4, -2, -3, -7, -15, -6,
};

int mg_king_table[64] = {
        -29, 159, 103, 119, 77, -16, -23, -110,
        -2, 2, -24, 30, 102, -9, -113, -43,
        0, -101, -32, 106, 86, 12, -90, 27,
        18, -32, -71, 22, 25, 2, 35, -148,
        -86, -30, -8, -2, 40, -19, 4, -37,
        -9, -21, -15, -27, 14, 6, -19, 29,
        17, 17, -4, -3, -20, -3, 11, 0,
        18, 31, 7, -21, 11, 1, 12, 24,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        64, 42, 149, 275, 14, -89, -9, -10,
        -49, 50, -10, 37, 7, 11, 29, -3,
        -13, 3, 8, -8, -2, -9, 4, -18,
        -30, -10, -11, -10, -18, -28, -16, -20,
        -23, -10, -4, -19, -31, -47, -10, -45,
        -26, -8, -7, -16, -33, -7, -19, -28,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -25, 2, -42, 115, -89, -57, -21, -89,
        -12, 7, 34, 102, 46, 87, -25, 17,
        -64, 90, 53, 54, 75, 83, 16, 26,
        48, 29, 12, 35, 41, 84, 21, 16,
        24, 33, 14, 23, 5, 21, 1, -26,
        -8, 18, 5, -18, 6, -3, -66, -14,
        -9, 9, 4, -16, 12, -27, -181, 32,
        -192, -38, -162, -41, -27, 2, -6, -44,
};

int eg_bishop_table[64] = {
        -48, -58, -40, 40, -33, -36, 88, -73,
        56, 4, 18, 43, -4, -13, 7, 36,
        -12, 4, 39, 4, 124, 102, 19, 56,
        -41, 6, 56, 17, 33, 57, 20, 8,
        7, 54, 5, 17, 17, 10, -38, 3,
        -19, -52, 1, 5, 4, -10, -21, 8,
        23, -11, 17, -15, 13, -6, 22, -123,
        -19, -19, -3, -111, 130, -7, -357, -5,
};

int eg_rook_table[64] = {
        -30, 99, 118, -50, -343, -109, 54, 83,
        86, 72, -29, 116, 106, 113, 83, 103,
        86, -10, -3, -106, -367, -123, -130, 63,
        24, 7, 9, -50, -56, -22, 2, 29,
        -6, -16, 42, -25, 31, -1, -22, 26,
        -28, 22, 33, 31, -14, 23, -36, 8,
        1, 38, 18, 17, 28, 10, -36, -16,
        5, 23, 2, 1, 24, 4, -14, 2,
};

int eg_queen_table[64] = {
        16, 14, -164, 19, -22, 19, 19, -10,
        -11, 18, 19, 19, 19, 19, 19, -21,
        19, 2, 11, 19, 19, 1, 19, 19,
        0, 18, 8, -10, 5, 19, 6, 13,
        10, 19, 10, 19, -7, -30, -7, -5,
        -35, 19, -6, 19, 14, 19, -28, 18,
        -44, 19, 18, 16, 8, -16, 19, -134,
        19, 19, 8, 14, -9, 3, -114, 3,
};

int eg_king_table[64] = {
        -6, 63, 53, 74, 57, 16, 30, -46,
        2, 14, 1, 28, 163, 17, -148, 29,
        16, -149, 17, 171, 157, -33, -121, 112,
        106, 41, -121, -208, 97, -15, -30, -364,
        -69, 217, -83, -91, -63, -128, 72, -225,
        -16, -205, -60, -45, -17, -7, -4, 142,
        48, 26, -8, -6, -16, 32, 74, 115,
        -6, 46, 27, 26, 41, 32, 68, 60,
};

const int mg_piece_values[] = {0, 100, 135, 173, 297, 449, 0};
const int eg_piece_values[] = {0, 71, 149, 188, 281, 432, 0};

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

    int mg_eval = 0;
    int eg_eval = 0;
    while (occupied){
        const int square = poplsb(&occupied);
        mg_eval += mg_piece_square_values[board->squares[square]][square];
        eg_eval += eg_piece_square_values[board->squares[square]][square];
    }

    // Interpolate based on number of pieces
    int num_pieces = __builtin_popcountll(occupied);
    int eval = (mg_eval * num_pieces + eg_eval * (32 - num_pieces)) / 32;

    return eval * (board->white_to_move ? 1 : -1);
}
