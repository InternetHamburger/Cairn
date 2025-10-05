#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        52, 25, 8, 12, 14, 24, 46, 75,
        46, 21, 10, 33, 24, 27, 57, 48,
        39, 37, 26, 17, 44, 42, 50, 66,
        45, 48, 28, 39, 36, 39, 71, 71,
        74, 66, 66, 34, 55, 86, 84, 92,
        107, 91, 111, 98, 79, 150, 70, 93,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        75, 77, 151, 125, 108, 62, 59, 52,
        53, -25, 61, 85, 98, 52, 67, 71,
        68, 90, 84, 120, 71, 99, 49, 71,
        53, 59, 54, 86, 77, 79, 36, 12,
        42, 54, 90, 69, 67, 67, 71, 55,
        54, 4, 89, 55, 74, -33, 32, 73,
        76, 47, 88, 85, 73, 92, 87, 48,
        -22, 71, 73, 15, 70, 58, -100, 24,
};

int bishop_table[64] = {
        136, 49, 83, 149, 107, 89, 115, 89,
        175, 72, 89, 111, 99, 92, 99, 124,
        58, 117, 104, 92, 111, 84, 54, 99,
        87, 51, 83, 72, 108, 57, 100, 101,
        40, 83, 61, 91, 88, 85, 92, 91,
        83, 65, 105, 67, 72, 68, 60, 88,
        5, 106, 110, 73, 79, 65, 90, 42,
        78, 19, 81, 90, 50, 72, 96, 67,
};

int rook_table[64] = {
        125, 163, 138, 144, 135, 160, 143, 126,
        133, 166, 160, 138, 117, 134, 130, 139,
        127, 131, 147, 124, 159, 153, 110, 123,
        153, 181, 138, 117, 142, 147, 180, 138,
        162, 178, 168, 126, 153, 183, 116, 130,
        184, 177, 171, 137, 97, 116, 125, 147,
        144, 164, 168, 195, 153, 162, 125, 145,
        164, 186, 145, 111, 104, 139, 144, 159,
};

int queen_table[64] = {
        300, 300, 300, 300, 300, 300, 300, 300,
        300, 300, 294, 299, 300, 300, 300, 300,
        300, 271, 283, 287, 298, 300, 300, 295,
        258, 296, 296, 300, 297, 285, 300, 300,
        300, 251, 300, 295, 300, 285, 300, 300,
        295, 300, 264, 287, 300, 300, 295, 300,
        260, 300, 300, 300, 300, 300, 300, 262,
        237, 257, 263, 300, 300, 300, 269, 268,
};

int king_table[64] = {
        -120, -34, 15, -2, -5, 5, -7, -4,
        -18, -15, -5, 12, 1, 6, -2, 19,
        -2, -17, -13, -1, -4, -4, 3, 3,
        -2, -4, -16, -22, -5, -10, -12, -19,
        31, -4, 5, -1, -4, 4, -15, -5,
        -11, -5, 19, 4, 20, -13, -5, -24,
        -7, 0, 11, 32, 2, 20, -23, -87,
        -51, -48, 13, 20, -17, -48, -15, -53,
};

int piece_square_values[BlackKing + 1][64];

__attribute__((constructor))
static void init_table(void) {
    for (int i = 0; i < 64; i++)
    {
        piece_square_values[WhitePawn][i] = pawn_table[i];
        piece_square_values[WhiteKnight][i] = knight_table[i];
        piece_square_values[WhiteBishop][i] = bishop_table[i];
        piece_square_values[WhiteRook][i] = rook_table[i];
        piece_square_values[WhiteQueen][i] = queen_table[i];
        piece_square_values[WhiteKing][i] = king_table[i];

        piece_square_values[BlackPawn][i] = -pawn_table[FlipSquare(i)];
        piece_square_values[BlackKnight][i] = -knight_table[FlipSquare(i)];
        piece_square_values[BlackBishop][i] = -bishop_table[FlipSquare(i)];
        piece_square_values[BlackRook][i] = -rook_table[FlipSquare(i)];
        piece_square_values[BlackQueen][i] = -queen_table[FlipSquare(i)];
        piece_square_values[BlackKing][i] = -king_table[FlipSquare(i)];
    }
}

int eval(const Board *board) {
    uint64_t occupied = GetOccupied(board);

    int eval = 0;
    while (occupied){
        const int square = poplsb(&occupied);
        eval += piece_square_values[board->squares[square]][square];
    }
    return eval * (board->white_to_move ? 1 : -1);
}
