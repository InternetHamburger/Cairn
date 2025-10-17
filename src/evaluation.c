#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        46, 99, 90, 90, 26, -33, 60, 29,
        28, 39, 24, 26, 9, 24, 30, 7,
        -4, -11, -5, -7, -23, -15, -26, -33,
        -12, -24, -16, -18, -19, -53, -39, -21,
        -14, -10, -11, -30, -20, -39, -18, -43,
        4, -5, -1, -24, -36, -18, 12, -14,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -36, 6, 1, 30, -38, -20, -49, -66,
        21, 19, -15, 30, 25, 10, 13, -16,
        14, 17, 28, 40, 32, 27, -6, -4,
        0, -12, 11, 23, 21, 26, 12, 2,
        -12, -4, 18, 2, 14, 1, -20, 18,
        4, -30, 26, 1, 3, 10, 7, 1,
        -26, -11, -25, -1, 14, -45, 69, -3,
        -78, -7, 3, -10, -47, -17, 47, -48,
};

int mg_bishop_table[64] = {
        3, 4, 13, -30, -19, -3, 1, -8,
        -24, 10, -23, 27, -36, 7, -6, -17,
        14, -1, 33, 13, 30, 11, 0, 9,
        -3, -22, 0, 19, 2, 32, -40, 9,
        -5, -14, 21, 11, 36, -17, 9, -39,
        -17, 12, -9, 24, -21, 4, -32, 13,
        22, 3, 30, -17, 29, -1, 16, 37,
        -31, 47, -27, -17, 5, -15, -52, -8,
};

int mg_rook_table[64] = {
        3, 34, 28, 13, 25, -17, -13, 20,
        20, 26, 11, 32, 31, 20, 9, 19,
        22, 23, -1, 5, 20, -1, -7, 13,
        12, -5, -7, -1, 7, 8, -18, -4,
        -4, 13, -2, 2, 3, -12, -9, -9,
        4, -6, -7, -11, -16, -21, -25, -21,
        -5, -13, -3, -18, -11, -2, -27, -16,
        7, -8, -30, -15, -5, -6, -32, -19,
};

int mg_queen_table[64] = {
        2, 2, 2, 2, 2, 2, 2, 2,
        1, 0, 2, 2, 2, 2, 2, 2,
        2, 1, 2, 2, 2, 2, 2, 2,
        0, 1, 2, 2, 2, 2, 2, 2,
        -1, 2, -1, 2, 2, 2, 2, 2,
        0, 1, 0, 1, 0, 2, -2, 2,
        2, 1, 2, 2, -3, -11, 2, 0,
        -28, -5, 0, 2, -2, -4, 1, -24,
};

int mg_king_table[64] = {
        -108, 37, 35, 20, -34, 18, -145, 65,
        -99, -8, 29, 49, -7, -7, 75, 189,
        -47, 10, -44, 50, 98, 45, 14, 89,
        -28, -63, 55, -3, -6, -66, 25, -23,
        -72, 19, -47, -14, -41, -8, 24, -28,
        -17, -11, 6, -55, -32, -12, 7, 41,
        4, -4, -31, -14, -20, 6, 24, 35,
        -15, 7, 11, -14, 13, 3, 16, 5,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        41, 44, 48, -71, 20, -6, 51, 45,
        -30, 76, 29, 28, 18, 45, 19, 5,
        -4, -3, -5, -1, 5, 2, 0, -18,
        -18, -10, -19, -7, -6, -15, -19, -14,
        -28, -11, -12, -17, -18, -22, 23, -17,
        -28, -23, -23, -36, -21, 4, 19, -20,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -33, -50, -4, 154, 96, -75, -102, -91,
        -40, 13, -10, 116, 50, 43, -17, 38,
        -20, 76, 36, 84, 66, 50, 6, 3,
        -1, 5, -19, 37, 28, 46, 17, 19,
        -18, 7, 6, 8, 18, 11, -4, 12,
        -23, -15, 3, -15, 6, 3, 16, -11,
        -91, -46, -25, -27, 1, -2, 20, 8,
        122, -41, -147, -32, 72, -2, -17, -318,
};

int eg_bishop_table[64] = {
        -16, -123, -46, 75, 64, -47, -45, -36,
        9, 21, 26, 59, 40, -22, 65, 11,
        5, 51, 11, 32, 21, 74, 20, 31,
        30, -16, 6, 7, 7, 16, 3, -28,
        -31, 22, 17, 20, 23, 6, -12, -29,
        7, -17, 4, 7, -8, -8, -42, 14,
        32, 3, 2, -9, 0, 10, 18, 18,
        -62, 67, -13, -96, 4, -17, -212, -26,
};

int eg_rook_table[64] = {
        -277, 73, 83, 85, 84, 33, -76, 77,
        5, -4, -25, 79, -95, 81, -89, 35,
        40, -79, 25, 62, 60, 24, -312, 56,
        21, -22, 26, 30, -3, -21, 7, 15,
        -7, 9, -5, 9, 19, 4, 1, 2,
        10, 10, -30, 4, 17, -5, 8, -9,
        2, -6, 3, 9, 24, 11, -11, -22,
        -8, -10, 0, -16, 15, 4, -30, -2,
};

int eg_queen_table[64] = {
        11, 11, 11, 11, 10, 11, 11, 11,
        8, -8, 10, -56, 11, 11, 11, 10,
        -1, -58, -6, 11, 11, 11, 11, -2,
        8, 5, 10, 0, 10, 5, 10, 11,
        5, -1, 1, 6, 11, -5, 2, -14,
        7, 1, -10, 9, 8, 11, -15, 11,
        -25, -5, 10, 5, -2, 6, 11, 7,
        -20, -8, -4, 10, -16, -52, -55, -5,
};

int eg_king_table[64] = {
        -8, 62, 64, 57, -58, 64, -10, 44,
        17, 51, 65, 70, -11, 43, 206, 223,
        37, 60, -75, 81, 178, 104, 48, -38,
        11, -289, -68, 28, 22, -299, -159, -161,
        -250, -25, -208, -18, -29, -173, -29, -115,
        -205, -150, 23, 58, -88, -7, 9, 78,
        35, 45, 33, 42, 31, 52, 72, 95,
        -173, 99, 87, 25, 68, 68, 87, 105,
};

const int mg_piece_values[] = {0, 100, 145, 180, 293, 420, 0};
const int eg_piece_values[] = {0, 63, 173, 211, 296, 411, 0};

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
