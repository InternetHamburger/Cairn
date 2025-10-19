#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        155, 199, 277, 166, 40, 44, 168, -28,
        122, 178, 145, 136, -30, 76, 21, 31,
        -6, 8, 16, -18, -68, 15, -50, -60,
        -56, -48, 4, -39, -73, -86, -97, -91,
        -76, 38, -54, -14, -82, -117, -24, -75,
        -36, 0, -163, -234, -101, -46, -18, -64,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        67, 173, -38, -50, 163, 168, -92, 109,
        -6, 152, 106, 44, 21, 198, -55, -282,
        101, 43, 79, 91, 142, -86, 57, -60,
        101, 47, 118, 104, 131, 67, 96, -61,
        104, 44, 50, 40, 24, 36, 20, 17,
        27, 0, -5, 173, 101, 54, 39, -45,
        10, 245, 35, -47, -32, 45, -180, -93,
        -1075, -120, -443, -90, -22, -78, 31, -502,
};

int mg_bishop_table[64] = {
        161, -184, -18, -7, 228, 66, -63, 70,
        -74, 91, 37, 8, 91, -57, 71, 62,
        -40, 41, -47, 55, -4, 75, 73, 41,
        -46, 49, 99, 41, 97, 39, 30, -11,
        -28, 150, 23, 108, 26, 14, -32, -11,
        24, 1, 104, 23, 85, -6, -55, -1,
        -46, -71, 0, -125, -117, 91, 78, -395,
        -103, 8, -135, -108, -286, -75, 88, -223,
};

int mg_rook_table[64] = {
        46, 14, 16, 25, 5, 32, -2, 69,
        71, 36, 54, 11, -1, 59, 40, 30,
        60, 80, 50, 14, 47, 42, 55, 5,
        62, 9, 7, 43, 2, -20, -8, 31,
        -4, 10, 10, 50, -29, -6, -121, -55,
        -94, -44, -11, 24, -28, -63, -132, -24,
        -10, 19, -23, -8, -59, -60, -28, -105,
        -48, -56, 20, -14, -6, -39, -16, -58,
};

int mg_queen_table[64] = {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, -23, -2, 1, -34,
        1, 1, 0, -24, 0, 1, 1, 1,
};

int mg_king_table[64] = {
        -77, 29, -82, -22, -30, 30, -53, 89,
        89, 22, -11, -6, -12, 9, -41, 56,
        21, -13, 64, 13, 83, -58, 88, 103,
        4, 15, 25, 40, 48, 80, 88, 12,
        -19, 7, 7, 52, 24, 21, 49, -42,
        -45, -48, 12, -47, -12, 12, 28, -30,
        -165, 123, 18, -72, -69, -4, 26, -38,
        -155, -90, -83, -109, 12, -14, 45, -30,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        352, 3, 74, 102, 272, -220, -85, -145,
        -48, 24, 121, 95, 2, 105, 281, 122,
        -81, 17, 54, -12, -6, -18, 32, -120,
        -63, 14, -17, 8, -31, -69, -49, -46,
        -51, 0, -17, -24, -61, -80, 40, -60,
        -86, 3, -82, -87, -123, -17, 35, -37,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -79, 339, -182, -33, -135, -147, 399, 9,
        -126, -292, 13, 78, -43, 68, -203, 157,
        143, 12, -39, -10, 66, -210, 0, 63,
        96, -4, 202, 87, 90, 66, 10, -24,
        -47, 83, 32, 25, 31, 37, -30, -25,
        -68, 80, -6, 47, 37, 31, -20, -79,
        -48, -173, 41, -58, -73, 35, 70, 79,
        -65, -80, -313, 130, 202, -200, -66, 0,
};

int eg_bishop_table[64] = {
        108, 351, -160, 218, -291, 89, 162, 207,
        -64, -8, -144, 84, 55, 25, 95, 61,
        -5, -21, 123, 36, 139, 90, -2, 3,
        81, 1, 82, 59, 41, 31, 44, -166,
        0, 53, 2, 0, 63, -75, -3, 42,
        -23, -1, 38, 7, -18, -19, 94, -7,
        -38, -40, 55, -40, -40, -11, -10, -485,
        -215, -42, -54, -76, -217, -42, -41, -169,
};

int eg_rook_table[64] = {
        -25, 85, 202, 111, 225, -71, -5, -69,
        -22, 110, -49, 5, -12, 101, 86, -42,
        -8, 120, 167, 141, 58, 97, -192, 145,
        95, 21, 42, -25, -20, -76, -18, -35,
        0, -59, -35, -66, 77, 38, 73, 68,
        -90, 94, -42, -164, -22, -67, -83, -3,
        -101, 51, -60, -38, -143, -45, 18, -139,
        -42, -53, -61, -29, 11, -43, -147, -61,
};

int eg_queen_table[64] = {
        12, 12, 11, 11, 12, 12, 12, 12,
        12, 11, 12, -25, -56, 12, 12, 12,
        11, 12, 12, 12, 12, 12, 12, 12,
        12, 12, -15, 11, 7, 12, 12, 12,
        -23, 12, -37, 10, 12, -58, 12, 12,
        -100, 12, 12, 12, 11, 12, -3, 12,
        6, -53, 12, 12, 12, 12, 12, -38,
        12, -19, 12, 0, 3, 3, -17, -122,
};

int eg_king_table[64] = {
        20, 20, -771, 20, 20, 20, 812, 20,
        20, -203, 812, 20, 20, 20, 812, 20,
        812, -741, -771, 301, 311, 812, 431, 20,
        -149, -742, -441, -206, 493, -61, 14, 124,
        -82, -173, 89, 66, 9, 122, 78, -200,
        150, -218, -60, -1, -195, -23, -1, -42,
        -199, 93, 26, -82, -115, 35, 89, -93,
        -771, -199, -141, -112, 0, -45, 104, -5,
};

const int mg_piece_values[] = {0, 174, 324, 411, 671, 900, 0};
const int eg_piece_values[] = {0, 167, 445, 502, 644, 888, 0};

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
