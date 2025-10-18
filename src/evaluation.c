#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        50, 56, 72, 45, 24, 39, 66, 19,
        4, 60, 19, 45, 27, 27, -2, 1,
        0, 1, -12, -9, -1, -20, -23, -34,
        -4, -8, -19, -13, -25, -36, -39, -40,
        -9, -5, -22, -19, -28, -35, -32, -41,
        4, -4, -6, -9, -13, -15, -12, -25,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -44, -4, 10, 5, 18, -7, -28, -29,
        -33, -7, 5, 32, 27, 15, -8, -11,
        -19, 17, 19, 35, 22, 23, 15, -5,
        -11, 8, 6, 20, 33, 30, 9, 6,
        -10, -10, 17, 6, 12, 16, 0, 2,
        -11, -19, 4, -4, -4, 5, 12, 6,
        -1, -44, -22, -16, 21, 3, -4, 5,
        -40, 0, -16, -20, -6, 9, -30, -11,
};

int mg_bishop_table[64] = {
        -4, -2, -7, 12, 8, 7, 19, -15,
        -3, 4, 7, 8, 12, -2, 17, -20,
        7, 15, 4, 5, -4, 20, -11, 14,
        4, -3, -5, 7, 13, -5, -3, -17,
        -1, 18, 10, 13, 2, 13, -14, -19,
        12, -3, 7, -11, 0, -15, -12, 0,
        -6, -5, -12, -8, -5, 3, 15, 3,
        -16, -2, 8, -11, 3, -6, -58, 16,
};

int mg_rook_table[64] = {
        0, 16, 22, 23, 20, 5, 8, 22,
        4, 3, 9, 27, 7, 28, 23, 10,
        -6, 1, 1, 11, 10, 11, 8, 14,
        -3, -3, 1, 8, 7, 9, -14, 1,
        -6, -9, -5, -5, -5, -1, -4, -7,
        -7, -9, -13, -11, -9, -8, 13, -16,
        -13, -14, -14, -10, -3, 9, -17, -27,
        -13, -8, -11, -6, 0, -7, -30, -18,
};

int mg_queen_table[64] = {
        6, 6, 6, 6, 6, 6, 6, 6,
        6, -7, 6, 6, 6, 6, 6, -1,
        6, 6, 6, 6, 6, 6, 6, 6,
        6, -5, -9, 2, 6, 6, 0, 6,
        6, 5, -10, 6, -2, 6, -9, 0,
        -11, -9, -9, 2, -1, -1, 0, -4,
        3, -8, -7, -4, -13, 6, -7, -6,
        -15, 2, -16, 6, -7, -3, -39, 0,
};

int mg_king_table[64] = {
        -20, 37, 59, 36, -1, 7, 65, -98,
        51, -42, 62, 91, 66, 45, 19, 47,
        31, -47, 80, 79, 17, 57, 72, -85,
        12, -62, -26, 4, -1, -47, -42, -39,
        -39, -67, -55, -10, -12, -59, -28, -64,
        -51, -23, -9, 6, -32, -23, 0, 9,
        -4, -3, -11, -2, -7, 6, 8, 13,
        -37, 16, 14, -13, 17, 3, 10, 18,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        110, 60, 60, -17, 26, 10, 80, 72,
        18, 28, 22, 36, 24, 62, 16, 22,
        -20, 2, -10, -3, -3, -11, -6, -36,
        -31, -21, -24, -12, -17, -23, -29, -36,
        -29, -15, -26, -25, -20, -26, 13, -36,
        -39, -19, -23, -34, -32, -5, 10, -41,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -81, -93, -87, 40, 103, -57, -48, -8,
        -38, -21, 23, 49, 41, 73, 1, 5,
        1, 56, 26, 80, 83, 67, 41, -8,
        -17, 9, 4, 34, 43, 43, 20, 25,
        -19, -11, 17, 2, 19, 17, 5, -7,
        -35, -44, 2, -6, -14, -8, 19, -7,
        -31, -46, -48, -28, -2, -10, -37, -19,
        -30, -38, -38, -29, -20, -31, -35, 102,
};

int eg_bishop_table[64] = {
        6, -29, -40, 49, 9, -6, -40, -5,
        6, -3, 17, -8, 10, -21, 11, 6,
        -18, 30, 9, 39, 16, 55, -2, 26,
        1, -1, 15, 14, 18, 10, -1, -22,
        -15, -1, 12, 22, 8, 7, -26, -43,
        -3, 1, -4, -9, -6, -4, -11, -1,
        1, -1, -12, -5, -6, 5, 12, -12,
        1, 6, -11, -58, 2, -28, 35, -6,
};

int eg_rook_table[64] = {
        24, 64, 65, 82, -38, 63, 60, 39,
        8, 5, -10, 22, 13, 56, 11, 16,
        18, 17, 32, -14, 62, 32, -43, 24,
        -12, -11, -3, -12, 18, -20, -1, -10,
        -15, -3, -29, -4, 0, -3, -15, -4,
        -31, -19, -40, -29, -10, -21, -6, -30,
        -25, -11, -6, -8, -5, -3, -39, -61,
        -23, -11, -17, -8, 6, -9, -48, -30,
};

int eg_queen_table[64] = {
        6, -14, 6, 6, 6, 6, -17, 6,
        2, 4, 6, -3, 6, 6, 6, -9,
        6, -8, 6, 6, 6, 6, 6, 6,
        3, -3, 2, 3, 6, -6, 2, 6,
        6, -9, 6, 2, -1, -1, 4, 6,
        2, 1, -9, 4, 4, 6, -8, -10,
        -23, 5, 6, 5, 3, -1, -8, -5,
        6, -20, -1, 6, -7, -3, -50, 6,
};

int eg_king_table[64] = {
        32, 52, 58, 48, 32, 44, 193, -115,
        49, -111, 140, 171, 128, 90, 56, 178,
        18, -108, 164, 146, 31, 112, 171, -113,
        -86, -284, -154, 148, -216, -144, -250, -230,
        -31, -19, -83, -33, -147, -153, -95, -261,
        -14, 25, -30, -4, -29, -24, 14, 27,
        40, 31, 1, 12, -1, 28, 48, 75,
        16, 58, 52, -6, 60, 40, 70, 80,
};

const int mg_piece_values[] = {0, 100, 300, 320, 500, 900, 0};
const int eg_piece_values[] = {0, 82, 300, 320, 500, 900, 0};

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
