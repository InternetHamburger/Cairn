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


constexpr int ASP_MIN_DEPTH = 5;

void Init(Thread* thread)
{
    ZeroKillers(thread);
}

int lmr_reduction[255][218]; // Indexed by [depth][move_num]

__attribute__((constructor))
static void init_table(){
    for (int depth = 1; depth < 255; depth++){
        for (int move_num = 1; move_num < 218; move_num++){
            lmr_reduction[depth][move_num] = (int)(1 + log2(depth) * log2(move_num) / 3);
        }
    }
}

int qSearch(Thread *thread, int alpha, int beta){
    Board* board = &thread->board;

    int static_eval = eval(board);

    const bool is_pv = beta - alpha > 1;
    const uint64_t tt_index = board->zobrist_hash % thread->tt.num_entries;
    const Entry entry = thread->tt.entries[tt_index];
    const bool tt_hit = board->zobrist_hash == entry.hash;

    if (!is_pv && tt_hit) {
        int type = GetEntryType(entry);
        if (((type & EXACT) == EXACT) ||
            ((type & UPPER) == UPPER && entry.score <= alpha) ||
            ((type & LOWER) == LOWER && entry.score >= beta)) {

            return entry.score;
        }
    }


    int best_score = tt_hit ? entry.score : static_eval;
    if( best_score >= beta )
        return best_score;
    if( best_score > alpha )
        alpha = best_score;

    int num_moves = 0;
    Move moves[256];
    GetMoves(board, moves, &num_moves);
    OrderCaptures(board, moves, num_moves);
    const Board copy = *board;

    for (int i = 0; i < num_moves; i++) {
        if (board->squares[TargetSquare(moves[i])] == 0) continue;
        if (GetFlag(moves[i]) == Castle && !IsLegalCastle(board, moves[i])){
            continue;
        }

        // Skip bad captures
        if (!staticExchangeEvaluation(board, moves[i], 0))
        {
            continue;
        }
        MakeMove(board, moves[i]);
        if (IsAttackedBySideToMove(board, board->white_to_move, board->white_to_move ? board->black_king_square : board->white_king_square)) {
            *board = copy;
            continue;
        }
        thread->nodes++;

        const int score = -qSearch(thread, -beta, -alpha);

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

    int type = EXACT;
    if (best_score >= beta)
    {
        type = LOWER;
    }
    else if (best_score < alpha)
    {
        type = UPPER;
    }

    Entry new_entry = {
        .hash = board->zobrist_hash,
        .best_move = MoveConstructor(0, 0, 0),
        .score = (int16_t)best_score,
        .depth_node_type = type | 0
    };
    thread->tt.entries[tt_index] = new_entry;
    return best_score;
}

int Negamax(Thread *thread, int alpha, int beta, int depth, int ply, PVariation *pv) {
    Board* board = &thread->board;

    const bool in_check = InCheck(board);
    PVariation lpv;

    pv->length = 0;

    if (in_check)
        depth++;
    if (depth <= 0) return qSearch(thread, alpha, beta);
    thread->hashes[board->game_ply] = board->zobrist_hash;
    if ((IsRepetition(thread->hashes, board->game_ply) || board->fifty_move_counter >= 100) && ply > 0){
        return 0;
    }
    const bool is_pv = beta - alpha > 1;

    const uint64_t tt_index = board->zobrist_hash % thread->tt.num_entries;
    const Entry entry = thread->tt.entries[tt_index];
    const bool tt_hit = board->zobrist_hash == entry.hash;

    if (GetDepth(entry) >= depth && ply > 0 && tt_hit && !is_pv)
    {
        if ((GetEntryType(entry) & EXACT) == EXACT)
            return entry.score;
        if ((GetEntryType(entry) & LOWER) == LOWER && entry.score >= beta)
            return entry.score;
        if ((GetEntryType(entry) & UPPER) == UPPER && entry.score <= alpha)
            return entry.score;
    }

    const int static_eval = in_check ? -NEG_INF : eval(board);
    thread->ss[ply].static_eval = static_eval;

    bool improving = false;
    if (!in_check && ply > 1 && static_eval > thread->ss[ply - 2].static_eval)
        improving = true;

    if (static_eval >= beta + 60 * depth && !in_check && !is_pv)
    {
        return static_eval;
    }

    const Board copy = *board;
    if (!is_pv && !in_check && depth >= 3 && HasNonPawnKing(board) && static_eval >= beta){
        int r = 3 + depth / 4;
        MakeNullMove(board);
        PVariation null_pv;
        int score = -Negamax(thread, -beta, -beta + 1, depth - r, ply + 1, &null_pv);
        *board = copy;
        if (score >= beta){
            return score > -(CHECKMATE + 255) ? beta : score;
        }
    }

    int num_legal_moves = 0;
    int num_moves = 0;
    Move tt_move = tt_hit ? entry.best_move : MoveConstructor(0, 0, 0);



    Move quiet_moves[218];

    int num_quiets = 0;



    int alphaOrig = alpha;
    int best_score = NEG_INF;
    Move best_move = MoveConstructor(0, 0, 0);

    if (tt_hit && tt_move.value)
    {
        const bool is_capture = board->squares[TargetSquare(tt_move)] != None;
        thread->ss[ply].to_square = TargetSquare(tt_move);
        thread->ss[ply].moved_piece = board->squares[StartSquare(tt_move)];

        assert(tt_move.value != 0);
        MakeMove(board, tt_move);

        if (!is_capture)
        {
            quiet_moves[num_quiets++] = tt_move;
        }

        thread->nodes++;
        num_legal_moves++;

        int score = -Negamax(thread, -beta, -alpha, depth - 1, ply + 1, &lpv);

        *board = copy;

        if (thread->nodes > thread->node_limit || clock() - thread->start_time > thread->time_limit) {
            if (ply == 0)
                return best_score;
            return NEG_INF;
        }

        if (score > best_score) {
            best_score = score;
            if (score > alpha){
                best_move = tt_move;
                alpha = score;
                pv->length = 1 + lpv.length;
                pv->line[0] = tt_move;
                memcpy(pv->line + 1, lpv.line, sizeof(Move) * lpv.length);
            }
        }

        if (score >= beta) {
            if (!is_capture){
                UpdateKillers(thread, tt_move, ply);
                for (int j = 0; j < num_quiets; j++)
                {
                    if (quiet_moves[j].value == tt_move.value)
                    {
                        UpdateHistTable(thread, ply, tt_move, depth * depth);
                    }else
                    {
                        UpdateHistTable(thread, ply, quiet_moves[j], -depth * depth);
                    }
                }
            }
            const Entry new_entry = {
                    .hash = board->zobrist_hash,
                    .best_move = best_move,
                    .score = (int16_t)best_score,
                    .depth_node_type = LOWER | depth
            };
            thread->tt.entries[tt_index] = new_entry;

            return best_score;
        }
    }

    Move moves[256];
    GetMoves(board, moves, &num_moves);
    OrderMoves(thread, moves, num_moves, ply, tt_move);

    for (int i = 0; i < num_moves; i++) {
        if (moves[i].value == tt_move.value) continue;
        const bool is_capture = board->squares[TargetSquare(moves[i])] != None;
        thread->ss[ply].to_square = TargetSquare(moves[i]);
        thread->ss[ply].moved_piece = board->squares[StartSquare(moves[i])];
        // Move loop pruning
        if (best_score > CHECKMATE + 255)
        {
            if (ply > 0 && !in_check && !is_capture && i >= 6 + depth * depth)
            {
                continue;
            }

            if (num_legal_moves > 0 && !in_check && !is_capture && depth <= 5 && static_eval + 125 + 200 * depth < alpha)
            {
                continue;
            }

            if (ply > 0 && depth <= 8 && !staticExchangeEvaluation(board, moves[i], (is_capture ? -90 : -60) * depth)){
                continue;
            }
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

        thread->nodes++;
        num_legal_moves++;

        int score;
        if (i == 0)
        {
            score = -Negamax(thread, -beta, -alpha, depth - 1, ply + 1, &lpv);
        }
        else if (depth >= 3)
        {
            int r = lmr_reduction[depth][num_legal_moves];
            r -= is_pv;
            r -= is_capture * 2;
            r -= thread->killer_moves[ply].value == moves[i].value;
            r -= improving;
            r = __max(r, 0);
            score = -Negamax(thread, -alpha - 1, -alpha, depth - 1 - r, ply + 1, &lpv);
            if (score > alpha && is_pv)
            {
                score = -Negamax(thread, -alpha - 1, -alpha, depth - 1, ply + 1, &lpv);
            }
            if (score > alpha)
            {
                score = -Negamax(thread, -beta, -alpha, depth - 1, ply + 1, &lpv);
            }
        }
        else
        {
            score = -Negamax(thread, -alpha - 1, -alpha, depth - 1, ply + 1, &lpv);
            if (score > alpha && is_pv)
            {
                score = -Negamax(thread, -beta, -alpha, depth - 1, ply + 1, &lpv);
            }
        }
        *board = copy;

        if (thread->nodes > thread->node_limit || clock() - thread->start_time > thread->time_limit) {
            if (ply == 0)
                return best_score;
            return NEG_INF;
        }

        if (score > best_score) {
            best_score = score;
            if (score > alpha){
                best_move = moves[i];
                alpha = score;
                pv->length = 1 + lpv.length;
                pv->line[0] = moves[i];
                memcpy(pv->line + 1, lpv.line, sizeof(Move) * lpv.length);
            }
        }

        if (score >= beta) {
            if (!is_capture){
                UpdateKillers(thread, moves[i], ply);
                for (int j = 0; j < num_quiets; j++)
                {
                    if (quiet_moves[j].value == moves[i].value)
                    {
                        UpdateHistTable(thread, ply, moves[i], 300 * depth - 250);
                    }else
                    {
                        UpdateHistTable(thread, ply, quiet_moves[j], -(300 * depth - 250));
                    }
                }
            }
            const Entry new_entry = {
                    .hash = board->zobrist_hash,
                    .best_move = best_move,
                    .score = (int16_t)best_score,
                    .depth_node_type = LOWER | depth
            };
            thread->tt.entries[tt_index] = new_entry;

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
    if (best_score <= alphaOrig)
    {
        new_entry.depth_node_type = UPPER | depth;
    }
    else
    {
        new_entry.depth_node_type = EXACT | depth;
    }
    thread->tt.entries[tt_index] = new_entry;
    return best_score;
}

int CountHashFull(Thread* thread)
{
    int num = 0;
    for (int i = 0; i < 1000; i++)
    {
        num += !IsNull(thread->tt.entries[i]);
    }
    return num;
}

void UCIReport(Thread *thread, PVariation *lpv, int depth, int score, int time_elapsed)
{
    printf("info depth %d", depth);
    if (abs(score) > -(CHECKMATE + 255))
    {
        printf(" score mate %d", (score < 0 ? -1 : 1) * (-CHECKMATE - abs(score) + 1) / 2);
    }else
    {
        printf(" score cp %d", score);
    }
    printf(" nodes %llu", thread->nodes);
    printf(" nps %llu", thread->nodes * 1000 / (time_elapsed == 0 ? 1 : time_elapsed));
    printf(" hashfull %d", CountHashFull(thread));
    printf(" time %d", time_elapsed);

    printf(" pv ");
    for (int i = 0; i < lpv->length; i++) {
        char* moveStr = MoveToString(lpv->line[i]);
        printf("%s ", moveStr);
    }
    printf("\n");
}

SearchResult search(Thread *thread) {
    Init(thread);
    Move best_move = MoveConstructor(0, 0, 0);
    int best_score = NEG_INF;
    thread->start_time = clock();



    PVariation lpv;

    int alpha = NEG_INF;
    int beta = -NEG_INF;

    int depth;
    for (depth = 1; depth <= thread->depth_limit; depth++) {
        PVariation pv;

        int score = NEG_INF;
        if (depth >= ASP_MIN_DEPTH)
        {
            int delta = 20;
            alpha = __max(best_score - delta, NEG_INF);
            beta = __min(best_score + delta, -NEG_INF);
            while (1)
            {
                if (thread->nodes > thread->soft_node_limit || (clock() - thread->start_time) > thread->time_limit || thread->nodes > thread->node_limit) {
                    break;
                }
                score = Negamax(thread, alpha, beta, depth, 0, &pv);
                delta += delta;
                if (score <= alpha)
                {
                    beta = (alpha + beta) / 2;
                    alpha = __max(best_score - delta, NEG_INF);
                }
                else if (score >= beta)
                {
                    beta = __min(best_score + delta, -NEG_INF);
                }
                else
                {
                    break;
                }

                if (delta >= 500)
                {
                    alpha = NEG_INF;
                    beta = -NEG_INF;
                }
            }
        }
        else
        {
            score = Negamax(thread, NEG_INF, -NEG_INF, depth, 0, &pv);
        }
        best_move = pv.line[0];
        if (score != NEG_INF){
            best_score = score;
            lpv = pv;
        }

        const int time_elapsed = (int)(clock() - thread->start_time);
        if (thread->print_info){
            UCIReport(thread, &lpv, depth, best_score, time_elapsed);
        }
        assert(lpv.line[0].value != 0);
        if (thread->nodes > thread->soft_node_limit || (clock() - thread->start_time) > thread->time_limit || thread->nodes > thread->node_limit) {
            break;
        }
    }

    Board* board = &thread->board;

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

    if (thread->print_info)
        printf("bestmove %s\n", MoveToString(best_move));

    SearchResult result = {
            .best_move = best_move,
            .score = best_score,
            .depth = depth,
            .nodes = thread->nodes
    };

    assert(best_move.value != 0);
    return result;
}