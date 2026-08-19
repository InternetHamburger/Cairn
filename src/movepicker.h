#ifndef CAIRN_MOVEPICKER_H
#define CAIRN_MOVEPICKER_H

#include "search.h"

enum {
    STAGE_TT,
    STAGE_GENERATE_CAPTURES,
    STAGE_GOOD_CAPTURES,
    STAGE_KILLERS,
    STAGE_GENERATE_QUIETS,
    STAGE_QUIETS,
    STAGE_BAD_CAPTURES
};



void init_picker(MovePicker* mp, Thread* thread, int ply, Move tt_move);
Move next_move(MovePicker* mp, Thread* thread, int ply);

#endif //CAIRN_MOVEPICKER_H
