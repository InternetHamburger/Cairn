#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        142, 154, 153, 144, 131, 144, 142, 137,
        25, 25, 27, 21, 13, 6, 17, 10,
        -5, -9, -13, -16, -18, -19, -13, -19,
        -16, -17, -21, -21, -22, -24, -22, -26,
        -18, -18, -22, -22, -19, -25, -18, -27,
        -18, -14, -19, -28, -25, -16, -13, -30,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -37, -3, -1, 0, -1, -11, -15, -43,
        -13, 2, 7, 15, 10, 3, -7, -3,
        -2, 7, 18, 14, 18, 14, 7, 0,
        0, 13, 19, 19, 20, 13, 13, 3,
        -2, 7, 17, 14, 17, 17, 5, -3,
        -12, 3, 6, 11, 10, 7, 8, -9,
        -22, -1, -4, 1, 3, 0, -3, -12,
        -12, -28, -14, -10, -6, -10, -26, -29,
};

int mg_bishop_table[64] = {
        -4, -2, 0, 0, 0, -6, -9, -3,
        -8, 4, 4, 1, 2, 5, 5, -9,
        -8, 9, 8, 9, 10, 15, 11, 1,
        -4, 7, 10, 17, 17, 12, 7, 0,
        -4, 5, 12, 15, 13, 8, 2, -4,
        -8, 2, 3, 10, 8, 6, 2, -6,
        -8, -2, 0, -2, 0, -2, -1, -10,
        -19, -3, -23, -10, -12, -19, -15, -27,
};

int mg_rook_table[64] = {
        8, 10, 13, 14, 12, 7, 10, 11,
        17, 15, 15, 14, 14, 14, 14, 13,
        12, 10, 12, 10, 5, 6, 4, 4,
        4, 4, 8, 9, 4, 4, 2, -2,
        -5, 0, 0, 2, -1, -2, -10, -11,
        -13, -5, -6, -5, -8, -8, -10, -17,
        -19, -11, -8, -11, -12, -10, -17, -28,
        -14, -12, -6, -4, -6, -9, -14, -26,
};

int mg_queen_table[64] = {
        -6, 2, 14, 17, 23, 16, 4, 3,
        -8, 0, 14, 24, 30, 18, 9, 10,
        -10, 1, 8, 17, 29, 23, 27, 15,
        -12, 0, 9, 16, 28, 22, 15, 16,
        -13, -10, 1, 8, 13, 10, 13, 3,
        -5, 0, 2, -3, 2, 1, -3, -7,
        -26, -14, -4, -7, -10, -9, -17, -28,
        -20, -33, -22, -16, -25, -40, -50, -44,
};

int mg_king_table[64] = {
        -35, -11, -10, -13, -3, -1, -1, -44,
        -4, 9, 9, 7, 6, 15, 6, -12,
        2, 17, 14, 15, 15, 18, 10, 6,
        3, 15, 15, 16, 16, 16, 16, 0,
        -4, 7, 9, 12, 13, 11, 2, -5,
        -7, 4, 3, 7, 5, 3, 0, -4,
        -3, -1, -1, 0, -2, -1, 0, -5,
        -29, -13, -5, -22, -12, -19, -6, -19,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        150, 152, 135, 134, 118, 120, 104, 127,
        12, 14, 11, 17, 18, 13, 10, 2,
        -9, -6, -13, -11, -9, -13, -12, -23,
        -17, -16, -15, -11, -13, -16, -13, -26,
        -19, -12, -16, -20, -15, -16, -5, -27,
        -22, -12, -21, -34, -28, -10, -6, -30,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -76, -12, -16, -12, 2, -52, -26, -52,
        -19, 0, 14, 14, 9, 27, -6, -7,
        1, 18, 24, 31, 32, 23, 26, -8,
        -1, 9, 27, 24, 23, 33, 12, 13,
        -7, 5, 12, 12, 17, 14, 10, 0,
        -14, 0, 0, 10, 8, 5, 10, -12,
        -19, -9, -3, -2, 0, 7, -2, -14,
        -26, -25, -18, -10, -10, -7, -21, -19,
};

int eg_bishop_table[64] = {
        -5, -8, -8, -14, -19, -38, -1, -6,
        -8, 3, 1, -8, 4, 1, 0, -10,
        -1, 15, 9, 20, 12, 21, 18, 7,
        0, 7, 10, 23, 15, 17, 12, 1,
        -1, 5, 11, 11, 13, 3, 3, -4,
        -3, 2, 3, 6, 1, 7, 1, -2,
        -12, -4, -1, -4, 0, 0, 2, -19,
        -23, -12, -15, -7, -10, -15, -17, -12,
};

int eg_rook_table[64] = {
        16, 27, 23, 28, 22, 15, 8, 13,
        17, 17, 27, 32, 23, 26, 14, 19,
        14, 15, 17, 20, 17, 16, 9, 4,
        -3, 0, 6, 4, 1, 1, -7, -4,
        -13, -14, -8, -3, -8, -11, -12, -16,
        -14, -11, -12, -12, -12, -19, -12, -22,
        -26, -19, -13, -15, -16, -11, -19, -36,
        -16, -11, -8, -9, -9, -10, -21, -25,
};

int eg_queen_table[64] = {
        0, 4, 20, 23, 31, 32, 17, 9,
        -11, -3, 7, 9, 13, 41, 29, 19,
        -9, -5, 7, 16, 27, 30, 28, 18,
        -16, -13, 0, 4, 7, 10, 3, 4,
        -9, -11, -5, -1, 0, -3, 1, -6,
        -18, -8, -4, -7, -6, -4, -4, -11,
        -21, -9, -7, -7, -8, -9, -12, -29,
        -15, -20, -17, -8, -13, -25, -24, -22,
};

int eg_king_table[64] = {
        -15, -68, 42, 12, 54, -14, -104, -88,
        -1, -3, 39, 11, 75, 43, 6, -41,
        3, 28, 15, 25, 27, 29, 30, -8,
        -18, 6, 18, 2, 13, 16, 4, -9,
        -25, -1, -2, 0, -4, -5, -3, -29,
        -11, 3, -1, -5, -5, -3, 2, -4,
        0, 5, -1, -6, -8, 1, 6, 0,
        -26, 0, 2, -18, 0, -13, 11, 0,
};

const int mg_piece_values[] = {0, 85, 167, 187, 289, 528, 0};
const int eg_piece_values[] = {0, 76, 194, 210, 314, 606, 0};

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
