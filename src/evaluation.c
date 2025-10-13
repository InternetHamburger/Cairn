#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        152, 211, 168, 233, 138, 144, 146, 79,
        32, 66, 82, 101, 45, -9, 37, 49,
        -13, 15, -34, -8, -23, -19, -37, -49,
        -77, -22, -35, -46, -66, -80, -71, -61,
        -69, -24, -38, -36, -74, -94, -62, -77,
        -62, -24, -79, -181, -27, -81, -21, -91,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -186, -35, -28, -25, 10, -152, -55, -105,
        -59, -30, 33, 32, 12, -34, -70, -105,
        -58, 4, 46, 25, 73, 27, 56, -33,
        -22, -8, -6, 62, 58, 10, -7, 53,
        5, -10, 42, 14, 55, 36, -65, 67,
        31, -59, 64, 65, 36, 53, 73, -72,
        5, 118, 11, 22, -11, 48, 127, 24,
        -152, -45, -56, 101, 45, 14, -66, -19,
};

int mg_bishop_table[64] = {
        -21, -2, -40, 16, -58, 97, 6, -64,
        -14, 48, -11, 40, 41, -7, 25, -44,
        -8, 112, -33, 27, -21, 12, -14, 19,
        6, 29, 36, 37, 25, -7, -12, -27,
        14, 55, 39, 38, 23, -36, -87, -11,
        31, 34, 27, -23, 6, -21, -30, 56,
        43, -12, -25, -82, -34, 34, -19, 4,
        7, -28, -52, -55, -13, -102, 47, -33,
};

int mg_rook_table[64] = {
        45, 13, 36, 16, -14, 27, 12, 42,
        6, 10, -8, 25, 21, 49, 3, 13,
        21, 21, 47, 15, 9, 35, 3, -1,
        16, -4, 24, 32, 14, -1, -6, -4,
        13, -16, 8, 2, 5, 9, -31, -24,
        -44, -22, 40, -15, -26, -43, -82, -69,
        -20, 27, -4, -10, -2, -7, -29, -59,
        3, -8, 1, 1, 5, -9, -58, -83,
};

int mg_queen_table[64] = {
        -90, 29, 31, -3, 31, 14, -51, -38,
        -4, 31, 29, 31, 31, 31, 31, 31,
        17, 4, 31, 31, 31, 31, 31, 31,
        2, -6, 31, 31, 30, 31, 31, 31,
        30, -55, 29, 0, 31, 31, 10, -46,
        -111, -47, 0, 31, 30, 19, 21, 0,
        30, -29, -1, -83, 3, -40, -122, -94,
        -26, -8, 8, -14, -6, -86, -11, -48,
};

int mg_king_table[64] = {
        -9, -1, 78, 17, -37, -20, 0, -15,
        0, 25, 79, -5, 23, 12, 31, -20,
        6, 29, 47, 44, 37, 98, 61, -38,
        43, 67, 53, 15, 49, 32, -2, -3,
        3, 13, 64, 61, 30, -17, -1, -49,
        0, -25, 26, -13, 4, -6, -16, -30,
        -52, -11, -7, -41, -24, -20, 5, -49,
        -101, -125, -119, -73, -24, -36, -7, -47,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        201, 25, 115, 72, 58, 239, 63, 270,
        -22, 189, 31, 151, 50, 14, 116, 37,
        -33, 18, 4, 25, 49, -19, 41, -80,
        -88, -39, -42, 20, -38, -71, -43, -97,
        -64, -41, -45, -67, -93, -165, -8, -69,
        -107, -22, -117, -138, -168, -27, 10, -93,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -138, -116, -36, 184, -255, -223, 383, -25,
        103, 35, 41, 130, -29, 91, -121, -86,
        -80, 49, 84, 110, 66, -27, 94, -16,
        71, 11, 7, 110, 111, 102, 63, 49,
        -31, -12, 26, 4, 15, 32, 52, 24,
        -61, -40, 1, 31, 94, 48, 18, -49,
        54, 22, 28, -4, 3, -1, -134, -40,
        -71, -90, -128, -133, -60, -94, -51, -202,
};

int eg_bishop_table[64] = {
        -8, -91, 22, 68, -189, -56, 39, 28,
        -46, -62, 56, 32, 70, -53, -21, 0,
        -26, 78, -41, 72, -26, 127, 38, 12,
        39, 15, 58, 76, 69, 38, 23, 1,
        -19, 53, 51, 11, 0, -3, -22, -13,
        -6, 18, 29, 15, -10, -6, -84, 34,
        169, -17, 53, -37, -21, 15, -1, -63,
        -33, 80, -69, -14, -118, -61, -207, -48,
};

int eg_rook_table[64] = {
        52, 207, 169, 146, 76, -66, -99, 54,
        33, 37, 87, -23, 121, 206, 79, -12,
        -2, 58, 55, 51, 25, 160, 17, 1,
        -20, 42, -3, 1, 52, 86, 55, 39,
        -27, 22, 19, -71, -20, 13, -15, -34,
        -20, -47, -16, -86, -52, -92, -23, -77,
        -47, -57, -132, -63, -111, -120, -31, -204,
        -43, -44, -19, -41, -6, -31, -100, -88,
};

int eg_queen_table[64] = {
        7, 8, 8, 8, 8, 8, 8, 8,
        -14, 8, 8, 8, 8, 8, 8, -30,
        8, 8, 8, 8, 8, 8, 8, 8,
        5, 8, 7, 8, 8, 8, 2, 8,
        2, 6, 8, 8, 8, 7, 8, 8,
        -75, 8, -1, 8, -25, 8, 8, 8,
        7, 0, -9, 0, 8, -10, -33, 8,
        -45, -18, 8, 8, -6, 7, 7, -115,
};

int eg_king_table[64] = {
        63, 63, 63, 63, 63, 63, 63, 855,
        63, -728, 63, 63, -728, -192, -84, 63,
        500, 148, -138, -381, -728, 207, -27, -1,
        379, -3, 259, 74, -626, -138, 316, 25,
        25, 22, -159, -61, -114, 4, 39, 79,
        -35, 14, 74, -30, -125, 15, 32, 73,
        -95, 219, 64, -57, -44, 31, 99, 48,
        -76, 18, -105, -23, 104, 35, 172, 83,
};

const int mg_piece_values[] = {0, 162, 297, 359, 482, 760, 0};
const int eg_piece_values[] = {0, 176, 408, 457, 585, 784, 0};

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
        eg_eval += mg_piece_square_values[board->squares[square]][square];
    }

    // Interpolate based on number of pieces
    int num_pieces = __builtin_popcountll(occupied);
    int eval = (mg_eval * num_pieces + eg_eval * (32 - num_pieces)) / 32;

    return eval * (board->white_to_move ? 1 : -1);
}
