#ifndef CAIRN_MOVE_H
#define CAIRN_MOVE_H

typedef enum{
    none = 0b0000,
    Castle = 0b0001,
    EnPassant = 0b0010,
    DoublePush = 0b0011,
    PromoteQueen = 0b0100,
    PromoteKnight = 0b0101,
    PromoteRook = 0b0110,
    PromoteBishop = 0b0111,
} MoveFlag;

typedef struct{
    short int value; // Format ffffsssssstttttt, f - flag, s - start square, t - target square
} Move;

int StartSquare(Move move);
int TargetSquare(Move move);
int GetFlag(Move move);
bool IsPromotion(Move move);
Move MoveConstructor(int start_square, int target_square, MoveFlag flag);

#endif //CAIRN_MOVE_H
