#include "search.h"
#include "utility.h"
#include "board.h"
#include "evaluation.h"
#include "move.h"
#include "transposition.h"
#include "moveGeneration.h"
#include "zobrist.h"
#include "moveOrderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <string.h>

void Init()
{
    ZeroHist();
}

int lmr_reduction[255][218]; // Indexed by [depth][move_num]

__attribute__((constructor))
static void init_table(){
    for (int depth = 1; depth < 255; depth++){
        for (int move_num = 1; move_num < 218; move_num++){
            lmr_reduction[depth][move_num] = (int)(1 + log(depth) * log(move_num) / 3);
        }
    }
}

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
    OrderMoves(board, moves, num_moves, 0, MoveConstructor(0, 0, 0));
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

int Negamax(Stack *stack, Board *board, int alpha, int beta, int depth, int ply, PVariation *pv) {

    const bool in_check = InCheck(board);
    PVariation lpv;

    pv->length = 0;

    if (in_check)
        depth++;
    if (depth <= 0) return qSearch(stack, board, alpha, beta);
    stack->hashes[stack->hash_index] = board->zobrist_hash;
    if (IsRepetition(stack->hashes, stack->hash_index) && ply > 0){
        return 0;
    }
    const bool is_pv = beta - alpha > 1;

    const uint64_t tt_index = board->zobrist_hash % tt.num_entries;
    const Entry entry = tt.entries[tt_index];

    if (GetDepth(entry) >= depth && ply > 0 && board->zobrist_hash == entry.hash && !is_pv)
    {
        if (GetEntryType(entry) == EXACT)
        {
            return entry.score;
        }
    }

    const int static_eval = eval(board);
    if (static_eval >= beta + 150 * depth && !in_check)
    {
        return static_eval;
    }

    int num_legal_moves = 0;
    int num_moves = 0;
    Move moves[256];
    GetMoves(board, moves, &num_moves);

    Move quiet_moves[num_moves];
    int num_quiets = 0;

    Move tt_move = board->zobrist_hash == entry.hash ? entry.best_move : MoveConstructor(0, 0, 0);
    OrderMoves(board, moves, num_moves, ply, tt_move);

    const Board copy = *board;
    bool alpha_raised = false;
    int best_score = NEG_INF;
    Move best_move = MoveConstructor(0, 0, 0);

    for (int i = 0; i < num_moves; i++) {

        const bool is_capture = board->squares[TargetSquare(moves[i])] != None;
        // Late move pruning
        if (!in_check && !is_capture && i >= 5 + 2 * depth * depth)
        {
            continue;
        }

        if (GetFlag(moves[i]) == Castle && !IsLegalCastle(board, moves[i])){
            continue;
        }
        assert(moves[i].value != 0);
        MakeMove(board, moves[i]);
        if (IsAttackedBySideToMove(board, board->white_to_move, board->white_to_move ? board->black_king_square : board->white_king_square)) {
            *board = copy;
            continue;
        }

        if (!is_capture)
        {
            quiet_moves[num_quiets++] = moves[i];
        }

        stack->nodes++;
        num_legal_moves++;

        stack->hash_index++;
        int score;
        if (i == 0)
        {
            score = -Negamax(stack, board, -beta, -alpha, depth - 1, ply + 1, &lpv);
        }
        else
        {
            score = -Negamax(stack, board, -alpha - 1, -alpha, depth - 1 - lmr_reduction[depth][num_legal_moves], ply + 1, &lpv);
            if (score > alpha)
            {
                score = -Negamax(stack, board, -beta, -alpha, depth - 1, ply + 1, &lpv);
            }
        }
        stack->hash_index--;
        *board = copy;

        if (stack->nodes > stack->node_limit || clock() - stack->start_time > stack->time_limit) {
            return NEG_INF;
        }

        if (score > best_score) {
            alpha_raised = true;
            best_score = score;
            best_move = moves[i];
            if (score > alpha){
                alpha = score;
                pv->length = 1 + lpv.length;
                pv->line[0] = moves[i];
                memcpy(pv->line + 1, lpv.line, sizeof(Move) * lpv.length);
            }
        }

        if (score >= beta) {
            if (!is_capture){
                UpdateKillers(moves[i], ply);
                for (int j = 0; j < num_quiets; j++)
                {
                    if (quiet_moves[j].value == moves[i].value)
                    {
                        UpdateHistTable(board, moves[i], depth * depth);
                    }else
                    {
                        UpdateHistTable(board, quiet_moves[j], -depth * depth);
                    }
                }
            }
            const Entry new_entry = {
                    .hash = board->zobrist_hash,
                    .best_move = best_move,
                    .score = (int16_t)best_score,
                    .depth_node_type = LOWER | depth
            };
            tt.entries[tt_index] = new_entry;

            return best_score;
        }
    }
    if (num_legal_moves == 0) {
        if (InCheck(board)) return CHECKMATE + ply;
        return 0; // Stalemate
    }
    Entry new_entry = {
            .hash = board->zobrist_hash,
            .best_move = best_move,
            .score = (int16_t)best_score,
    };
    if (alpha_raised)
    {
        new_entry.depth_node_type = EXACT | depth;
    }
    else
    {
        new_entry.depth_node_type = UPPER | depth;
    }
    tt.entries[tt_index] = new_entry;
    return best_score;
}

SearchResult search(Board *board, Stack *stack) {
    Init();
    Move best_move = MoveConstructor(0, 0, 0);
    int best_score = NEG_INF;
    stack->start_time = clock();
    int depth;

    for (depth = 1; depth <= stack->depth_limit; depth++) {
        PVariation pv;
        const int score = Negamax(stack, board, NEG_INF, -NEG_INF, depth, 0, &pv);
        assert(pv.line[0].value != 0);
        if (!(stack->nodes > stack->soft_node_limit || (clock() - stack->start_time) > stack->time_limit || stack->nodes > stack->node_limit)) {
            best_score = score;
            best_move = pv.line[0];
        }else{
            break;
        }


        const int time_elapsed = (int)(clock() - stack->start_time);
        if (stack->print_info){
            printf("info depth %d", depth);
            printf(" score cp %d", best_score);
            printf(" nodes %llu", stack->nodes);
            printf(" nps %llu", stack->nodes * 1000 / (time_elapsed == 0 ? 1 : time_elapsed));
            printf(" time %d", time_elapsed);

            printf(" pv ");
            for (int i = 0; i < pv.length; i++) {
                char* moveStr = MoveToString(pv.line[i]);
                printf("%s ", moveStr);
            }
            printf("\n");
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
        uint64_t seed = num_legal_moves;
        unsigned long long rand_index = PseudorandomNumber(&seed) % num_legal_moves;
        assert(legal_moves[rand_index].value != 0);
        best_move = legal_moves[rand_index];
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