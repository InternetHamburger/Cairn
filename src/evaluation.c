#include "evaluation.h"

#include <stdlib.h>

#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        269, 217, 165, 199, 224, 122, 90, 89,
        -57, -16, -7, 22, 31, 20, -8, -50,
        -64, -36, -41, -23, -18, -22, -23, -58,
        -64, -41, -40, -25, -26, -28, -26, -55,
        -64, -39, -42, -41, -33, -31, -10, -51,
        -74, -47, -54, -70, -58, -23, -13, -58,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -178, -90, -71, 13, 68, -138, -22, -138,
        -24, -2, 38, 77, 53, 78, -11, 0,
        -8, 32, 43, 87, 117, 97, 62, 9,
        -1, 7, 36, 39, 28, 66, 14, 23,
        -8, -4, 10, 9, 17, 12, 21, 2,
        -22, -2, -6, 7, 10, 2, 13, -16,
        -33, -21, -11, -10, -4, 9, -20, -8,
        -48, -33, -32, -41, -27, -18, -33, -20,
};

int mg_bishop_table[64] = {
        -74, -67, -73, -43, -7, -67, 19, -27,
        -33, -17, 3, -6, 32, 38, 8, -16,
        0, 18, 37, 51, 66, 102, 38, 22,
        -1, 9, 25, 50, 36, 37, 12, 5,
        -18, 12, 13, 25, 27, 8, 6, 2,
        0, 1, -1, 8, -3, 7, 1, 2,
        -14, -2, -1, -10, 0, 3, 13, -13,
        -70, -24, -11, -17, -20, -21, -40, -46,
};

int mg_rook_table[64] = {
        78, 58, 61, 68, 69, 34, 39, 82,
        22, 27, 69, 77, 63, 85, 26, 64,
        -3, 11, 14, 58, 93, 74, 61, 17,
        -25, -28, -3, 6, 11, 5, 0, -2,
        -51, -41, -40, -19, -29, -24, -21, -30,
        -58, -40, -43, -46, -45, -33, -16, -35,
        -62, -56, -43, -37, -42, -28, -37, -71,
        -37, -39, -31, -27, -21, -28, -42, -35,
};

int mg_queen_table[64] = {
        -11, 24, 43, 101, 142, 155, 65, 63,
        -25, -32, -5, 8, 16, 72, 47, 61,
        -31, -19, -6, 22, 29, 63, 43, 7,
        -41, -41, -17, -16, -11, -8, -13, -11,
        -21, -38, -20, -20, -17, -24, -13, -22,
        -22, -17, -28, -22, -16, -19, -2, -17,
        -46, -17, -9, -13, -15, 1, -12, -20,
        -39, -42, -15, -8, -10, -37, -42, -19,
};

int mg_king_table[64] = {
        182, 35, 184, 21, 40, 85, 60, -98,
        48, 45, -14, -21, -19, -22, -52, -169,
        58, -39, -54, -88, 0, -96, -61, -18,
        10, -11, -73, -36, -18, -38, -18, -45,
        -33, -2, -44, -43, -37, -28, -5, -50,
        10, 12, -8, -42, -30, -12, 25, 10,
        54, 45, 35, -9, -6, 41, 54, 59,
        -48, 45, 51, -24, 46, 5, 79, 75,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        -14, 61, 87, 38, -12, 90, 118, 123,
        109, 75, 45, 12, -11, -15, 32, 56,
        31, 5, -11, -33, -43, -50, -34, -7,
        5, -20, -32, -43, -47, -50, -41, -30,
        -5, -18, -35, -32, -37, -48, -46, -38,
        12, -5, -11, -12, -16, -31, -34, -31,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        16, 50, 47, -5, -43, 109, 33, 20,
        -6, 8, -10, -15, -15, -32, 10, -33,
        8, -4, 27, -11, -26, -15, 0, -2,
        -5, 15, 34, 26, 47, 13, 32, -4,
        -8, 31, 44, 43, 32, 40, 2, -2,
        -32, 0, 8, 26, 16, 12, -7, -30,
        -45, -8, -7, 11, 2, -21, 0, -49,
        -50, -60, -22, -5, -18, -36, -39, -95,
};

int eg_bishop_table[64] = {
        75, 83, 73, 43, 7, 35, -21, -1,
        42, 39, 13, 24, -12, -23, -15, -13,
        -4, 12, 0, -14, -34, -51, -8, -21,
        -14, 20, 2, -3, 5, 0, 15, -9,
        -9, -3, 22, 18, 6, 13, 4, -14,
        -24, 0, 16, 8, 24, 6, -2, -17,
        -15, -13, -5, 2, -1, -18, -22, -63,
        18, -8, -58, -14, -25, -32, -20, -17,
};

int eg_rook_table[64] = {
        -56, -23, -15, -18, -16, 13, 6, -32,
        22, 24, -4, -14, -4, -23, 21, -9,
        33, 20, 27, -11, -46, -22, -24, 9,
        33, 44, 32, 18, 7, 13, 8, 0,
        27, 27, 36, 19, 19, 16, 8, -1,
        11, 4, 8, 10, 10, -6, -23, -28,
        -8, -5, -3, -15, -13, -23, -12, -14,
        -14, 1, 3, 3, -8, -8, 0, -33,
};

int eg_queen_table[64] = {
        10, -5, -9, -75, -97, -121, -8, -18,
        24, 58, 60, 56, 69, 30, 21, -16,
        20, 41, 58, 43, 83, 47, 44, 53,
        23, 58, 50, 87, 105, 110, 71, 59,
        -27, 28, 17, 43, 41, 61, 32, 24,
        -48, -24, 21, 9, 0, 16, -27, -30,
        -26, -60, -39, -45, -29, -72, -93, -92,
        -32, -24, -98, -68, -109, -91, -102, -89,
};

int eg_king_table[64] = {
        -120, -20, -147, -11, -41, -57, -49, -47,
        -33, -1, 36, 32, 32, 57, 77, 104,
        -42, 73, 81, 105, 44, 127, 106, 36,
        -14, 43, 94, 70, 64, 78, 65, 32,
        -9, 14, 49, 64, 56, 47, 17, 12,
        -40, -16, 10, 37, 35, 13, -28, -40,
        -80, -55, -48, -9, -13, -48, -62, -85,
        10, -75, -78, -49, -100, -66, -103, -134,
};

const int mg_piece_values[] = {0, 108, 351, 371, 463, 840, 0};
const int eg_piece_values[] = {0, 169, 260, 287, 556, 1098, 0};

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
