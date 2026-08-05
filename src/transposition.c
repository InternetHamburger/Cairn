#include "transposition.h"
#include <string.h>

int GetDepth(Entry entry){
    return entry.depth_node_type & 0b00111111;
}

int GetEntryType(Entry entry){
    return entry.depth_node_type & 0b11000000;
}

bool IsNull(Entry entry)
{
    return !(entry.depth_node_type & 0b11000000);
}

void ZeroTT(TT* tt){
    memset(tt->entries, 0, sizeof(Entry) * tt->num_entries);
}

void StoreTT(Entry prev_entry, uint64_t idx, int score, Move move, int depth, int type, Thread* thread)
{
    const bool tt_hit = prev_entry.hash == thread->board.zobrist_hash;

    move = type == UPPER && tt_hit ? prev_entry.best_move : move;

    const Entry entry = {
        .hash = thread->board.zobrist_hash,
        .best_move = move,
        .score = (int16_t)score,
        .depth_node_type = type | depth
    };
    thread->tt.entries[idx] = entry;
}