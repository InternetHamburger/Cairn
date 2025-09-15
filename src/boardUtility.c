#include "boardUtility.h"
#include "move.h"
#include "board.h"

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