#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        103, 98, 122, 33, 100, 113, 24, 43,
        43, 39, 19, 9, 44, -39, 89, -44,
        -6, 1, 23, -31, -19, -52, -17, -18,
        -34, -7, 8, -45, 7, -37, -42, -33,
        -33, -17, -40, -60, -31, -32, -57, -47,
        -4, -11, 14, -15, -23, -51, -50, -37,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -176, -3, -106, -144, -25, -70, -36, -102,
        45, 43, -3, 33, 33, -29, -87, -174,
        -45, 72, 92, 36, 24, -3, -48, 31,
        5, 5, 65, 4, 21, 79, 29, -48,
        -1, 57, 30, 17, 54, 9, 83, 34,
        3, 70, 62, -14, 44, 44, -39, 23,
        -60, -34, 48, -24, 34, 61, -6, 0,
        -102, -13, 28, 31, -4, -16, 19, 37,
};

int mg_bishop_table[64] = {
        30, 34, 31, -86, -151, 91, -36, 2,
        81, 86, -9, 32, -4, 67, 1, 23,
        -110, -33, 21, -52, 70, 32, 106, -34,
        -36, 52, 16, 14, -27, -1, -52, -10,
        21, -2, 84, 9, 1, -24, 64, -116,
        -25, 13, 8, 39, 90, 25, -27, 89,
        -1, -7, -22, 46, 68, -207, 26, -112,
        -24, 0, -97, -37, 30, 10, -2, -40,
};

int mg_rook_table[64] = {
        11, 50, 51, 40, -6, -18, 32, 25,
        0, 5, 24, 32, 19, 9, 33, -18,
        12, 22, 33, -6, 44, -5, 43, -47,
        -17, -2, 4, 12, 15, -14, -69, 4,
        2, -94, 47, 54, 11, 37, -45, -32,
        -23, -27, 54, 0, 36, -4, 4, 25,
        -41, -87, 8, -27, -37, -5, 23, -78,
        -7, 0, -20, 18, -14, -26, -20, -50,
};

int mg_queen_table[64] = {
        6, -65, 101, 128, 27, -137, -151, 77,
        -72, 49, 27, 35, 26, 38, 114, -23,
        65, 13, 59, 129, 129, -3, 10, 42,
        3, 28, 57, 106, -76, 129, -141, 87,
        -94, 17, 48, -50, 56, 12, -133, -63,
        -68, 36, 53, 96, 15, 5, 3, -146,
        -160, -183, -6, 23, -16, -4, 48, -76,
        33, 92, 15, -14, -100, -126, -187, 38,
};

int mg_king_table[64] = {
        73, 240, 70, 111, 294, 185, -71, -237,
        269, -157, 319, 141, 37, 93, 54, 112,
        86, 35, 31, 292, -25, -267, -5, -20,
        -57, -73, 96, -20, -5, -30, 0, -46,
        -158, -42, -55, -37, -63, -16, -43, -46,
        -73, -36, 3, -74, -45, -21, -36, -28,
        -51, -25, -107, -36, -56, -36, -28, -4,
        -63, 0, -86, -105, -34, -42, -48, -7,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        90, 53, -113, 93, 11, 86, 223, -48,
        6, 95, 9, 63, -10, -22, 41, 17,
        -24, 13, 1, 14, -21, -32, -16, -12,
        -44, -16, -29, -5, -14, -20, -14, -27,
        -31, -38, -18, -18, 8, -22, -17, -35,
        -41, -11, -7, -20, 1, -20, -44, -42,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -123, 7, -79, -12, 75, 120, 194, 26,
        -74, -78, 44, 77, 151, 185, -162, -50,
        57, 86, 118, -2, 104, 99, -45, 139,
        -221, -25, 66, 75, 25, 122, 4, 13,
        -34, 0, -57, -20, -20, 75, -34, 29,
        -46, -188, -20, -95, 37, 12, 27, 10,
        -27, -69, -42, -84, -33, -9, 51, -143,
        -24, -47, 60, -92, 20, -73, -45, -41,
};

int eg_bishop_table[64] = {
        42, 25, -111, -15, -7, 28, -12, 9,
        93, 41, 42, -57, -31, 333, 9, -30,
        5, 10, 71, 75, -79, -41, 109, 0,
        54, -21, 26, 2, -22, -79, -17, -10,
        -95, -39, 13, 31, 48, 45, 53, -1,
        1, 13, -12, 13, 14, -12, 3, -4,
        4, 30, -39, 0, 16, -378, -13, 35,
        29, -44, -18, -119, -82, -3, 181, -104,
};

int eg_rook_table[64] = {
        97, -33, 75, 29, -40, -98, -44, 0,
        8, 65, 73, 57, -22, 34, -61, -15,
        56, 65, -4, 54, 79, 16, -61, 76,
        -2, -60, 47, 56, -74, 51, -164, -6,
        -46, -49, -9, 18, -17, -9, -94, -33,
        -8, -8, 20, -18, -25, -10, -15, -29,
        -11, 48, 85, 0, 42, -27, -26, -58,
        14, 2, 37, 14, 31, 34, -82, -47,
};

int eg_queen_table[64] = {
        -7, 24, 23, 24, 24, -99, 24, -34,
        23, 23, 24, 24, 24, 24, -42, -49,
        12, 24, 24, 24, 23, -41, 24, -92,
        0, -11, 24, 24, -43, 24, -8, 11,
        24, 24, 24, 24, 24, 2, -8, -51,
        0, 24, 21, 24, 20, -21, -6, -64,
        -13, 23, 24, 0, -19, -65, 24, 18,
        24, 5, -18, 16, 23, -57, 23, -100,
};

int eg_king_table[64] = {
        131, 131, -660, 131, 131, 131, 131, -660,
        131, -660, 131, -660, 131, 131, 923, -660,
        -660, -660, -660, 472, -660, -37, -564, 113,
        -493, 438, 238, 280, -256, 278, -111, 175,
        -77, -9, 36, 146, -75, 48, 193, 116,
        -41, 65, 100, 7, 143, 114, 93, 239,
        204, 117, 28, 169, 131, 168, 158, 138,
        93, 88, 129, 120, 159, 150, 171, 57,
};

const int mg_piece_values[] = {0, 122, 210, 237, 418, 662, 0};
const int eg_piece_values[] = {0, 78, 255, 278, 411, 768, 0};

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
