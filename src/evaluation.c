#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        178, 168, 151, 136, 131, 142, 123, 138,
        11, 15, 14, 15, 13, 13, 8, 0,
        -14, -8, -13, -11, -9, -14, -11, -24,
        -20, -18, -16, -14, -16, -17, -16, -27,
        -23, -14, -19, -18, -16, -20, -9, -28,
        -26, -17, -22, -34, -29, -11, -8, -30,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -81, -4, -9, 5, 20, -49, 17, -61,
        -21, 4, 15, 30, 24, 23, 0, -7,
        -10, 20, 25, 37, 40, 31, 24, -1,
        3, 10, 22, 26, 23, 36, 16, 12,
        -6, 4, 14, 12, 16, 13, 7, 0,
        -15, -1, -1, 8, 10, 3, 4, -16,
        -17, -15, -5, -2, -2, 0, -8, -9,
        -47, -26, -20, -18, -16, -14, -28, -24,
};

int mg_bishop_table[64] = {
        -7, -10, -5, -5, -5, -25, 1, -17,
        -9, 6, 6, 11, 10, 11, -2, -15,
        -7, 7, 17, 19, 22, 29, 15, 6,
        0, 8, 12, 22, 19, 14, 9, 0,
        -9, 5, 9, 12, 9, 5, 4, 0,
        -2, 2, 0, 4, 2, 3, 3, -3,
        -10, -5, -4, -5, -2, 0, 0, -16,
        -26, -13, -18, -11, -13, -21, -26, -23,
};

int mg_rook_table[64] = {
        14, 20, 20, 23, 23, 20, 15, 19,
        22, 24, 28, 27, 33, 36, 15, 23,
        9, 16, 15, 22, 28, 26, 20, 2,
        -2, 0, 4, 5, 4, 3, 0, 0,
        -17, -13, -7, -2, -7, -9, -9, -18,
        -24, -15, -17, -18, -15, -17, -13, -24,
        -29, -20, -16, -19, -17, -17, -17, -40,
        -18, -15, -11, -9, -10, -12, -25, -26,
};

int mg_queen_table[64] = {
        -8, 16, 28, 32, 37, 50, 24, 13,
        -15, -2, 9, 10, 30, 48, 40, 23,
        -16, 0, 4, 16, 28, 47, 37, 20,
        -14, -9, 2, 6, 15, 18, 3, 4,
        -15, -12, -6, -3, 0, 0, 0, -7,
        -24, -13, -9, -7, -7, -4, -6, -12,
        -27, -19, -10, -14, -10, -9, -29, -24,
        -21, -24, -22, -12, -27, -35, -45, -27,
};

int mg_king_table[64] = {
        -89, 61, 11, -65, -64, -13, -36, -82,
        22, 3, 8, 11, -25, 47, 15, 61,
        -1, 35, 25, 39, 29, 52, 63, 13,
        -5, 20, 10, 13, 3, 23, 10, -18,
        -24, 0, -2, 0, 0, 2, 0, -17,
        -6, -2, -4, -4, -5, -3, 0, -11,
        -2, -1, -4, -12, -11, 0, 3, -4,
        -30, -10, -1, -24, -4, -16, 7, -1,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        135, 148, 151, 143, 132, 138, 134, 137,
        27, 25, 18, 20, 14, 5, 13, 10,
        -4, -4, -11, -11, -16, -18, -14, -19,
        -14, -15, -18, -16, -22, -24, -18, -25,
        -18, -13, -21, -19, -19, -22, -15, -25,
        -15, -13, -17, -21, -22, -13, -11, -26,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -37, -3, 2, -1, 6, -10, -17, -28,
        -18, 2, 5, 14, 10, 9, 0, -8,
        1, 12, 18, 17, 22, 18, 12, 0,
        -2, 8, 24, 22, 22, 13, 14, 6,
        -5, 6, 17, 15, 19, 14, 14, 0,
        -16, -4, 3, 11, 11, 8, 4, -10,
        -25, -9, -6, 1, 0, 4, -8, -9,
        -34, -31, -21, -14, -14, -17, -28, -28,
};

int eg_bishop_table[64] = {
        1, 4, -2, 5, 2, -3, 0, -6,
        -8, 3, 5, 4, 7, 4, 2, -4,
        -6, 7, 12, 13, 12, 15, 15, 0,
        -4, 9, 8, 16, 18, 12, 8, 4,
        -5, 4, 12, 16, 17, 6, 5, 0,
        -6, 0, 3, 10, 9, 6, 0, -2,
        -12, -6, -2, -1, -4, -4, 0, -21,
        -26, -15, -21, -11, -16, -22, -25, -24,
};

int eg_rook_table[64] = {
        -1, 8, 17, 15, 12, 11, 10, 10,
        15, 19, 20, 17, 14, 19, 15, 15,
        11, 8, 12, 10, 10, 11, 11, 5,
        3, 6, 10, 6, 4, 4, 2, 0,
        -5, 1, 0, 1, -3, 0, -3, -11,
        -15, -5, -7, -8, -9, -9, -7, -16,
        -22, -16, -14, -12, -16, -15, -19, -29,
        -17, -14, -6, -6, -6, -10, -17, -23,
};

int eg_queen_table[64] = {
        -2, 11, 20, 20, 21, 33, 15, 23,
        -15, 6, 14, 26, 20, 32, 22, 26,
        -17, 0, 14, 19, 26, 34, 24, 21,
        -12, -2, 4, 19, 29, 26, 18, 13,
        -15, -13, 3, 7, 7, 11, 10, 1,
        -19, -14, -4, -8, -2, 1, -9, -14,
        -21, -15, -13, -12, -16, -13, -31, -30,
        -28, -30, -25, -22, -36, -40, -55, -40,
};

int eg_king_table[64] = {
        -35, -13, -18, -16, -7, -7, -10, -69,
        -1, 13, 5, 3, 4, 7, 10, -32,
        9, 20, 18, 16, 16, 15, 10, 3,
        2, 18, 17, 19, 18, 16, 16, 2,
        -4, 6, 13, 17, 13, 12, 7, -5,
        -7, 1, 5, 5, 7, 5, 2, -10,
        -5, -4, -4, -3, -6, -2, 0, -5,
        -17, -12, -9, -25, -13, -20, -5, -19,
};

const int mg_piece_values[] = {0, 84, 222, 238, 346, 657, 0};
const int eg_piece_values[] = {0, 86, 176, 196, 301, 547, 0};

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
