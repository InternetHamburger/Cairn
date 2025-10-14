#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        168, -65, 121, 260, 91, 237, 221, 80,
        12, -50, 26, 112, 103, -5, 109, 81,
        -42, 15, -2, -73, -27, -80, -49, -10,
        -54, -26, -84, -83, -22, -43, -82, 15,
        -57, -43, -53, -71, -74, -54, -17, -53,
        29, -46, -61, -104, -148, -71, -32, 8,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -94, -44, -185, -85, -65, -67, -158, -170,
        55, 27, 10, -105, -46, -4, -12, 128,
        13, -39, 108, 138, 6, -66, -38, -78,
        33, 114, 81, 105, -66, 108, 32, -77,
        0, 32, 84, 105, 39, -32, 76, -30,
        -3, -64, 82, -31, -20, 5, 108, -90,
        24, -77, 474, 109, 62, -55, -493, 33,
        158, 85, -1, -102, 169, -71, 17, -130,
};

int mg_bishop_table[64] = {
        10, -166, -72, 111, -66, 42, 184, 89,
        -90, -40, -127, -66, 0, 137, 130, 100,
        -41, 190, 3, -32, -231, 30, -17, 15,
        4, -11, -50, 1, 64, 38, 72, 46,
        -119, 51, 0, 95, -15, -5, 65, -67,
        -40, 69, -31, -35, -45, -37, -182, -61,
        11, -19, 93, 37, 97, -94, -40, 160,
        -106, -132, 58, 72, -94, 17, 110, -60,
};

int mg_rook_table[64] = {
        18, -33, -24, 33, 41, -51, -13, 80,
        67, 43, -4, 11, 25, 58, 24, 16,
        -20, 4, 19, 25, -43, 136, -22, -42,
        36, 44, 9, -10, 66, 0, 42, -10,
        -12, 55, -4, -21, -53, 7, -33, -23,
        15, 21, -88, 7, 12, -36, 10, 6,
        4, -33, -29, 30, 21, -28, -123, -39,
        -20, -51, -6, 17, -3, -20, -64, -20,
};

int mg_queen_table[64] = {
        82, 123, 123, 81, -103, 123, -106, 38,
        123, 123, 80, -222, 123, 90, 123, 123,
        99, 73, 105, -2, 59, 66, 123, 121,
        -24, 39, 80, 80, 105, -142, 71, -282,
        123, 113, 81, 103, 68, -113, 105, -153,
        -263, -135, 71, -224, -9, -51, -1, -178,
        37, -2, 54, 86, -157, -235, -75, -172,
        37, 40, -149, 122, -3, -264, -48, -278,
};

int mg_king_table[64] = {
        -137, 19, -102, -68, -35, 66, -30, -187,
        -85, 10, 104, -59, 117, 80, -14, -183,
        -100, 16, 50, 115, 48, 113, -25, -69,
        -36, 18, 84, 71, 130, 30, -5, -109,
        -280, 31, 120, 57, -2, 4, -7, 22,
        8, 81, 97, 14, 23, -19, -51, 5,
        -18, 40, 5, 31, -56, 37, 11, -13,
        -74, 25, 13, -112, 21, -32, -5, 168,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        -83, 292, -133, 304, 187, 35, 214, 227,
        -13, -73, 104, 55, 43, 71, 203, -179,
        -40, -25, -36, -17, -10, -40, -55, -95,
        -40, -54, -85, -5, -50, -42, -53, -79,
        -18, -46, -63, -14, -31, -51, 34, -51,
        -28, -29, -48, -28, -66, -31, 11, -67,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        33, 186, -43, 506, 225, -219, 420, -139,
        52, -15, 121, -417, 148, 83, 45, -89,
        422, 23, 90, 15, -62, 116, 116, -187,
        -27, -38, -48, -29, -44, 248, -17, 29,
        44, -121, -81, -35, -57, -26, 259, 24,
        -39, -149, -50, -89, -46, -26, -158, -52,
        -103, -69, 19, -87, -121, -19, 506, -59,
        -130, -70, -150, -255, -218, -115, -50, 41,
};

int eg_bishop_table[64] = {
        10, -16, -121, 414, -447, -36, 221, -60,
        38, -33, -61, -67, -2, -476, -104, 37,
        -39, 322, 4, -64, 7, -44, -54, 27,
        6, 62, 65, -36, -21, -73, 15, 8,
        -118, 92, -42, 118, -54, -2, 0, 44,
        71, 53, -28, 4, 25, 42, -54, -4,
        273, -66, 95, 32, 21, -269, 38, 528,
        89, 132, -7, -116, -160, 36, -263, 3,
};

int eg_rook_table[64] = {
        199, -34, -374, -133, 167, 268, 71, 65,
        152, 29, 11, -112, 5, 72, -1, -4,
        4, 12, -96, 199, -77, -1, -17, 62,
        35, 98, -66, -48, -135, 77, 62, 124,
        13, -72, 16, -85, 10, -16, -38, 56,
        23, 36, -39, -58, -8, 20, -46, -85,
        -51, -57, -82, -83, 13, -31, -13, 73,
        -17, -51, -28, -12, 14, -6, -29, -2,
};

int eg_queen_table[64] = {
        31, 31, 31, 31, 31, 31, 31, 1,
        24, -23, -7, -6, 31, 29, 30, 31,
        -35, -64, 19, 31, 13, 31, -11, 22,
        -55, -94, 31, -101, 31, 14, 29, 11,
        -23, -4, 31, 31, 4, 30, 31, 31,
        25, 11, -75, -32, 19, -16, 29, 31,
        31, -69, 15, -27, 0, -21, -91, 31,
        -53, -24, -47, -19, 31, 31, -97, -2,
};

int eg_king_table[64] = {
        78, 78, 78, 78, 78, 78, 78, 78,
        78, 251, -713, -713, 78, 78, 55, -219,
        -713, 78, -238, -704, 34, 320, 15, -43,
        -713, 9, -183, 133, -6, 112, -105, -4,
        344, 133, -28, -87, -180, 42, 193, 147,
        168, 107, -76, 26, 169, 54, 22, 210,
        249, 66, -8, 66, 81, 108, 85, -116,
        170, 37, 82, -14, 94, 164, 161, 11,
};

const int mg_piece_values[] = {0, 148, 131, 225, 427, 669, 0};
const int eg_piece_values[] = {0, 108, 286, 263, 411, 761, 0};

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
