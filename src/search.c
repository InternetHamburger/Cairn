#include "search.h"
#include "utility.h"
#include "board.h"
#include "evaluation.h"
#include "move.h"
#include "transposition.h"
#include "moveGeneration.h"
#include "zobrist.h"
#include "moveOrderer.h"
#include "movepicker.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#include "nnue.h"

#define CLAMP(x, a, b) __min(__max(x, a), b)

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

bool is_mate_score(int score){
    return abs(score) > -(CHECKMATE + 255);
}

void update_entry(int* entry, int bonus){
    *entry += bonus - abs(bonus) * *entry / 4096;
}

void update_corrhist(Thread* thread, int depth, int bonus){
    bonus *= 256 * depth / 8;
    bonus = CLAMP(bonus, -1024, 1024);
    int color = thread->board.white_to_move;

    int* entry = &thread->pawn_corr_hist[color][thread->board.pawn_key % 16384];
    update_entry(entry, bonus);
    entry = &thread->non_pawn_corr_hist[0][color][thread->board.non_pawn_key[0] % 16384];
    update_entry(entry, bonus);
    entry = &thread->non_pawn_corr_hist[1][color][thread->board.non_pawn_key[1] % 16384];
    update_entry(entry, bonus);
    entry = &thread->minor_corr_hist[color][thread->board.minor_key % 16384];
    update_entry(entry, bonus);
}

int correct_eval(Thread* thread, int eval){
    int color = thread->board.white_to_move;

    int pawn_entry = thread->pawn_corr_hist[color][thread->board.pawn_key % 16384];
    int s_non_pawn_entry = thread->non_pawn_corr_hist[0][color][thread->board.non_pawn_key[0] % 16384];
    int n_non_pawn_entry = thread->non_pawn_corr_hist[1][color][thread->board.non_pawn_key[1] % 16384];
    int minor_entry = thread->minor_corr_hist[color][thread->board.minor_key % 16384];

    int correction = 0;
    correction += 256 * pawn_entry;
    correction += 256 * s_non_pawn_entry;
    correction += 256 * n_non_pawn_entry;
    correction += 256 * minor_entry;

    int corrected = eval + correction / (256 * 128);
    return CLAMP(corrected, CHECKMATE + 256, -(CHECKMATE + 256));
}

int correct_score(int score, int ply){
    if (is_mate_score(score)){
        score = (abs(score) + ply) * (score < 0 ? -1 : 1);
    }
    return score;
}

