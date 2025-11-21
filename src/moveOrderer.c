#include "moveOrderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "utility.h"




int history_table[BlackKing + 1][64];
Move killer_moves[256];

const int piece_scores[] = {0, 1, 3, 3, 5, 9, 0};

void ZeroHist()
{
    memset(history_table, 0, sizeof(history_table));
    memset(killer_moves, 0, sizeof(Move) * 256);
}

void UpdateHistTable(const Board* board, const Move move, const int bonus)
{
    int* hist_value = &history_table[board->squares[StartSquare(move)]][TargetSquare(move)];
    *hist_value += bonus - *hist_value * abs(bonus) / MAX_HISTORY;
}

void UpdateKillers(const Move move, const int ply)
{
    killer_moves[ply] = move;
}


int mvv_lva(Move move, Board *board){
    return 100 * piece_scores[GetType(board->squares[TargetSquare(move)])] - piece_scores[GetType(board->squares[StartSquare(move)])];
}

int move_score(Move move, Board* board, Move tt_move, int ply){
    if (move.value == tt_move.value){
        return 100000;
    }
    if (board->squares[TargetSquare(move)] != None){
        return mvv_lva(move, board) - !staticExchangeEvaluation(board, move, 0) * 100000;
    }
    if (move.value == killer_moves[ply].value){
        return 1;
    }
    return history_table[board->squares[StartSquare(move)]][TargetSquare(move)] - MAX_HISTORY; // Ensure quiet moves are ordered last
}

void OrderMoves(Board *board, Move* moves, int move_length, int ply, Move tt_move){
    int move_scores[move_length];
    for (int i = 0; i < move_length; i++){
        move_scores[i] = move_score(moves[i], board, tt_move, ply);
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

void OrderCaptures(Board *board, Move* moves, int move_length){
    int move_scores[move_length];
    for (int i = 0; i < move_length; i++){
        move_scores[i] = mvv_lva(moves[i], board);// + staticExchangeEvaluation(board, moves[i], 0) * 1000;
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