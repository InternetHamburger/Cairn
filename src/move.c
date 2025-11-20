#include "move.h"
#include "board.h"
#include <stdlib.h>
#include <stdio.h>

int StartSquare(const Move move){
    return (move.value >> 6) & 0b111111;
}

int TargetSquare(const Move move){
    return move.value & 0b111111;
}

int GetFlag(const Move move) {
    return (move.value >> 12) & 0b1111;
}

bool IsPromotion(const Move move) {
    return GetFlag(move) > 0b0011;
}

Move MoveConstructor(const int start_square, const int target_square, const MoveFlag flag){
    const Move move = {
            .value = (flag << 12) | (start_square << 6) | target_square
    };
    return move;
}