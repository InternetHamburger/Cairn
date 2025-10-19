#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        284, 196, 137, 258, 94, -2, 188, 93,
        145, 108, 56, 172, 19, 60, 0, 69,
        6, 15, 33, 18, -47, -25, -23, -106,
        -63, -44, -48, -84, -69, -126, -79, -100,
        -26, -17, -18, -44, -127, -156, -60, -109,
        -32, -21, -125, -115, -79, -40, -52, -93,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -43, 70, -8, 137, -4, -47, -52, 30,
        -87, -5, 24, 68, 76, 71, -290, -101,
        41, 78, 73, 80, 117, -24, -45, -39,
        -8, 30, 121, 75, 111, 60, 54, 58,
        8, 50, 67, 60, 35, 31, -24, 15,
        18, -4, -20, 52, 34, 24, -22, -37,
        -133, -66, 102, -110, -40, -90, 1, -127,
        45, -117, -99, -67, 34, -111, -41, -103,
};

int mg_bishop_table[64] = {
        28, 37, 8, 67, -13, 36, 49, 50,
        -13, -7, 63, 9, 96, -3, 45, 88,
        10, 12, -5, 81, -13, 66, -3, -15,
        -23, -11, 57, 17, 45, 2, -17, -4,
        -1, 68, 26, 27, 7, 32, -95, 6,
        -12, 27, 25, -24, -40, -16, 19, -6,
        45, -31, 4, -90, -55, -46, -5, 2,
        -20, 27, -62, -14, -258, -108, -79, -90,
};

int mg_rook_table[64] = {
        62, 61, 51, 61, 48, 28, 52, 30,
        28, 38, 35, 62, 42, 0, 26, -14,
        32, 42, 36, 62, 50, 61, 12, 40,
        49, 52, 28, 51, 4, -14, -50, -40,
        4, 28, 18, 55, -14, 3, -29, -64,
        -27, 16, -8, -27, -58, -57, -94, -23,
        -70, -6, -24, -37, -85, -53, -98, -73,
        -14, -41, 5, -25, -16, -31, -86, -74,
};

int mg_queen_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_king_table[64] = {
        -25, 93, 12, -35, -75, 27, 64, 214,
        55, 61, 106, -96, 14, 194, 106, 129,
        -38, 28, -88, 20, 107, 63, 133, 47,
        -140, -52, -52, -15, 59, 94, 58, 57,
        -211, 28, -21, 0, 37, 68, 27, -18,
        76, -58, -60, -48, -24, 25, 44, 35,
        -267, 53, 10, -19, -78, 13, 35, -17,
        -358, -179, -125, -123, 58, -14, 65, -57,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        494, 91, 294, 399, 337, -513, 30, 131,
        103, 161, 47, 132, 15, -16, 40, 80,
        -32, 32, 40, 7, 3, -8, 69, -112,
        -73, -25, -28, -52, -62, -130, -87, -134,
        -97, 9, -46, -60, -123, -166, 0, -102,
        -69, -23, -128, -165, -120, -18, -15, -92,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -19, -199, -6, 91, 6, -155, 268, 77,
        -43, 46, 7, 32, 41, 142, -26, -63,
        73, 81, 115, 123, 91, 52, 84, -178,
        64, 7, 156, 66, 68, 63, 31, -25,
        -37, 93, 8, 57, 39, 0, -16, 24,
        -40, 8, -22, 78, -3, 24, -31, -89,
        -44, -11, 22, -124, -130, 13, 1, -149,
        -18, -114, -222, -70, 10, -147, -77, -113,
};

int eg_bishop_table[64] = {
        -60, -19, 38, 187, 54, -269, -11, 113,
        45, 3, 116, -33, 66, -112, -28, 119,
        -22, 81, 20, 99, -37, 50, 14, 21,
        51, 53, 72, 65, 46, -29, 3, -102,
        44, 27, 48, 87, 51, 0, -97, 14,
        25, -13, 58, 38, 15, -34, 7, 9,
        -99, -2, 22, -68, -93, -41, 33, 79,
        -55, -24, -117, -235, -100, -127, -14, -59,
};

int eg_rook_table[64] = {
        18, 101, 39, 102, -132, -147, -81, -31,
        33, 102, 102, 102, 102, 102, 101, -124,
        31, 102, 102, 102, 102, 2, 9, 82,
        96, 54, 72, 72, 32, 68, -59, -4,
        49, -67, -17, -106, -29, -30, -1, -16,
        -25, 13, -111, -39, -12, -85, -29, -41,
        -29, 1, -56, -90, -36, -22, -85, -157,
        -4, -43, -11, -26, 32, 4, -132, -67,
};

int eg_queen_table[64] = {
        2, 2, 2, 2, 2, 2, 1, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 1, 2, 1, 2, 2,
        2, 2, -13, -11, -5, 2, 2, 2,
        2, 2, 2, 2, 2, 2, -51, 1,
        2, 1, 1, 2, -55, -25, 2, 2,
};

int eg_king_table[64] = {
        79, 79, 79, 79, 79, 79, 79, 79,
        79, 79, 554, 79, 79, 79, 79, 79,
        871, 79, 79, 120, 514, -76, -97, -82,
        -292, -693, -136, 182, 100, 96, -78, 30,
        -712, -118, 81, -90, 34, 48, -17, -253,
        -304, -196, -109, -27, -70, -7, 40, -22,
        -379, 72, 57, -8, -46, 107, 153, 112,
        -651, -229, -112, -99, 118, 25, 206, 23,
};

const int mg_piece_values[] = {0, 324, 488, 593, 829, 900, 0};
const int eg_piece_values[] = {0, 336, 595, 687, 783, 897, 0};

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
