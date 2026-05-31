#ifndef CAIRN_MOVEPICKER_H
#define CAIRN_MOVEPICKER_H

#include "board.h"
#include "search.h"

enum {
    STAGE_TT,
    STAGE_GENERATE_MOVES,
    STAGE_MOVES
};



void init_picker(MovePicker* mp, Thread* thread, int ply, Move tt_move);
Move next_move(MovePicker* mp, Thread* thread, int ply);

#endif //CAIRN_MOVEPICKER_H
