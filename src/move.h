#ifndef CAIRN_MOVE_H
#define CAIRN_MOVE_H

typedef struct{
    short int value; // Format ffffsssssstttttt, f - flag, s - start square, t - target square
} Move;

int StartSquare(Move move);
int TargetSquare(Move move);
Move MoveConstructor(int start_square, int target_square, int flag);

#endif //CAIRN_MOVE_H
