#include "evaluation.h"

#include <stdlib.h>

#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        275, 228, 172, 205, 230, 129, 98, 94,
        -58, -16, -7, 22, 31, 19, -9, -51,
        -65, -38, -42, -24, -20, -23, -25, -60,
        -66, -42, -41, -26, -27, -29, -27, -58,
        -66, -40, -43, -42, -35, -33, -12, -53,
        -76, -49, -56, -71, -60, -24, -15, -61,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -191, -92, -71, 12, 69, -138, -29, -149,
        -25, -3, 39, 79, 54, 79, -11, 0,
        -8, 33, 44, 88, 119, 99, 63, 11,
        -1, 7, 38, 40, 29, 68, 15, 24,
        -7, -3, 11, 10, 18, 12, 22, 3,
        -22, -1, -5, 8, 12, 2, 14, -16,
        -33, -21, -11, -9, -3, 9, -20, -8,
        -48, -33, -31, -41, -27, -18, -33, -20,
};

int mg_bishop_table[64] = {
        -78, -69, -72, -42, -5, -68, 20, -33,
        -34, -16, 3, -5, 33, 38, 8, -17,
        0, 19, 37, 52, 66, 103, 39, 22,
        -1, 9, 26, 51, 36, 37, 12, 5,
        -19, 12, 14, 25, 28, 7, 7, 2,
        0, 2, -2, 8, -2, 8, 1, 1,
        -14, -2, 0, -10, 0, 4, 14, -13,
        -70, -24, -12, -17, -20, -22, -40, -46,
};

int mg_rook_table[64] = {
        80, 58, 62, 69, 70, 34, 41, 85,
        22, 27, 70, 78, 64, 86, 26, 65,
        -3, 10, 15, 59, 94, 75, 61, 16,
        -26, -28, -3, 7, 11, 5, 0, -2,
        -51, -41, -40, -19, -30, -24, -21, -31,
        -59, -40, -43, -45, -45, -33, -16, -35,
        -63, -56, -43, -37, -42, -28, -37, -72,
        -38, -40, -31, -27, -22, -29, -43, -36,
};

int mg_queen_table[64] = {
        -23, 14, 32, 91, 133, 140, 60, 56,
        -26, -32, -6, 4, 13, 72, 46, 61,
        -30, -18, -6, 20, 29, 63, 45, 9,
        -40, -39, -16, -14, -11, -6, -11, -7,
        -19, -36, -17, -17, -15, -22, -10, -19,
        -20, -13, -26, -18, -13, -16, 1, -14,
        -45, -14, -6, -10, -12, 4, -10, -19,
        -36, -39, -12, -4, -7, -36, -47, -20,
};

int mg_king_table[64] = {
        108, 33, 182, 17, 33, 74, 47, -81,
        49, 48, -14, -21, -20, -21, -54, -160,
        61, -38, -53, -88, 1, -95, -61, -18,
        10, -9, -70, -34, -15, -35, -16, -41,
        -31, 1, -41, -40, -35, -26, -3, -49,
        13, 14, -6, -39, -28, -10, 27, 12,
        56, 47, 36, -8, -4, 42, 56, 60,
        -47, 47, 52, -23, 48, 6, 81, 76,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        -20, 51, 80, 32, -18, 82, 110, 117,
        109, 76, 45, 12, -10, -14, 33, 57,
        33, 6, -9, -32, -42, -47, -32, -5,
        7, -18, -31, -42, -45, -48, -39, -28,
        -3, -16, -34, -31, -36, -47, -45, -35,
        15, -3, -9, -9, -15, -29, -32, -28,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        30, 51, 47, -4, -44, 109, 38, 31,
        -5, 9, -10, -16, -16, -33, 10, -32,
        8, -5, 26, -12, -28, -17, 0, -3,
        -5, 15, 33, 25, 46, 11, 31, -5,
        -8, 31, 43, 42, 31, 39, 0, -3,
        -32, -1, 8, 25, 15, 11, -9, -31,
        -44, -8, -7, 11, 2, -22, -1, -49,
        -51, -59, -22, -5, -18, -36, -39, -96,
};

int eg_bishop_table[64] = {
        78, 83, 72, 42, 5, 36, -21, 4,
        43, 39, 13, 24, -12, -22, -15, -12,
        -4, 12, 0, -15, -34, -52, -9, -21,
        -14, 20, 2, -4, 4, 0, 14, -9,
        -8, -3, 21, 18, 5, 13, 4, -14,
        -24, 0, 16, 8, 24, 6, -2, -18,
        -15, -12, -5, 2, -1, -18, -22, -62,
        18, -8, -58, -15, -25, -32, -19, -17,
};

int eg_rook_table[64] = {
        -57, -23, -15, -18, -17, 12, 4, -34,
        22, 23, -5, -15, -5, -24, 20, -9,
        34, 20, 27, -11, -47, -23, -24, 9,
        33, 44, 32, 18, 7, 13, 8, 1,
        28, 28, 36, 19, 19, 16, 8, -1,
        11, 4, 9, 9, 10, -6, -22, -27,
        -8, -5, -3, -15, -13, -23, -12, -13,
        -13, 1, 3, 3, -7, -8, 0, -32,
};

int eg_queen_table[64] = {
        32, 13, 11, -55, -79, -95, 3, -3,
        28, 60, 64, 65, 76, 34, 25, -14,
        18, 42, 61, 47, 86, 50, 44, 50,
        21, 54, 50, 86, 105, 109, 66, 50,
        -37, 24, 11, 38, 37, 55, 26, 17,
        -52, -34, 14, 1, -7, 7, -36, -36,
        -28, -68, -49, -53, -38, -79, -98, -92,
        -38, -30, -106, -80, -115, -91, -87, -86,
};

int eg_king_table[64] = {
        -68, -19, -145, -8, -36, -48, -38, -59,
        -33, -3, 35, 33, 34, 57, 79, 98,
        -43, 73, 81, 105, 43, 126, 106, 36,
        -14, 41, 92, 67, 62, 76, 63, 30,
        -10, 11, 47, 61, 54, 45, 15, 12,
        -42, -18, 8, 35, 33, 11, -29, -41,
        -81, -56, -50, -11, -15, -49, -63, -86,
        8, -77, -79, -49, -100, -66, -104, -134,
};

const int mg_piece_values[] = {0, 113, 372, 394, 500, 1098, 0};
const int eg_piece_values[] = {0, 164, 235, 262, 514, 793, 0};

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

const int piece_game_phase[] = {0, 0, 1, 1, 2, 4, 0};

int eval(const Board *board) {
    uint64_t occupied = GetOccupied(board);

    int mg_eval = 0;
    int eg_eval = 0;
    int phase = 0;
    while (occupied){
        const int square = poplsb(&occupied);
        mg_eval += mg_piece_square_values[board->squares[square]][square];
        eg_eval += eg_piece_square_values[board->squares[square]][square];
        phase += piece_game_phase[GetType(board->squares[square])];
    }

    // Interpolate based on number of pieces
    phase = __min(24, phase);
    int eval = (mg_eval * phase + eg_eval * (24 - phase)) / 24;

    return eval * (board->white_to_move ? 1 : -1);
}
