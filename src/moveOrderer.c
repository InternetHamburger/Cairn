#include "moveOrderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "utility.h"




int history_table[BlackKing + 1][64];

const int piece_scores[] = {0, 1, 3, 3, 5, 9, 0};

void ZeroHist()
{
    memset(history_table, 0, sizeof(history_table));
}

void UpdateHistTable(const Board* board, const Move move, const int bonus)
{
    int* hist_value = &history_table[board->squares[StartSquare(move)]][TargetSquare(move)];
    *hist_value += bonus - *hist_value * abs(bonus) / MAX_HISTORY;
}

int mvv_lva(Move move, Board *board){
    return 100 * piece_scores[GetType(board->squares[TargetSquare(move)])] - piece_scores[GetType(board->squares[StartSquare(move)])];
}

int move_score(Move move, Board* board, Move tt_move){
    if (move.value == tt_move.value){
        return 1000;
    }
    if (board->squares[TargetSquare(move)] != None){
        return mvv_lva(move, board);
    }
    return history_table[board->squares[StartSquare(move)]][TargetSquare(move)] - MAX_HISTORY; // Ensure quiet moves are ordered last
}

void OrderMoves(Board *board, Move* moves, int move_length, Move tt_move){
    int move_scores[move_length];
    for (int i = 0; i < move_length; i++){
        move_scores[i] = move_score(moves[i], board, tt_move);
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
}