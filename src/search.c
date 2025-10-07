#include "search.h"
#include <limits.h>
#include "utility.h"
#include "board.h"
#include "evaluation.h"
#include "move.h"
#include "moveGeneration.h"
#include "zobrist.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

int qSearch(Stack *stack, Board *board, int alpha, int beta){
    int static_eval = eval(board);

    int best_score = static_eval;
    if( best_score >= beta )
        return best_score;
    if( best_score > alpha )
        alpha = best_score;

    int num_moves = 0;
    Move moves[256];
    GetMoves(board, moves, &num_moves);
    const Board copy = *board;

    for (int i = 0; i < num_moves; i++) {
        if (board->squares[TargetSquare(moves[i])] == 0) continue;
        if (GetFlag(moves[i]) == Castle && !IsLegalCastle(board, moves[i])){
            continue;
        }
        MakeMove(board, moves[i]);
        if (IsAttackedBySideToMove(board, board->white_to_move, board->white_to_move ? board->black_king_square : board->white_king_square)) {
            *board = copy;
            continue;
        }
        stack->nodes++;

        const int score = -qSearch(stack, board, -beta, -alpha);

        *board = copy;

        if (score > best_score) {
            best_score = score;
            if (score > alpha){
                alpha = score;
            }
        }

        if (score >= beta) {
            break;
        }
    }

    return best_score;
}

int Negamax(Stack *stack, Board *board, int alpha, int beta, int depth, int ply, bool isTop, Move *move) {
    if (depth == 0) return eval(board);
    stack->hashes[stack->hash_index] = board->zobrist_hash;
    if (IsRepetition(stack->hashes, stack->hash_index)){
        return 0;
    }

    int num_legal_moves = 0;

    int num_moves = 0;
    Move moves[256];
    GetMoves(board, moves, &num_moves);
    const Board copy = *board;

    int best_score = NEG_INF;
    for (int i = 0; i < num_moves; i++) {
        if (GetFlag(moves[i]) == Castle && !IsLegalCastle(board, moves[i])){
            continue;
        }
        assert(moves[i].value != 0);
        MakeMove(board, moves[i]);
        if (IsAttackedBySideToMove(board, board->white_to_move, board->white_to_move ? board->black_king_square : board->white_king_square)) {
            *board = copy;
            continue;
        }
        stack->nodes++;
        num_legal_moves++;
        stack->hash_index++;
        const int score = -Negamax(stack, board, -beta, -alpha, depth - 1, ply + 1, false, move);
        stack->hash_index--;
        *board = copy;

        if (stack->nodes > stack->node_limit || clock() - stack->start_time > stack->time_limit) {
            return NEG_INF;
        }

        if (score > best_score) {
            best_score = score;
            if (score > alpha){
                alpha = score;
            }
            if (isTop) {
                *move = moves[i];
            }
        }

        if (score >= beta) {
            break;
        }
    }
    if (num_legal_moves == 0) {
        if (InCheck(board)) return CHECKMATE;
        return 0; // Stalemate
    }

    return best_score;
}

SearchResult search(Board *board, Stack *stack) {

    Move best_move = MoveConstructor(0, 0, 0);
    int best_score = NEG_INF;
    bool quit = false;
    stack->start_time = clock();
    int depth = 1;
    for (depth = 1; depth <= stack->depth_limit && !quit; depth++) {
        Move move;

        int score = Negamax(stack, board, NEG_INF, -NEG_INF, depth, 0, true, &move);
        assert(move.value != 0);
        if (!(stack->nodes > stack->soft_node_limit || (clock() - stack->start_time) > stack->time_limit || stack->nodes > stack->node_limit)) {
            best_score = score;
            best_move = move;
        }
        else if (depth > 1) {
            quit = true;
        }


        const int time_elapsed = (int)(clock() - stack->start_time);
        if (stack->print_info){
        printf("info depth %d", depth);
        printf(" score cp %d", best_score);
        printf(" nodes %llu", stack->nodes);
        printf(" nps %llu", stack->nodes * 1000 / (time_elapsed == 0 ? 1 : time_elapsed));
        printf(" time %d", time_elapsed);
        printf(" pv %s\n", MoveToString(best_move));
        }
    }
    if (best_move.value == 0){
        int num_moves = 0;
        Move moves[256];
        GetMoves(board, moves, &num_moves);


        int num_legal_moves = 0;
        Move* legal_moves = malloc(sizeof(Move) * num_moves);
        const Board copy = *board;
        for (int i = 0; i < num_moves; i++) {
            if (GetFlag(moves[i]) == Castle && !IsLegalCastle(board, moves[i])){
                continue;
            }
            assert(moves[i].value != 0);
            MakeMove(board, moves[i]);
            if (IsAttackedBySideToMove(board, board->white_to_move, board->white_to_move ? board->black_king_square : board->white_king_square)) {
                *board = copy;
                continue;
            }
            legal_moves[num_legal_moves++] = moves[i];

            *board = copy;
        }
        if (num_legal_moves == 0) {
            exit(-2);
        }
        else {
            uint64_t seed = num_legal_moves;
            unsigned long long rand_index = PseudorandomNumber(&seed) % num_legal_moves;
            assert(legal_moves[rand_index].value != 0);
            best_move = legal_moves[rand_index];
        }
        free(legal_moves);
    }

    if (stack->print_info)
        printf("bestmove %s\n", MoveToString(best_move));

    SearchResult result = {
            .best_move = best_move,
            .score = best_score,
            .depth = depth,
            .nodes = stack->nodes
    };

    assert(best_move.value != 0);
    return result;
}