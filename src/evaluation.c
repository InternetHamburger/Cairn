#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        175, 77, -2, 122, 93, -60, 191, 106,
        80, 61, 23, 66, 6, -36, 2, 29,
        43, 19, 35, 9, -56, -44, -38, -57,
        0, -27, -9, -65, -28, -81, -27, -48,
        22, -29, 14, 16, -54, -77, -64, -50,
        -18, -38, -89, 53, -33, -73, -59, -55,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        0, -63, -8, 40, -11, 65, 76, -20,
        -35, -73, -13, 22, 48, 62, -213, -20,
        -53, -20, 24, 8, 61, 0, -62, -48,
        -32, 33, 37, 22, 44, 38, 57, 54,
        -28, 17, 46, 48, 46, 16, -12, 7,
        45, 38, -24, 11, 51, 17, -1, 54,
        -24, -24, 89, -43, -33, -140, 33, -131,
        91, -28, 4, -43, 15, -35, -26, -72,
};

int mg_bishop_table[64] = {
        -6, 3, -5, 37, -65, -44, 57, -9,
        -46, 1, 24, 4, 102, -23, -64, 51,
        27, -1, 0, 48, 1, 53, 23, -100,
        -117, -20, -2, 26, 19, 51, 27, 17,
        -17, 22, 25, 14, 32, 43, -50, 36,
        9, -34, 21, 42, -45, 51, 42, -23,
        59, -36, -44, -39, 39, -87, -28, 184,
        22, 27, 57, 73, -209, -49, -94, -91,
};

int mg_rook_table[64] = {
        8, 9, 18, 6, -3, 48, 0, 37,
        3, 4, 7, 16, 7, 1, 8, -20,
        -6, 29, 11, 26, -1, 52, 0, 12,
        5, 20, -14, 0, -12, -30, -60, -43,
        -49, 7, 24, 34, -31, 17, -24, -48,
        -38, -2, 9, -9, 14, 16, -33, 24,
        -51, 45, 15, 25, -8, 41, -52, 13,
        -11, -19, 16, 19, -5, -6, -12, -49,
};

int mg_queen_table[64] = {
        -2, 20, 20, 19, 20, 20, 20, 20,
        20, -66, 20, 20, 20, 20, -7, 20,
        -88, -9, -14, 20, 20, 19, 20, 20,
        3, 20, 20, 20, 20, 20, -38, -40,
        -39, 20, 20, 20, 17, 20, 19, -59,
        20, 19, -23, -9, -73, -58, -67, -42,
        19, 20, 20, 19, 20, 20, 19, -99,
        20, 20, -13, -4, -75, 16, -30, 19,
};

int mg_king_table[64] = {
        353, 62, -124, -51, 115, 38, -7, -97,
        87, -18, -63, 67, 46, -5, 37, 38,
        -169, -80, 35, 9, 44, 91, 46, 53,
        -99, -32, -61, 51, 31, 23, 57, -14,
        -53, 59, 9, 10, 26, 14, 6, 10,
        79, 6, -5, 1, 7, 10, 22, -10,
        -42, -5, 16, -3, -61, -1, 2, -2,
        -264, -59, -85, -57, 9, -18, -18, -57,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        342, 105, 158, 200, 430, -400, 270, 90,
        -16, 150, -28, 2, 9, -136, -50, -21,
        -35, 1, -27, -7, -4, -22, 23, -58,
        -15, -27, -14, -38, -48, -55, 1, -53,
        -46, -11, -33, -34, -41, -75, -15, -51,
        -52, -46, -44, -67, -41, -32, -45, -72,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        19, -181, -32, 39, -173, -5, 356, 46,
        -9, 45, -11, -111, 0, 100, -56, -63,
        6, 17, 20, 51, 49, 96, 49, -141,
        -16, 26, 62, 32, 7, -17, 6, 0,
        -36, 79, -15, 37, 33, -4, 42, 62,
        -21, -17, 8, 44, -36, 13, 25, -14,
        135, 3, 2, -75, -40, 28, 26, -76,
        -26, -23, -157, -34, -6, 24, -42, -158,
};

int eg_bishop_table[64] = {
        -116, -68, 104, 97, 308, -305, -17, -71,
        47, -26, 125, -63, 36, -24, -51, 128,
        -22, 7, -20, 25, -74, 17, -36, 9,
        41, 8, 15, 57, -17, 24, 13, -45,
        54, -45, 13, 56, 60, 19, -81, 45,
        -45, 40, -7, 19, -23, -25, 1, 1,
        -4, -13, 26, -34, 3, -63, -10, 81,
        72, -57, -26, -150, 17, -40, 0, 25,
};

int eg_rook_table[64] = {
        37, 97, 34, 137, -160, -117, -25, -1,
        -37, 130, 47, 109, 111, 248, 132, -174,
        33, 111, 52, 96, 105, -52, -80, 68,
        1, -63, 87, 72, 47, 13, -43, -55,
        8, -93, -21, -130, -30, 20, -17, -28,
        0, -15, -48, -22, -31, -84, 35, -12,
        -54, 7, -27, -59, 16, 51, -77, -74,
        -18, -42, -21, -31, 11, -4, -104, -74,
};

int eg_queen_table[64] = {
        22, 22, 22, 22, 22, -107, -147, 22,
        22, 21, 22, -43, 22, -1, 22, 22,
        -48, 22, 22, 15, 20, -65, 22, 22,
        22, -38, 22, 0, -50, -29, -21, 22,
        -3, 0, -5, -7, -9, -15, -8, 7,
        22, -31, 10, -31, 21, 22, 22, 22,
        20, 8, 22, 22, 20, 22, -145, 21,
        22, 10, 10, 22, -30, 22, 22, 22,
};

int eg_king_table[64] = {
        77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 171, 77, 77, 77, 77, 77,
        869, 77, 77, 56, 235, 301, -519, -714,
        -428, -714, -196, 386, 363, 0, -713, -72,
        -714, 122, 111, 28, 98, -30, 63, -190,
        -253, 22, -69, 60, 27, 47, 75, 105,
        -524, 58, 132, 98, 65, 124, 149, 135,
        -434, -23, -22, 41, 129, 63, 137, 33,
};

const int mg_piece_values[] = {0, 159, 236, 296, 477, 900, 0};
const int eg_piece_values[] = {0, 134, 326, 358, 482, 898, 0};

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
