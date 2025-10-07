#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        7, 38, 7, 34, 31, 57, 25, 33,
        11, 37, 34, 41, 46, 71, 40, 46,
        20, 35, 26, 39, 49, 71, 25, 71,
        15, 43, 41, 40, 55, 90, 91, 81,
        63, 69, 60, 67, 76, 124, 96, 93,
        76, 113, 66, 82, 88, 131, 100, 144,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        98, 49, 90, 51, 80, 52, 51, 94,
        75, 31, 75, 71, 61, 73, 49, 69,
        57, 78, 72, 77, 76, 84, 65, 82,
        62, 74, 63, 86, 67, 87, 64, 75,
        62, 69, 82, 69, 82, 74, 91, 79,
        63, 68, 74, 78, 99, 87, 69, 87,
        71, 32, 63, 77, 83, 84, 101, 68,
        30, 66, 80, 85, 79, 79, 50, 48,
};

int bishop_table[64] = {
        77, 90, 47, 82, 70, 74, 62, 81,
        71, 74, 95, 71, 86, 60, 87, 88,
        61, 87, 62, 85, 81, 86, 80, 89,
        59, 72, 90, 69, 78, 79, 76, 65,
        79, 76, 64, 98, 78, 71, 72, 51,
        81, 78, 70, 82, 83, 66, 83, 86,
        74, 88, 83, 67, 59, 68, 100, 68,
        83, 65, 82, 72, 73, 88, 86, 69,
};

int rook_table[64] = {
        113, 110, 128, 121, 126, 133, 122, 104,
        112, 108, 125, 119, 127, 126, 136, 124,
        111, 128, 136, 125, 131, 131, 129, 108,
        118, 125, 135, 129, 131, 130, 138, 135,
        139, 151, 144, 150, 134, 133, 142, 127,
        135, 147, 148, 144, 147, 154, 156, 156,
        144, 137, 161, 131, 159, 150, 152, 163,
        148, 152, 141, 124, 144, 149, 158, 171,
};

int queen_table[64] = {
        342, 305, 292, 301, 342, 338, 353, 354,
        365, 354, 348, 350, 360, 367, 364, 377,
        352, 365, 336, 376, 355, 363, 345, 353,
        361, 351, 345, 361, 346, 344, 338, 343,
        363, 362, 345, 363, 346, 360, 365, 340,
        363, 355, 357, 368, 335, 310, 354, 332,
        367, 368, 362, 344, 350, 352, 377, 354,
        345, 364, 352, 355, 343, 377, 341, 363,
};

int king_table[64] = {
        -63, -18, 3, -9, -27, -7, 21, 11,
        -25, -6, 1, 2, -4, 4, 38, 32,
        -27, -8, -7, 6, -9, 18, -8, -23,
        -51, -23, -8, -10, 12, 4, 5, -22,
        -55, -41, -34, -38, -16, -27, -64, -4,
        -64, -11, 25, 36, -33, -39, -8, -26,
        -4, 17, 16, 17, -40, -14, -25, -33,
        -61, 10, 29, 29, -64, -20, -24, -40,
};

int piece_square_values[BlackKing + 1][64];

__attribute__((constructor))
static void init_table(void) {
    for (int i = 0; i < 64; i++)
    {
        piece_square_values[WhitePawn][i] = 100;
        piece_square_values[WhiteKnight][i] = 300;
        piece_square_values[WhiteBishop][i] = 300;
        piece_square_values[WhiteRook][i] = 500;
        piece_square_values[WhiteQueen][i] = 900;
        piece_square_values[WhiteKing][i] = 0;

        piece_square_values[BlackPawn][i] = -100;
        piece_square_values[BlackKnight][i] = -300;
        piece_square_values[BlackBishop][i] = -300;
        piece_square_values[BlackRook][i] = -500;
        piece_square_values[BlackQueen][i] = -900;
        piece_square_values[BlackKing][i] = 0;
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
