#include "move.h"

int StartSquare(Move move){
    return (move.value >> 6) & 0b111111;
}

int TargetSquare(Move move){
    return move.value & 0b111111;
}

Move MoveConstructor(int start_square, int target_square, MoveFlag flag){
    Move move = {
            .value = ((short)(flag) << (short)12) | (start_square << 6) | target_square
    };
    return move;
}