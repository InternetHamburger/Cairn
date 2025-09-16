#include "utility.h"
#include "board.h"
#include <assert.h>
#include <stdlib.h>

bool IsOppositeColor(bool white_to_move, int piece){
    return white_to_move == (piece >> 3) && !piece;
}

bool IsColor(bool white_to_move, int piece){
    return white_to_move != (piece >> 3);
}

char PieceToChar(const Piece piece) {
    switch (piece) {
        case None:
            return ' ';
        case WhitePawn:
            return 'P';
        case WhiteKnight:
            return 'N';
        case WhiteBishop:
            return 'B';
        case WhiteRook:
            return 'R';
        case WhiteQueen:
            return 'Q';
        case WhiteKing:
            return 'K';
        case BlackPawn:
            return 'p';
        case BlackKnight:
            return 'n';
        case BlackBishop:
            return 'b';
        case BlackRook:
            return 'r';
        case BlackQueen:
            return 'q';
        case BlackKing:
            return 'k';
        default:
            exit(-1);
    }
}

PieceType GetType(Piece piece) {
    return piece & 0b0111;
}


int getlsb(unsigned long long bb) {
    assert(bb);  // lsb(0) is undefined
    return __builtin_ctzll(bb);
}

int poplsb(unsigned long long *bb) {
    int lsb = getlsb(*bb);
    *bb &= *bb - 1;
    return lsb;
}