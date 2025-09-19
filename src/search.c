#include "search.h"
#include <limits.h>
#include "utility.h"
#include "board.h"
#include "evaluation.h"
#include "move.h"
#include "moveGeneration.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int Negamax(Stack *stack, Board *board, int depth, bool isTop, Move *move) {
    if (depth == 0) return eval(board);

    int num_legal_moves = 0;

    int num_moves = 0;
    Move* moves = GetMoves(board, &num_moves);
    const Board copy = *board;

    int best_score = NEG_INF;
    for (int i = 0; i < num_moves; i++) {

        if (GetFlag(moves[i]) == Castle && !IsLegalCastle(board, moves[i])){
            continue;
        }
        MakeMove(board, moves[i]);
        if (IsAttackedBySideToMove(board, board->white_to_move, board->white_to_move ? board->black_king_square : board->white_king_square)) {
            *board = copy;
            continue;
        }
        stack->nodes++;
        num_legal_moves++;
        const int score = -Negamax(stack, board, depth - 1, false, move);
        *board = copy;

        if (stack->nodes > stack->node_limit || clock() - stack->start_time > stack->time_limit) {
            free(moves);
            return NEG_INF;
        }

        if (score > best_score) {
            best_score = score;
            if (isTop) {
                *move = moves[i];
            }
        }
    }
    free(moves);
    if (num_legal_moves == 0) {
        if (InCheck(board)) return CHECKMATE;
        return 0; // Stalemate
    }

    return best_score;
}

int search(Board *board, int depth_limit, int node_limit, int soft_node_limit, int time_limit) {
    if (depth_limit == -1) depth_limit = 255;
    if (node_limit == -1) node_limit = INT_MAX;
    if (soft_node_limit == -1) node_limit = INT_MAX;
    if (time_limit == -1) time_limit = INT_MAX;
    Stack stack = {
        .nodes = 0,
        .start_time = clock(),
        .time_limit = time_limit,
        .node_limit = node_limit
    };
    Move best_move;
    int best_score;
    bool quit = false;
    for (int depth = 1; depth <= depth_limit && !quit; depth++) {

        Move move;

        int score = Negamax(&stack, board, depth, true, &move);

        if (!(stack.nodes > node_limit || (clock() - stack.start_time) > time_limit)) {
            best_score = score;
            best_move = move;
        }
        else {
            quit = true;
        }


        const int time_elapsed = (int)(clock() - stack.start_time);
        printf("info depth %d", depth);
        printf(" score cp %d", best_score);
        printf(" nodes %llu", stack.nodes);
        printf(" nps %llu", stack.nodes * 1000 / (time_elapsed == 0 ? 1 : time_elapsed));
        printf(" time %d", time_elapsed);
        printf(" pv %s\n", MoveToString(best_move));
    }
    printf("bestmove %s\n", MoveToString(best_move));
    return best_score;
}