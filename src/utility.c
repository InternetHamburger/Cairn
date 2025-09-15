#include "utility.h"
#include "move.h"
#include "board.h"
#include <assert.h>

bool IsOppositeColor(bool white_to_move, int piece){
    return white_to_move == (piece >> 3);
}

bool IsColor(bool white_to_move, int piece){
    return white_to_move != (piece >> 3);
}

char* MoveToChar(Move move){

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