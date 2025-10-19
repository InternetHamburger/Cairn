#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        325, 119, 220, 221, 146, 3, 70, 90,
        55, 70, 151, 131, -4, 76, 130, 79,
        -34, -3, 52, -24, -63, -46, -16, -94,
        -64, -47, -34, -52, -68, -114, -111, -98,
        -69, -22, -27, -38, -106, -133, -53, -92,
        -52, -17, -91, -105, -93, -56, -20, -69,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -50, 156, 7, 10, -16, -27, 92, -122,
        -49, -56, 19, 65, 3, 42, -74, -35,
        67, 28, 68, 31, 71, -18, -2, -20,
        49, -22, 104, 53, 80, 48, 32, -26,
        -19, 25, 19, 26, 25, 18, -8, -33,
        -19, 18, -29, 84, 26, 26, -19, -59,
        -25, 3, 53, -134, -70, 15, 18, -63,
        -178, -74, -81, 10, 64, -121, -24, 6,
};

int mg_bishop_table[64] = {
        79, 110, -20, 132, -37, 33, 31, 52,
        -22, 25, 61, 31, 31, -19, 47, 43,
        13, 60, 28, 46, 28, 56, -4, 52,
        -44, 41, 72, 25, 71, 9, -35, -133,
        31, 50, 24, 74, 38, -24, -65, 16,
        53, -25, 56, 18, 18, -33, -3, -15,
        -41, -8, 19, -125, -155, -33, 13, -22,
        -84, 7, -146, -132, -129, -126, -20, -95,
};

int mg_rook_table[64] = {
        36, 43, 84, 49, 85, 65, 39, 5,
        35, 71, 69, 29, 16, 45, 34, -10,
        4, 62, 84, 57, 51, 17, 18, 31,
        64, 31, 50, 8, 19, 21, -42, -13,
        15, -13, -4, 0, 16, -25, -14, -58,
        -42, 6, -6, -8, -48, -92, -95, -52,
        -32, 8, -19, -35, -112, -90, -38, -124,
        -29, -34, -6, -17, 0, -31, -83, -74,
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
        -235, 3, -135, -15, -78, 160, 284, 119,
        -23, 35, 234, 15, 88, 129, 261, 95,
        239, -24, -46, 152, 191, 245, 185, 89,
        -21, -115, -83, 6, 197, 96, 43, 48,
        -183, -151, -19, 20, 14, 65, 24, -98,
        -32, -109, -71, -51, -68, -3, -14, -67,
        -228, 23, -3, -62, -91, 1, 28, -108,
        -311, -207, -204, -196, 1, -68, 66, -57,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        353, 203, 211, 254, 93, -20, 133, 12,
        107, 147, 141, 207, 23, 181, 3, 78,
        -20, 7, 10, 8, -34, -1, -27, -112,
        -85, -41, -13, -37, -64, -108, -99, -140,
        -93, 28, -65, -58, -128, -159, -6, -102,
        -55, 0, -206, -252, -138, -21, -32, -99,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -77, 153, -52, 128, -90, -151, 57, 83,
        -78, 59, 61, 127, 95, 153, -231, 0,
        57, 107, 81, 108, 105, -14, 88, -86,
        85, 24, 116, 72, 109, 119, 64, -30,
        41, 38, 18, 41, 3, 5, -19, -25,
        -35, -42, -43, 88, 35, 20, -25, -117,
        -151, 94, -26, -106, -114, 5, -38, -198,
        -91, -181, -55, -116, -14, -187, -63, 35,
};

int eg_bishop_table[64] = {
        77, 18, 69, 126, 7, -5, 24, 59,
        -12, 29, 82, 29, 116, -8, 47, 108,
        -60, 2, -11, 105, 42, 132, 20, 19,
        -18, 44, 93, 54, 94, -1, -23, -37,
        -20, 97, -1, 72, -5, -8, -84, -17,
        -8, -13, 66, -2, 40, -30, -75, -44,
        9, -59, -12, -80, -168, 54, 49, -305,
        -86, -29, -140, -259, 4, -127, 19, -46,
};

int eg_rook_table[64] = {
        62, 62, 62, 62, 52, 61, 62, 62,
        62, 62, 62, 62, 55, 62, 41, -57,
        62, 62, 62, 62, 62, 62, 56, 62,
        62, 45, 47, 11, 12, -28, -27, 30,
        13, -45, -22, -8, 12, 19, -46, -3,
        -59, -26, -59, -75, -69, -62, -100, -70,
        -20, 11, -51, -79, -74, -104, -45, -207,
        -31, -44, 11, -28, 7, -23, -97, -84,
};

int eg_queen_table[64] = {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, -7,
        1, 1, 1, 1, 0, 1, 0, -8,
        1, -5, 1, 1, 1, 1, 1, 1,
        -12, 1, -6, 1, -45, 1, 1, 1,
};

int eg_king_table[64] = {
        77, 77, 77, -714, 77, 77, 77, 344,
        77, 156, 214, -512, 82, 869, -23, -79,
        -14, 189, -714, -233, -32, -52, 110, -64,
        83, -274, -22, -238, 78, 281, 109, 100,
        -290, 39, 133, 41, 30, 82, 62, -235,
        -33, -157, 4, -11, -69, -1, 65, 89,
        -92, 159, 68, -29, -54, 100, 147, -39,
        -386, -107, -9, -96, 136, 69, 196, 32,
};

const int mg_piece_values[] = {0, 298, 467, 560, 803, 900, 0};
const int eg_piece_values[] = {0, 286, 592, 686, 829, 898, 0};

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
