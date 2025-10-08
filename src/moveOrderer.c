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
    int move_scores[move_length];
    for (int i = 0; i < move_length; i++){
        move_scores[i] = mvv_lva(moves[i], board);
    }

    // Insertion sort implementation
    for (int i = 1; i < move_length; i++) {
        Move key_move = moves[i];
        int key_score = move_scores[i];
        int j = i - 1;

        while (j >= 0 && move_scores[j] < key_score) {
            moves[j + 1] = moves[j];
            move_scores[j + 1] = move_scores[j];
            j--;
        }

        moves[j + 1] = key_move;
        move_scores[j + 1] = key_score;
    }
    //qsort_s(moves, move_length, sizeof(Move), compare, board);
}