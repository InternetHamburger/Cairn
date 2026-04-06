#include "moveOrderer.h"

#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "utility.h"

const int piece_scores[] = {0, 1, 3, 3, 5, 9, 0};

void ZeroHist(Thread* thread)
{
    memset(thread->cont_hist, 0, sizeof(thread->cont_hist));
    memset(thread->quiet_history, 0, sizeof(thread->quiet_history));
}

void ZeroKillers(Thread* thread)
{
    memset(thread->killer_moves, 0, sizeof(Move) * 256);
}

void UpdateHistTable(Thread* thread, const int ply, const Move move, const int bonus)
{
    const Piece piece = thread->board.squares[StartSquare(move)];
    const int to_square = TargetSquare(move);

    if (ply > 0){
        int* cont_value = &thread->cont_hist[thread->ss[ply - 1].moved_piece][thread->ss[ply - 1].to_square][piece][to_square];
        *cont_value += bonus - *cont_value * abs(bonus) / MAX_HISTORY;
    }
    if (ply > 1){
        int* cont_value = &thread->cont_hist[thread->ss[ply - 2].moved_piece][thread->ss[ply - 2].to_square][piece][to_square];
        *cont_value += bonus - *cont_value * abs(bonus) / MAX_HISTORY;
    }

    int* quiet_value = &thread->quiet_history[piece][to_square];
    *quiet_value += bonus - *quiet_value * abs(bonus) / MAX_HISTORY;
}

int get_history(Thread* thread, const Move move, const int ply){

    const Piece piece = thread->board.squares[StartSquare(move)];
    const int to_square = TargetSquare(move);

    int cont_value = 0;
    cont_value = ply < 1 ? 0 : thread->cont_hist[thread->ss[ply - 1].moved_piece][thread->ss[ply - 1].to_square][piece][to_square];
    cont_value += ply < 2 ? 0 : thread->cont_hist[thread->ss[ply - 2].moved_piece][thread->ss[ply - 2].to_square][piece][to_square];

    int quiet_value = thread->quiet_history[piece][to_square];

    return cont_value + quiet_value;
}

void UpdateKillers(Thread* thread, const Move move, const int ply)
{
    thread->killer_moves[ply] = move;
}

int mvv_lva(Move move, Board *board)
{
    return 100 * piece_scores[GetType(board->squares[TargetSquare(move)])] - piece_scores[GetType(board->squares[StartSquare(move)])];
}

int move_score(Thread* thread, Move move, Move tt_move, int ply)
{
    if (move.value == tt_move.value){
        return 100000;
    }
    if (thread->board.squares[TargetSquare(move)] != None){
        return mvv_lva(move, &thread->board) - !staticExchangeEvaluation(&thread->board, move, 0) * 100000;
    }
    if (move.value == thread->killer_moves[ply].value){
        return 1;
    }
    return get_history(thread, move, ply) - 3 * MAX_HISTORY; // Ensure quiet moves are ordered last
}

void OrderMoves(Thread* thread, Move* moves, int move_length, int ply, Move tt_move)
{
    int move_scores[move_length];
    for (int i = 0; i < move_length; i++){
        move_scores[i] = move_score(thread, moves[i], tt_move, ply);
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

void OrderCaptures(Board *board, Move* moves, int move_length)
{
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
}