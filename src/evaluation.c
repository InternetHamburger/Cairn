#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        77, 103, 180, 195, -12, 28, 83, 163,
        139, 132, 121, 106, -64, 79, 20, -5,
        -8, 17, 20, 5, -38, -40, -58, -30,
        -64, -57, -62, -19, -55, -9, -83, -12,
        -79, -10, 8, -45, -71, -57, -40, -57,
        -84, -26, 70, -71, -293, -13, -35, -21,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        209, 176, 66, 159, 109, 117, -78, -37,
        -135, 120, 100, 102, -3, -9, -34, -75,
        -114, 3, -5, 60, 179, -9, -90, 140,
        -97, -9, 30, -36, 7, 78, 38, 117,
        -238, 54, -33, 36, -24, 49, -38, -93,
        68, 21, 77, 68, 35, 48, -99, 49,
        -36, 212, -67, 69, -4, -51, -213, 80,
        -192, -162, 202, -367, -108, -58, -33, -300,
};

int mg_bishop_table[64] = {
        100, 268, 22, -151, -8, -16, 0, 153,
        -45, 21, -100, 144, 159, 20, 3, -166,
        -162, -179, -161, 13, 30, 111, -68, 129,
        -104, -34, -13, -13, 41, 41, 101, -60,
        -75, 90, -54, -12, 56, 73, 23, 19,
        54, 30, 25, 69, 74, -42, 43, -5,
        -4, -39, 83, 168, 77, 72, 85, -753,
        6, 29, -25, -156, -17, -3, -205, 217,
};

int mg_rook_table[64] = {
        27, 24, -3, 3, 0, -12, -52, 1,
        40, 29, 29, 64, 5, -14, -57, 53,
        80, 103, -27, -2, 44, -54, 46, 47,
        -17, -51, -67, 17, 29, -2, -27, -36,
        16, 26, 11, 84, 55, 52, -3, -99,
        -22, -21, 29, 6, 69, -58, -38, -42,
        11, 8, 11, -42, -45, 25, -33, -79,
        16, -20, 13, 6, -7, 26, -127, -65,
};

int mg_queen_table[64] = {
        30, -1, 30, 30, 30, -26, 30, 30,
        -240, 30, -26, 29, 30, 30, -36, -147,
        29, -19, 30, 30, 30, 30, -38, -1,
        2, -7, -140, 30, 30, 30, 29, 30,
        -47, -23, -202, 17, 21, 29, -17, -66,
        30, 30, 30, 30, 30, -10, 30, 30,
        -113, 30, 30, 30, 30, 30, 30, 29,
        30, 30, 23, 11, 30, 30, -116, 30,
};

int mg_king_table[64] = {
        -157, 43, -284, 285, 259, -185, 258, 329,
        153, -68, -151, 241, 66, -24, -50, 344,
        66, -7, 63, 52, 114, -188, 31, 39,
        -78, -75, 14, 25, 1, 15, 36, 60,
        -111, 74, 13, -4, -52, -31, 22, -63,
        -115, -96, -11, -61, -43, -24, -71, -24,
        157, -17, -126, -94, -124, -29, -68, -90,
        310, 35, -142, -192, -16, -82, -43, -115,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        -82, -192, 339, 133, 266, 639, 361, 122,
        -73, 18, -65, 114, 66, 0, 68, 8,
        -74, -35, -38, -52, -19, 17, -24, -97,
        -64, -29, -71, -12, -27, -35, -35, -83,
        -129, -21, -35, -40, -43, -123, 1, -87,
        -110, -25, -72, -140, -129, 4, 0, -76,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -35, 86, -209, 59, -164, -35, -247, -234,
        -568, -108, 122, 4, 220, -17, 90, 110,
        -100, 82, -102, 51, 41, 146, 55, -231,
        4, -48, -120, 68, 35, 74, 39, 65,
        -27, 44, 76, 24, 30, 66, 126, -33,
        -61, -23, -23, 77, 71, 11, 14, -18,
        -74, -94, -58, 11, -36, 79, -264, 245,
        433, -50, 305, -177, 282, -47, -47, 9,
};

int eg_bishop_table[64] = {
        21, 270, -9, 145, 428, 59, -100, -29,
        -62, 105, 74, 3, -13, -168, 43, -38,
        -22, 155, -36, 19, -70, 70, 83, 83,
        89, 0, 148, 10, 24, 78, 41, -27,
        50, 60, 28, 13, 77, 26, 58, 0,
        -7, 94, -37, 33, 27, 61, 41, -8,
        51, -8, -24, -2, -22, -10, 10, -783,
        -33, 19, -13, 3, 67, -12, -1155, 42,
};

int eg_rook_table[64] = {
        91, 170, 142, 100, -116, 222, 222, 130,
        56, 47, 64, -6, 76, 222, 172, 104,
        -21, -42, -14, -148, 222, -254, -69, -2,
        -48, -133, -36, -291, -88, 132, -173, 18,
        -67, -76, 18, 26, 39, -111, 31, -8,
        36, 16, -18, -59, -87, 93, -47, -21,
        -51, 93, 18, 41, -168, 5, -44, -129,
        -9, -54, -20, -9, -9, -19, -136, -35,
};

int eg_queen_table[64] = {
        -74, 31, 31, 31, 31, 31, 31, 31,
        -111, 1, 0, -40, -5, 31, 31, 14,
        31, -122, 13, 31, 31, 30, -32, 21,
        31, -29, 31, 31, 14, -22, 31, 31,
        -46, 9, -54, 31, 31, 7, -17, 30,
        -4, 4, -12, 31, -38, 31, -12, -16,
        10, 18, -86, 26, 31, 31, 30, 31,
        -80, 31, 31, 23, -269, -36, 31, 31,
};

int eg_king_table[64] = {
        27, 27, 27, 27, 27, 27, 27, 27,
        27, -764, -113, -88, 27, 27, 27, 27,
        819, 273, 27, -271, 102, 27, 27, 27,
        461, -88, 174, -764, -128, -582, 3, 183,
        -63, 4, -79, 20, 75, -764, 95, 154,
        305, -197, 100, 119, -92, 95, -6, 56,
        -764, 86, -1, -73, -18, 7, 73, 70,
        789, -44, -53, 24, 47, 46, 119, 189,
};

const int mg_piece_values[] = {0, 189, 283, 284, 488, 762, 0};
const int eg_piece_values[] = {0, 153, 359, 364, 569, 761, 0};

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
