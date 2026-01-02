#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        209, 209, 182, 178, 192, 173, 151, 165,
        -2, 7, 5, 15, 13, 7, 6, -11,
        -25, -15, -22, -18, -16, -22, -18, -33,
        -30, -25, -26, -20, -24, -25, -22, -37,
        -34, -23, -29, -28, -25, -28, -15, -37,
        -36, -25, -30, -42, -37, -18, -14, -39,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -89, 11, -12, 11, 18, -50, -1, -61,
        -14, 4, 22, 31, 22, 29, 4, -6,
        0, 18, 30, 42, 51, 39, 31, 2,
        -2, 9, 29, 30, 25, 38, 16, 12,
        -7, 0, 15, 13, 17, 15, 12, -1,
        -18, -4, -1, 8, 9, 2, 4, -17,
        -27, -17, -7, -3, -3, 0, -12, -14,
        -48, -31, -25, -23, -19, -19, -31, -36,
};

int mg_bishop_table[64] = {
        -23, -2, 0, 0, 3, -40, 0, -9,
        -13, 1, 7, 6, 9, 14, -1, -8,
        -3, 8, 22, 22, 30, 33, 22, 6,
        -2, 11, 13, 26, 22, 18, 9, 0,
        -11, 3, 11, 16, 16, 5, 4, -1,
        -5, 0, 1, 5, 1, 3, 0, -2,
        -15, -5, -4, -6, -1, -2, 2, -20,
        -27, -16, -19, -14, -16, -22, -24, -26,
};

int mg_rook_table[64] = {
        23, 27, 25, 27, 30, 33, 24, 29,
        23, 27, 34, 35, 38, 48, 23, 29,
        9, 11, 16, 23, 30, 35, 20, 6,
        -4, -4, 2, 7, 4, 5, -1, -2,
        -19, -14, -11, -4, -11, -9, -9, -19,
        -28, -17, -19, -23, -18, -20, -14, -26,
        -36, -28, -23, -23, -23, -18, -22, -46,
        -22, -19, -14, -12, -12, -15, -25, -27,
};

int mg_queen_table[64] = {
        -7, 12, 25, 35, 40, 42, 38, 26,
        -13, -5, 19, 14, 24, 55, 39, 31,
        -18, 0, 7, 20, 33, 52, 39, 22,
        -18, -11, 0, 8, 17, 20, 8, 4,
        -18, -14, -6, -1, 0, 0, 0, -9,
        -25, -16, -13, -11, -9, -7, -9, -16,
        -30, -20, -12, -16, -14, -12, -27, -27,
        -28, -31, -28, -16, -29, -40, -40, -28,
};

int mg_king_table[64] = {
        -133, 59, 55, -4, -4, -63, -39, -101,
        41, 69, 48, -4, 38, 59, 35, -44,
        40, 23, 40, 0, 73, 60, 63, 31,
        -8, 19, 9, -17, 6, 19, 15, -36,
        -50, -11, -6, 5, -2, 0, 0, -28,
        -17, -11, -8, -13, -9, -7, -3, -17,
        -5, -4, -7, -16, -16, -4, -1, -6,
        -35, -11, -4, -34, -11, -24, 3, -6,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        162, 180, 181, 173, 158, 166, 155, 159,
        27, 26, 19, 12, 7, 0, 8, 8,
        -6, -7, -17, -19, -24, -27, -20, -22,
        -18, -19, -25, -25, -29, -30, -24, -30,
        -25, -18, -27, -25, -25, -28, -20, -31,
        -19, -14, -22, -26, -27, -16, -15, -31,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -39, 0, 0, 9, 6, -5, -5, -55,
        -9, 2, 11, 23, 15, 11, -1, -8,
        0, 9, 25, 28, 20, 23, 18, 0,
        0, 8, 18, 25, 28, 27, 18, 5,
        -7, 6, 23, 20, 18, 19, 9, 0,
        -22, -6, 3, 13, 14, 9, 6, -12,
        -35, -8, -7, 0, 0, 1, -5, -15,
        -58, -42, -31, -19, -18, -18, -33, -45,
};

int eg_bishop_table[64] = {
        -9, 0, -3, -1, 0, -6, -3, -16,
        -5, 2, 6, 5, 6, 4, -1, -7,
        -3, 11, 12, 15, 15, 18, 14, 3,
        -6, 11, 13, 18, 17, 13, 15, 0,
        -6, 4, 15, 17, 13, 9, 4, -4,
        -7, 1, 6, 9, 11, 8, 1, -6,
        -17, -8, -2, -2, 0, 0, 0, -24,
        -28, -22, -28, -13, -17, -22, -27, -25,
};

int eg_rook_table[64] = {
        1, 13, 19, 18, 17, 18, 14, 13,
        18, 20, 26, 23, 24, 23, 19, 18,
        12, 13, 15, 14, 12, 15, 12, 9,
        3, 7, 9, 6, 7, 7, 6, 2,
        -7, -1, 0, 0, -1, 0, -3, -8,
        -17, -15, -13, -14, -12, -12, -11, -22,
        -28, -23, -18, -19, -22, -19, -20, -33,
        -23, -18, -9, -7, -11, -14, -18, -29,
};

int eg_queen_table[64] = {
        1, 15, 19, 24, 33, 30, 26, 27,
        -9, 6, 19, 19, 31, 41, 30, 24,
        -12, 3, 15, 24, 36, 42, 34, 25,
        -20, 2, 8, 18, 30, 28, 25, 20,
        -19, -14, 0, 6, 9, 8, 6, 8,
        -30, -16, -9, -7, -6, 0, -5, -18,
        -29, -27, -17, -25, -19, -24, -38, -39,
        -44, -27, -36, -29, -39, -51, -60, -48,
};

int eg_king_table[64] = {
        -57, 2, 0, -3, 1, 15, 14, -71,
        -9, 9, 8, 8, 11, 24, 28, -11,
        2, 17, 13, 9, 22, 31, 31, 7,
        -5, 15, 13, 12, 14, 23, 20, 3,
        -14, 2, 6, 8, 13, 11, 5, -5,
        -11, -4, 0, 1, 5, 2, 0, -8,
        -9, -3, -7, -7, -8, -3, -1, -7,
        -25, -16, -13, -30, -22, -23, -7, -20,
};

const int mg_piece_values[] = {0, 100, 261, 278, 400, 747, 0};
const int eg_piece_values[] = {0, 105, 212, 235, 373, 681, 0};

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
