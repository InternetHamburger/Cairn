#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        113, 122, 110, 84, 51, 21, 56, 45,
        32, 70, 57, 39, 31, 0, 23, 48,
        6, 9, -2, -16, 19, -14, 2, -22,
        -44, -11, -17, -6, -26, -58, -44, -54,
        -8, -13, -6, -43, -38, -95, -23, -37,
        -33, -2, -77, -82, -101, -26, -14, -30,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -11, -66, 9, 25, 49, -77, -47, -91,
        -50, 21, 16, 20, -8, 22, -27, -71,
        15, 50, 35, 36, 44, 18, 23, -37,
        30, 30, 14, 43, 48, 38, 12, 35,
        -13, 7, 36, 5, -10, 31, 7, -6,
        13, -11, -3, -3, 41, 14, 3, -1,
        45, 6, 0, -5, -6, -17, -106, -27,
        -78, -26, 69, -30, -1, -16, 4, -67,
};

int mg_bishop_table[64] = {
        40, -18, 12, 6, 11, 32, -8, 7,
        -12, 1, 29, 17, -5, -17, 20, 18,
        -2, 22, -23, 4, -15, 39, -2, 14,
        13, 17, 3, 18, 43, 0, 15, 6,
        10, 24, 24, 22, 16, -27, -35, -6,
        15, 24, 31, 32, -6, -53, -20, 20,
        43, -22, -6, -69, -30, 8, 33, -109,
        -43, 5, 6, -16, -35, -35, -78, -10,
};

int mg_rook_table[64] = {
        16, -5, 15, -6, -5, 6, -31, -6,
        31, 20, 20, 17, -2, 16, 16, 19,
        11, 11, 14, 4, 0, 10, 5, 0,
        14, 27, -17, 9, 18, 10, 16, 7,
        -2, 8, 6, 18, -17, 7, -1, -14,
        11, -18, 0, -6, 19, -36, 7, -30,
        8, 15, -5, 7, -39, -9, -35, -40,
        -12, -11, -10, -1, -3, -11, -30, -40,
};

int mg_queen_table[64] = {
        -2, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        -5, 1, 1, 1, 1, 1, 1, 1,
        1, -1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, -1, 1, 0, 1,
        1, -2, 1, 1, 1, -3, -15, 1,
        1, 0, 1, 1, -7, -4, 1, 1,
};

int mg_king_table[64] = {
        -56, -1, 14, -2, -100, 58, 18, -23,
        -142, -49, 9, -66, 6, 58, 78, 0,
        -48, -6, -16, 4, -25, 25, 27, 26,
        24, 62, 10, -25, 49, 30, 27, -7,
        -9, 8, 13, 34, -2, -14, 16, -15,
        -1, -28, -2, -9, 4, 6, 4, 13,
        50, -1, 9, -30, -24, 3, 17, -5,
        -25, 6, -26, -17, 29, -1, 42, -5,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        42, -35, 150, 117, -29, 104, -56, 130,
        14, 49, 40, 69, 48, -14, 13, 34,
        -11, 23, 7, 22, 11, -6, -6, -17,
        -44, -13, -14, 6, -18, -39, -28, -46,
        -28, -21, -14, -31, -46, -67, -6, -38,
        -42, -3, -43, -55, -71, -13, 9, -32,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -16, -122, 50, -2, -73, -112, -147, -4,
        -33, 18, 0, -23, 55, 42, 41, 7,
        70, 45, 81, 72, 72, -19, 71, -2,
        -28, 24, 29, 69, 76, 42, 51, 57,
        12, -2, 48, 38, 29, 28, 5, 4,
        -7, 2, 20, 15, 17, 41, 10, -8,
        6, 3, 18, 14, 8, 12, -61, -46,
        -377, -9, -92, -1, -71, 12, -17, -40,
};

int eg_bishop_table[64] = {
        87, -132, -159, -129, -69, 55, 25, 50,
        -163, -2, 14, -50, 42, 18, 3, 3,
        11, 28, -32, -1, 12, 31, -45, 49,
        0, 33, 35, 42, 69, 53, 33, 18,
        23, 51, 44, 40, 40, 29, -17, -28,
        14, 14, 57, 38, 23, -6, -44, 29,
        67, 5, 28, 14, -12, 21, 39, -59,
        -10, 49, 6, -14, -28, -11, -300, -28,
};

int eg_rook_table[64] = {
        -97, -66, -157, -95, -102, 70, -171, -7,
        65, 62, 71, -74, -86, -76, 8, 10,
        40, 72, -16, 72, -78, -3, 62, 58,
        59, 74, -57, 48, 68, 59, 40, 59,
        21, 1, -16, 52, -85, 36, 12, 36,
        3, -6, 5, -1, 35, -38, 18, -18,
        21, 60, 31, 4, -15, 18, -35, -79,
        15, 20, 14, -11, 16, 13, -32, -2,
};

int eg_queen_table[64] = {
        5, -3, 11, 11, 11, 11, 11, 11,
        11, 11, 11, 11, 11, 11, 2, 11,
        11, 5, 11, -11, -28, 11, -49, 4,
        -1, -6, 11, 11, 11, 11, 2, 6,
        11, 6, 5, 6, -8, -5, 7, 10,
        -53, 11, 11, 10, 7, 11, 9, -1,
        -32, 5, 7, 8, 9, -51, -21, 11,
        2, -39, 11, 11, -35, -21, -59, 11,
};

int eg_king_table[64] = {
        -17, 33, 26, -48, -17, 43, 46, 24,
        -105, -74, -7, -79, 32, 50, 74, 19,
        -68, 4, -60, -7, -61, 36, 29, 46,
        39, 109, 113, -209, -75, -106, 66, -96,
        -12, 29, -34, -63, -148, -52, 43, -54,
        -9, -56, -34, -113, -47, -4, -2, 117,
        240, 72, 15, -15, -30, 28, 63, 82,
        90, 156, -155, -56, 51, 18, 101, 20,
};

const int mg_piece_values[] = {0, 100, 153, 178, 256, 296, 0};
const int eg_piece_values[] = {0, 85, 179, 195, 223, 286, 0};

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