int qSearch(Thread *thread, int alpha, int beta, int ply){
    Board* board = &thread->board;
    const uint64_t tt_index = board->zobrist_hash % thread->tt.num_entries;
    __builtin_prefetch(&thread->tt.entries[tt_index]);

    const int static_eval = correct_eval(thread, nnueval(board));

    const bool is_pv = beta - alpha > 1;
    const Entry entry = thread->tt.entries[tt_index];
    const bool tt_hit = board->zobrist_hash == entry.hash;
    const int tt_score = correct_score(entry.score, -ply);

    if (is_pv && ply > thread->seldepth){
        thread->seldepth = ply;
    }

    if (!is_pv && tt_hit) {
        int type = GetEntryType(entry);
        if (type == EXACT)
            return tt_score;
        if (type == LOWER && tt_score >= beta)
            return tt_score;
        if (type == UPPER && tt_score <= alpha)
            return tt_score;
    }


    int best_score = tt_hit ? tt_score : static_eval;
    if( best_score >= beta )
        return best_score;
    if( best_score > alpha )
        alpha = best_score;

    Move moves[256];
    int num_moves = GetMoves(board, moves);
    OrderCaptures(thread, moves, num_moves);

    const Board copy = *board;
    Move best_move = MoveConstructor(0, 0, 0);
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

        const int score = -qSearch(thread, -beta, -alpha, ply + 1);

        *board = copy;

        if (score > best_score) {
            best_score = score;
            if (score > alpha){
                best_move = moves[i];
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
        .best_move = best_move,
        .score = (int16_t)correct_score(best_score, ply),
        .depth_node_type = type | 0
    };
    thread->tt.entries[tt_index] = new_entry;
    return best_score;
}

int Negamax(Thread *thread, int alpha, int beta, int depth, int ply, bool cutnode, PVariation *pv) {
    Board* board = &thread->board;
    const uint64_t tt_index = board->zobrist_hash % thread->tt.num_entries;
    MovePicker* mp = &thread->ss[ply].mp;
    __builtin_prefetch(&thread->tt.entries[tt_index]);

    const bool in_check = InCheck(board);
    PVariation lpv;

    pv->length = 0;

    if (in_check)
        depth++;
    if (depth <= 0) return qSearch(thread, alpha, beta, ply);
    thread->hashes[board->game_ply] = board->zobrist_hash;
    if (IsDraw(thread->hashes, board) && ply > 0){
        return 0;
    }
    const bool is_singular = thread->ss[ply].excluded.value != 0;
    const bool is_pv = beta - alpha > 1;
    const Entry entry = thread->tt.entries[tt_index];
    const uint8_t tt_flag = GetEntryType(entry);
    const bool tt_hit = !is_singular && board->zobrist_hash == entry.hash;
    const int tt_depth = GetDepth(entry);
    const Move tt_move = tt_hit ? entry.best_move : MoveConstructor(0, 0, 0);
    const int tt_score = correct_score(entry.score, -ply);

    if (is_pv && ply > thread->seldepth){
        thread->seldepth = ply;
    }

    if (!is_singular && tt_depth >= depth && ply > 0 && tt_hit && !is_pv)
    {
        if (tt_flag == EXACT)
            return tt_score;
        if (tt_flag == LOWER && tt_score >= beta)
            return tt_score;
        if (tt_flag == UPPER && tt_score <= alpha)
            return tt_score;
    }

    int static_eval = in_check ? -NEG_INF : correct_eval(thread, nnueval(board));
    thread->ss[ply].static_eval = static_eval;

    bool improving = false;
    if (in_check) {
        improving = false;
    } else if (ply >= 2 && thread->ss[ply - 2].static_eval != -NEG_INF) {
        improving = static_eval > thread->ss[ply - 2].static_eval;
    } else if (ply >= 4 && thread->ss[ply - 4].static_eval != -NEG_INF) {
        improving = static_eval > thread->ss[ply - 4].static_eval;
    }

    if (!is_mate_score(beta) && !is_singular && static_eval >= beta + 60 * depth && !in_check && !is_pv)
    {
        return static_eval;
    }

    const Board copy = *board;
    if (!is_mate_score(beta) && !is_singular && !is_pv && !in_check && depth >= 3 && HasNonPawnKing(board) && static_eval >= beta){
        int r = 3 + depth / 4 + improving;
        thread->ss[ply].to_square = 0;
        thread->ss[ply].moved_piece = None;
        MakeNullMove(board);
        PVariation null_pv;
        int score = -Negamax(thread, -beta, -beta + 1, depth - r, ply + 1, !cutnode, &null_pv);
        *board = copy;
        if (score >= beta){
            return score > -(CHECKMATE + 255) ? beta : score;
        }
    }

    int extension = 0;
    if (ply > 0 && depth >= 7 && !is_singular && tt_depth >= depth - 3 && tt_flag != UPPER)
    {
        const int singular_beta = __max(NEG_INF + 1, tt_score - depth);
        const int singular_depth = depth / 2;

        thread->ss[ply].excluded = tt_move;
        int score = Negamax(thread, singular_beta - 1, singular_beta, singular_depth, ply, cutnode, &lpv);
        thread->ss[ply].excluded.value = 0;

        if (score < singular_beta)
        {
            extension++;
            if (score < singular_beta - 40)
            {
                extension++;
            }
        }
        else if (singular_beta >= beta){
            return singular_beta;
        }
        else if (cutnode){
            extension--;
        }
    }

    if (tt_move.value == 0 && depth > 4 && is_pv)
    {
        depth--;
    }

    int num_legal_moves = 0;

    Move quiet_moves[218];
    Move captures[218];

    int num_quiets = 0;
    int num_captures = 0;
    int played = -1;

    int best_score = NEG_INF;
    Move best_move = MoveConstructor(0, 0, 0);
    uint8_t new_flag = UPPER;

    init_picker(mp, thread, ply, tt_move);
    Move move;
    while ((move = next_move(mp, thread, ply)).value != 0) {
        played++;
        if (thread->ss[ply].excluded.value == move.value) continue;
        const bool is_capture = board->squares[TargetSquare(move)] != None;
        thread->ss[ply].to_square = TargetSquare(move);
        thread->ss[ply].moved_piece = board->squares[StartSquare(move)];
        // Move loop pruning
        if (best_score > CHECKMATE + 255)
        {
            if (depth <= 4 && !in_check && !is_capture && get_history(thread, move, ply) < depth * -2048) {
                continue;
            }

            if (ply > 0 && !in_check && !is_capture && played >= (6 + depth * depth) / (2 - improving))
            {
                continue;
            }

            if (num_legal_moves > 0 && !in_check && !is_capture && depth <= 5 && static_eval + 125 + 200 * depth < alpha)
            {
                continue;
            }

            if (ply > 0 && depth <= 8 && !staticExchangeEvaluation(board, move, (is_capture ? -50 * depth : -60) * depth)){
                continue;
            }
        }


        if (GetFlag(move) == Castle && !IsLegalCastle(board, move)){
            continue;
        }
        assert(move.value != 0);
        MakeMove(board, move);
        if (IsAttackedBySideToMove(board, board->white_to_move, board->white_to_move ? board->black_king_square : board->white_king_square)) {
            *board = copy;
            continue;
        }

        if (!is_capture)
        {
            quiet_moves[num_quiets++] = move;
        }
        else
        {
            captures[num_captures++] = move;
        }

        thread->nodes++;
        num_legal_moves++;

        int score;
        int lmr_depth = depth - 1 + extension;
        if (played == 0)
        {
            score = -Negamax(thread, -beta, -alpha, lmr_depth, ply + 1, false, &lpv);
        }
        else if (depth >= 3 && played >= 2 + (ply == 0))
        {
            int r = lmr_reduction[depth][num_legal_moves];
            r -= is_pv;
            r -= is_capture * 2;
            r -= thread->killer_moves[ply].value == move.value;
            r -= improving;
            r += 2 * cutnode;
            r = __max(r, 0);
            score = -Negamax(thread, -alpha - 1, -alpha, lmr_depth - r, ply + 1, true, &lpv);
            if (score > alpha && is_pv)
            {
                score = -Negamax(thread, -alpha - 1, -alpha, lmr_depth, ply + 1, !cutnode, &lpv);
            }
            if (score > alpha)
            {
                score = -Negamax(thread, -beta, -alpha, lmr_depth, ply + 1, false, &lpv);
            }
        }
        else
        {
            score = -Negamax(thread, -alpha - 1, -alpha, lmr_depth, ply + 1, !cutnode, &lpv);
            if (score > alpha && is_pv)
            {
                score = -Negamax(thread, -beta, -alpha, lmr_depth, ply + 1, false, &lpv);
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
                new_flag = EXACT;
                best_move = move;
                alpha = score;
                pv->length = 1 + lpv.length;
                pv->line[0] = move;
                memcpy(pv->line + 1, lpv.line, sizeof(Move) * lpv.length);
            }
        }

        if (score >= beta) {
            new_flag = LOWER;
            if (!is_capture){
                UpdateKillers(thread, move, ply);
                for (int j = 0; j < num_quiets; j++)
                {
                    if (quiet_moves[j].value == move.value)
                    {
                        UpdateHistTable(thread, ply, move, 300 * depth - 250);
                    }else
                    {
                        UpdateHistTable(thread, ply, quiet_moves[j], -(300 * depth - 250));
                    }
                }
            }
            else
            {
                for (int j = 0; j < num_captures; j++)
                {
                    if (captures[j].value == move.value)
                    {
                        update_caphist(thread, move, 300 * depth - 250);
                    }else
                    {
                        update_caphist(thread, captures[j], -(300 * depth - 250));
                    }
                }
            }
            break;
        }
    }
    if (num_legal_moves == 0) {
        if (InCheck(board)) return CHECKMATE + ply;
        return 0; // Stalemate
    }

    if (!is_singular)
    {
        Entry new_entry = {
            .hash = board->zobrist_hash,
            .best_move = best_move,
            .score = (int16_t)correct_score(best_score, ply),
            .depth_node_type = new_flag | depth
        };

        thread->tt.entries[tt_index] = new_entry;

        const bool is_capture = board->squares[TargetSquare(best_move)] != None;
        if (!in_check && (best_move.value == 0 || !is_capture) && (
                new_flag == EXACT ||
                (new_flag == LOWER && static_eval < best_score) ||
                (new_flag == UPPER && static_eval > best_score)))
        {
            update_corrhist(thread, depth, best_score - static_eval);
        }
    }


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
    printf("info depth %d seldepth %d", depth, thread->seldepth);
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
                if (thread->nodes > thread->soft_node_limit || (clock() - thread->start_time) > thread->soft_time_limit || thread->nodes > thread->node_limit) {
                    break;
                }
                score = Negamax(thread, alpha, beta, depth, 0, false, &pv);
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
            score = Negamax(thread, NEG_INF, -NEG_INF, depth, 0, false, &pv);
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
        if (thread->nodes > thread->soft_node_limit || (clock() - thread->start_time) > thread->soft_time_limit || thread->nodes > thread->node_limit) {
            break;
        }
    }

    Board* board = &thread->board;

    if (best_move.value == 0){

        Move moves[256];
        int num_moves = GetMoves(board, moves);


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