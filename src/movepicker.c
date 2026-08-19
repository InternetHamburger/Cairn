#include "board.h"
#include "movepicker.h"
#include "moveOrderer.h"
#include "moveGeneration.h"
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
    moves[idx] = moves[--*len];
    scores[idx] = scores[*len];
    return move;
}
Move next_move(MovePicker* mp, Thread* thread, int ply){
    Board* board = &thread->board;

    switch (mp->stage) {
        case STAGE_TT:
            mp->stage = STAGE_GENERATE_CAPTURES;
            if (IsLegal(board, mp->tt_move))
                return mp->tt_move;
            // Fall through

        case STAGE_GENERATE_CAPTURES:
            mp->stage = STAGE_GOOD_CAPTURES;
            mp->num_captures = GetCaptures(board, mp->captures);

            for (int i = 0; i < mp->num_captures; i++){
                mp->capture_scores[i] = move_score(thread, mp->captures[i], mp->tt_move, ply);
            }

            // Fall through

        case STAGE_GOOD_CAPTURES:
            while (mp->num_captures){
                int best = best_index(mp->capture_scores, 0, mp->num_captures);
                int score = mp->capture_scores[best];
                // Bad captures have begun
                if (score < -100 * MAX_HISTORY)
                {
                    mp->stage = STAGE_KILLERS;
                    break;
                }

                Move move = pop_move(&mp->num_captures, mp->captures, mp->capture_scores, best);

                if (move.value == mp->tt_move.value || move.value == mp->killer.value){
                    continue;
                }

                return move;
            }
            // Fall through

        case STAGE_KILLERS:
            mp->stage = STAGE_GENERATE_QUIETS;
            if (IsLegal(board, mp->killer))
                return mp->killer;
            // Fall through

        case STAGE_GENERATE_QUIETS:
            mp->stage = STAGE_QUIETS;
            mp->num_quiets = GetQuiets(board, mp->quiets);

            for (int i = 0; i < mp->num_quiets; i++){
                mp->quiet_scores[i] = move_score(thread, mp->quiets[i], mp->tt_move, ply);
            }
            // Fall through

        case STAGE_QUIETS:
            while (mp->num_quiets){
                int best = best_index(mp->quiet_scores, 0, mp->num_quiets);
                Move move = pop_move(&mp->num_quiets, mp->quiets, mp->quiet_scores, best);

                if (move.value == mp->tt_move.value || move.value == mp->killer.value){
                    continue;
                }

                return move;
            }
            // Fall through

        case STAGE_BAD_CAPTURES:
            while (mp->num_captures){
                int best = best_index(mp->capture_scores, 0, mp->num_captures);
                Move move = pop_move(&mp->num_captures, mp->captures, mp->capture_scores, best);

                if (move.value == mp->tt_move.value || move.value == mp->killer.value){
                    continue;
                }
                return move;
            }
            // Fall through
        default:
            return MoveConstructor(0, 0, 0);
    }
}