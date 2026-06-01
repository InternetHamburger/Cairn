#include "movepicker.h"

#include <stdio.h>

#include "moveGeneration.h"
#include "moveOrderer.h"

void init_picker(MovePicker* mp, Thread* thread, int ply, Move tt_move){

    mp->tt_move = tt_move;
    mp->stage = STAGE_TT;

    mp->killer = thread->killer_moves[ply];
}

int best_index(const int* scores, int start, int end){
    int best = start;
    int best_score = scores[best];

    for (int i = start + 1; i < end; i++){
        int s = scores[i];
        if (s > best_score)
        {
            best = i;
            best_score = s;
        }
    }

    return best;
}

Move pop_move(int *len, Move* moves, int* scores, int idx){
    Move move = moves[idx];

    (*len)--;
    for (int i = idx; i < *len; i++)
    {
        moves[i] = moves[i + 1];
        scores[i] = scores[i + 1];
    }
    return move;
}

Move next_move(MovePicker* mp, Thread* thread, int ply){
    Board* board = &thread->board;

    switch (mp->stage) {
        case STAGE_TT:
            mp->stage = STAGE_GENERATE_MOVES;
            if (IsPseudoLegal(board, mp->tt_move))
                return mp->tt_move;

        case STAGE_GENERATE_MOVES:
            mp->stage = STAGE_MOVES;
            int num_moves = GetMoves(board, mp->moves);
            mp->move_num = num_moves;

            for (int i = 0; i < num_moves; i++){
                mp->scores[i] = move_score(thread, mp->moves[i], mp->tt_move, ply);
            }

        case STAGE_MOVES:
            while (mp->move_num){
                int best = best_index(mp->scores, 0, mp->move_num);
                Move move = pop_move(&mp->move_num, mp->moves, mp->scores, best);

                if (move.value == mp->tt_move.value){
                    continue;
                }
                return move;
            }

        default:
            return MoveConstructor(0, 0, 0);
    }
}