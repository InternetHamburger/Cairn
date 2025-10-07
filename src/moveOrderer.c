#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "utility.h"



const int piece_scores[] = {0, 1, 3, 3, 5, 9, 0};

int mvv_lva(Move move, Board *board){
    return 100 * piece_scores[GetType(board->squares[TargetSquare(move)])] - piece_scores[GetType(board->squares[StartSquare(move)])];
}

int compare(void *board, const void *a, const void *b){
    Move *A = (Move*)a;
    Move *B = (Move*)b;
    int a_score = mvv_lva(*A, board);
    int b_score = mvv_lva(*B, board);

    return b_score - a_score;
}

void OrderMoves(Board *board, Move* moves, int move_length){
    qsort_s(moves, move_length, sizeof(Move), compare, board);
}