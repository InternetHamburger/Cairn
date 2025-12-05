#include "transposition.h"

TT tt;


int GetDepth(Entry entry){
    return entry.depth_node_type & 0b00111111;
}

int GetEntryType(Entry entry){
    return (entry.depth_node_type & 0b11000000) >> 6;
}

bool IsNull(Entry entry)
{
    return !(entry.depth_node_type & 0b11000000);
}

void ZeroTT(){
    for (int i = 0; i < tt.num_entries; i++) {
        tt.entries[i].hash = 0;
        tt.entries[i].best_move.value = 0;
        tt.entries[i].score = 0;
        tt.entries[i].depth_node_type = 0;
    }
}