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