#include <stdlib.h>

#include "utility.h"
#include "preComputedData.h"

#include <stdlib.h>
#include "utility.h"
#include "preComputedData.h"
void SplatPawnMoves(Move* moves, int* num_moves, int offset, int flag, uint64_t bb)
{
    while(bb){
        const int target_square = poplsb(&bb);
        moves[(*num_moves)++] = MoveConstructor(target_square + offset, target_square, flag);
    }
}
void SplatPinnedPawnMoves(Move* moves, int* num_moves, int offset, int flag, int king_sq, uint64_t bb)
{
    while(bb){
        const int target_square = poplsb(&bb);
        if (1ULL << target_square & lines[king_sq][target_square + offset])
            moves[(*num_moves)++] = MoveConstructor(target_square + offset, target_square, flag);
    }
}

void GetRookMoves(const Board* board, const uint64_t filter, int king_sq, Move *moves, int *num_moves, int square, uint64_t occupied){
    uint64_t move_bb = rook_attack(occupied, square) & filter;

    if (1ULL << square & board->pins)
    {
        move_bb &= lines[square][king_sq];
    }
    while(move_bb){
        const int target_square = poplsb(&move_bb);
        moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
    }
}

void GetBishopMoves(const Board* board, const uint64_t filter, int king_sq, Move *moves, int *num_moves, int square, uint64_t occupied){
    uint64_t move_bb = bishop_attack(occupied, square) & filter;

    if (1ULL << square & board->pins)
    {
        move_bb &= lines[square][king_sq];
    }
    while(move_bb){
        const int target_square = poplsb(&move_bb);
        moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
    }
}

void GetKnightMoves(const Board* board, const uint64_t filter, Move *moves, int *num_moves, uint64_t knights){
    knights &= ~board->pins;
    while (knights){
        const int sq = poplsb(&knights);
        uint64_t bitboard = knight_moves[sq] & filter;

        while(bitboard){
            const int target_square = poplsb(&bitboard);
            moves[(*num_moves)++] = MoveConstructor(sq, target_square, 0);
        }
    }
}
void GetKingMoves(const Board *board, const uint64_t check_lines, Move *moves, int *num_moves, int square, uint64_t friendly_pieces){
    uint64_t bitboard = king_moves[square] & ~friendly_pieces & ~board->threat_bb;
    uint64_t occupied = GetOccupied(board);
    while(bitboard){
        const int target_square = poplsb(&bitboard);
        moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
    }
    uint64_t castle_bb;
    if (check_lines)
    {
        return;
    }
    if (board->white_to_move){
        castle_bb = 1Ull << 61 | 1ULL << 62;
        if (board->white_kingside && !((occupied | board->threat_bb) & castle_bb)){
            moves[(*num_moves)++] = MoveConstructor(square, 62, Castle);
        }
        castle_bb = 1Ull << 57 | 1ULL << 58 | 1ULL << 59;
        if (board->white_queenside && !(occupied & castle_bb) && !(board->threat_bb & (1ULL << 58 | 1ULL << 59))){
            moves[(*num_moves)++] = MoveConstructor(square, 58, Castle);
        }
    }else{
        castle_bb = 1Ull << 5 | 1ULL << 6;
        if (board->black_kingside && !((occupied | board->threat_bb) & castle_bb)){
            moves[(*num_moves)++] = MoveConstructor(square, 6, Castle);
        }
        castle_bb = 1Ull << 1 | 1ULL << 2 | 1ULL << 3;
        if (board->black_queenside && !(occupied & castle_bb) && !(board->threat_bb & (1ULL << 2 | 1ULL << 3))){
            moves[(*num_moves)++] = MoveConstructor(square, 2, Castle);
        }
    }
}
void GetPawnMoves(const Board *board, const uint64_t check_lines, Move *moves, int *num_moves) {
    const bool can_en_passant = board->en_passant_square != -1;
    const int king_sq = getlsb(board->color_bbs[!board->white_to_move] & board->piece_bbs[King]);
    const uint64_t king_rank = first_rank >> (8 * GetRank(FlipRank(king_sq)));
    const uint64_t occupied = GetOccupied(board);
    const uint64_t empty = ~occupied;
    const uint64_t enemy_pieces = board->color_bbs[board->white_to_move];
    const uint64_t check_mask = check_lines ? check_lines : ~0ULL;
    const uint64_t capture_right_mask = ~(a_file << 7);
    const uint64_t capture_left_mask = ~a_file;
    const uint64_t promotion_rank = board->white_to_move ? first_rank >> 48 : first_rank >> 8;
    const uint64_t double_push_rank = board->white_to_move ? first_rank >> 8 : first_rank >> 48;
    uint64_t moved_pawns;
    // Non pinned pawns
    uint64_t pawns = board->color_bbs[!board->white_to_move] & board->piece_bbs[Pawn] & ~board->pins;
    if (board->white_to_move)
    {
        moved_pawns = (pawns & ~promotion_rank) >> 8 & empty & check_mask;
        SplatPawnMoves(moves, num_moves, 8, 0, moved_pawns);
        moved_pawns = (pawns & double_push_rank) >> 16 & empty >> 8 & empty & check_mask;
        SplatPawnMoves(moves, num_moves, 16, DoublePush, moved_pawns);
        moved_pawns = (pawns & ~promotion_rank & capture_left_mask) >> 9 & enemy_pieces & check_mask;
        SplatPawnMoves(moves, num_moves, 9, 0, moved_pawns);
        moved_pawns = (pawns & ~promotion_rank & capture_right_mask) >> 7 & enemy_pieces & check_mask;
        SplatPawnMoves(moves, num_moves, 7, 0, moved_pawns);
        const uint64_t promotion_pawns = pawns & promotion_rank;
        moved_pawns = promotion_pawns >> 8 & empty & check_mask;
        SplatPawnMoves(moves, num_moves, 8, PromoteQueen, moved_pawns);
        SplatPawnMoves(moves, num_moves, 8, PromoteKnight, moved_pawns);
        SplatPawnMoves(moves, num_moves, 8, PromoteRook, moved_pawns);
        SplatPawnMoves(moves, num_moves, 8, PromoteBishop, moved_pawns);
        moved_pawns = (promotion_pawns & capture_left_mask) >> 9 & enemy_pieces & check_mask;
        SplatPawnMoves(moves, num_moves, 9, PromoteQueen, moved_pawns);
        SplatPawnMoves(moves, num_moves, 9, PromoteKnight, moved_pawns);
        SplatPawnMoves(moves, num_moves, 9, PromoteRook, moved_pawns);
        SplatPawnMoves(moves, num_moves, 9, PromoteBishop, moved_pawns);
        moved_pawns = (promotion_pawns & capture_right_mask) >> 7 & enemy_pieces & check_mask;
        SplatPawnMoves(moves, num_moves, 7, PromoteQueen, moved_pawns);
        SplatPawnMoves(moves, num_moves, 7, PromoteKnight, moved_pawns);
        SplatPawnMoves(moves, num_moves, 7, PromoteRook, moved_pawns);
        SplatPawnMoves(moves, num_moves, 7, PromoteBishop, moved_pawns);
    }
    else
    {
        moved_pawns = (pawns & ~promotion_rank) << 8 & empty & check_mask;
        SplatPawnMoves(moves, num_moves, -8, 0, moved_pawns);
        moved_pawns = (pawns & double_push_rank) << 16 & empty << 8 & empty & check_mask;
        SplatPawnMoves(moves, num_moves, -16, DoublePush, moved_pawns);
        moved_pawns = (pawns & ~promotion_rank & capture_left_mask) << 7 & enemy_pieces & check_mask;
        SplatPawnMoves(moves, num_moves, -7, 0, moved_pawns);
        moved_pawns = (pawns & ~promotion_rank & capture_right_mask) << 9 & enemy_pieces & check_mask;
        SplatPawnMoves(moves, num_moves, -9, 0, moved_pawns);
        const uint64_t promotion_pawns = pawns & promotion_rank;
        moved_pawns = promotion_pawns << 8 & empty & check_mask;
        SplatPawnMoves(moves, num_moves, -8, PromoteQueen, moved_pawns);
        SplatPawnMoves(moves, num_moves, -8, PromoteKnight, moved_pawns);
        SplatPawnMoves(moves, num_moves, -8, PromoteRook, moved_pawns);
        SplatPawnMoves(moves, num_moves, -8, PromoteBishop, moved_pawns);
        moved_pawns = (promotion_pawns & capture_left_mask) << 7 & enemy_pieces & check_mask;
        SplatPawnMoves(moves, num_moves, -7, PromoteQueen, moved_pawns);
        SplatPawnMoves(moves, num_moves, -7, PromoteKnight, moved_pawns);
        SplatPawnMoves(moves, num_moves, -7, PromoteRook, moved_pawns);
        SplatPawnMoves(moves, num_moves, -7, PromoteBishop, moved_pawns);
        moved_pawns = (promotion_pawns & capture_right_mask) << 9 & enemy_pieces & check_mask;
        SplatPawnMoves(moves, num_moves, -9, PromoteQueen, moved_pawns);
        SplatPawnMoves(moves, num_moves, -9, PromoteKnight, moved_pawns);
        SplatPawnMoves(moves, num_moves, -9, PromoteRook, moved_pawns);
        SplatPawnMoves(moves, num_moves, -9, PromoteBishop, moved_pawns);
    }
    // Pinned pawns
    pawns = board->color_bbs[!board->white_to_move] & board->piece_bbs[Pawn] & board->pins & ~king_rank;
    if (board->white_to_move)
    {
        moved_pawns = (pawns & ~promotion_rank) >> 8 & empty & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, 8, 0, king_sq, moved_pawns);
        moved_pawns = (pawns & double_push_rank) >> 16 & empty >> 8 & empty & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, 16, DoublePush, king_sq, moved_pawns);
        moved_pawns = (pawns & ~promotion_rank & capture_left_mask) >> 9 & enemy_pieces & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, 9, 0, king_sq, moved_pawns);
        moved_pawns = (pawns & ~promotion_rank & capture_right_mask) >> 7 & enemy_pieces & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, 7, 0, king_sq, moved_pawns);
        const uint64_t promotion_pawns = pawns & promotion_rank;
        moved_pawns = (promotion_pawns & capture_left_mask) >> 9 & enemy_pieces & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, 9, PromoteQueen, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, 9, PromoteKnight, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, 9, PromoteRook, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, 9, PromoteBishop, king_sq, moved_pawns);
        moved_pawns = (promotion_pawns & capture_right_mask) >> 7 & enemy_pieces & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, 7, PromoteQueen, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, 7, PromoteKnight, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, 7, PromoteRook, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, 7, PromoteBishop, king_sq, moved_pawns);
    }
    else
    {
        moved_pawns = (pawns & ~promotion_rank) << 8 & empty & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, -8, 0, king_sq, moved_pawns);
        moved_pawns = (pawns & double_push_rank) << 16 & empty << 8 & empty & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, -16, DoublePush, king_sq, moved_pawns);
        moved_pawns = (pawns & ~promotion_rank & capture_left_mask) << 7 & enemy_pieces & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, -7, 0, king_sq, moved_pawns);
        moved_pawns = (pawns & ~promotion_rank & capture_right_mask) << 9 & enemy_pieces & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, -9, 0, king_sq, moved_pawns);
        const uint64_t promotion_pawns = pawns & promotion_rank;
        moved_pawns = (promotion_pawns & capture_left_mask) << 7 & enemy_pieces & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, -7, PromoteQueen, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, -7, PromoteKnight, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, -7, PromoteRook, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, -7, PromoteBishop, king_sq, moved_pawns);
        moved_pawns = (promotion_pawns & capture_right_mask) << 9 & enemy_pieces & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, -9, PromoteQueen, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, -9, PromoteKnight, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, -9, PromoteRook, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, -9, PromoteBishop, king_sq, moved_pawns);
    }
    const int en_passant_file = board->en_passant_square % 8;
    const int en_passant_rank = board->en_passant_square / 8;
    if (can_en_passant){
        uint64_t pseudo_occupied;
        const int square = en_passant_rank * 8 + en_passant_file;
        const int captured_sq = square + (board->white_to_move ? 8 : -8);
        if (board->white_to_move) {
            pseudo_occupied = (occupied | 1ULL << square) & ~(1ULL << captured_sq | 1ULL << (square + 9));
            if (en_passant_file + 1 < 8 && board->squares[square + 9] == WhitePawn && !(AttackersToSquare(board, king_sq, pseudo_occupied) & pseudo_occupied & enemy_pieces)) {
                moves[(*num_moves)++] = MoveConstructor(square + 9, square, EnPassant);
            }
            pseudo_occupied = (occupied | 1ULL << square) & ~(1ULL << captured_sq | 1ULL << (square + 7));
            if (en_passant_file - 1 >= 0 && board->squares[square + 7] == WhitePawn && !(AttackersToSquare(board, king_sq, pseudo_occupied) & pseudo_occupied & enemy_pieces)) {
                moves[(*num_moves)++] = MoveConstructor(square + 7, square, EnPassant);
            }
        }else {
            pseudo_occupied = (occupied | 1ULL << square) & ~(1ULL << captured_sq | 1ULL << (square - 7));
            if (en_passant_file + 1 < 8 && board->squares[square - 7] == BlackPawn && !(AttackersToSquare(board, king_sq, pseudo_occupied) & pseudo_occupied & enemy_pieces)) {
                moves[(*num_moves)++] = MoveConstructor(square - 7, square, EnPassant);
            }
            pseudo_occupied = (occupied | 1ULL << square) & ~(1ULL << captured_sq | 1ULL << (square - 9));
            if (en_passant_file - 1 >= 0 && board->squares[square - 9] == BlackPawn && !(AttackersToSquare(board, king_sq, pseudo_occupied) & pseudo_occupied & enemy_pieces)) {
                moves[(*num_moves)++] = MoveConstructor(square - 9, square, EnPassant);
            }
        }
    }
}
/// Legal moves
int GetMoves(const Board *board, Move* moves){
    int num_moves = 0;
    uint64_t enemy_pieces = board->color_bbs[board->white_to_move];
    uint64_t friendly_pieces = board->color_bbs[!board->white_to_move];
    uint64_t occupied = enemy_pieces | friendly_pieces;
    uint64_t king = friendly_pieces & board->piece_bbs[King];
    uint64_t pieces = friendly_pieces & ~(board->piece_bbs[Knight] | board->piece_bbs[Pawn] | king);
    const int king_sq = getlsb(king);
    const uint64_t check_lines = GetCheckers(board, king_sq);
    const uint64_t filter = (check_lines ? check_lines : ~0ULL) & ~friendly_pieces;

    // When in double check only the king can move
    if (__builtin_popcountll(check_lines & enemy_pieces) >= 2)
    {
        GetKingMoves(board, check_lines, moves, &num_moves, poplsb(&king), friendly_pieces);
        return num_moves;
    }

    GetPawnMoves(board, check_lines, moves, &num_moves);
    GetKnightMoves(board, filter, moves, &num_moves, friendly_pieces & board->piece_bbs[Knight]);
    GetKingMoves(board, check_lines, moves, &num_moves, poplsb(&king), friendly_pieces);

    while(pieces){
        const int square = poplsb(&pieces);
        switch (GetType(board->squares[square])) {
            case Bishop:
                GetBishopMoves(board, filter, king_sq, moves, &num_moves, square, occupied);
                break;
            case Rook:
                GetRookMoves(board, filter, king_sq, moves, &num_moves, square, occupied);
                break;
            case Queen:
                GetRookMoves(board, filter, king_sq, moves, &num_moves, square, occupied);
                GetBishopMoves(board, filter, king_sq, moves, &num_moves, square, occupied);
                break;
            default:
                exit(-1);
        }
    }

    return num_moves;
}

void GetKingCaptures(const Board *board, Move *moves, int *num_moves, int square, uint64_t enemy_pieces){
    uint64_t bitboard = king_moves[square] & enemy_pieces & ~board->threat_bb;

    while(bitboard){
        const int target_square = poplsb(&bitboard);
        moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
    }
}

void GetPawnCaptures(const Board *board, const uint64_t check_lines, Move *moves, int *num_moves) {
    const bool can_en_passant = board->en_passant_square != -1;
    const int king_sq = getlsb(board->color_bbs[!board->white_to_move] & board->piece_bbs[King]);
    const uint64_t king_rank = first_rank >> (8 * GetRank(FlipRank(king_sq)));

    const uint64_t occupied = GetOccupied(board);
    const uint64_t enemy_pieces = board->color_bbs[board->white_to_move];
    const uint64_t check_mask = check_lines ? check_lines : ~0ULL;

    const uint64_t capture_right_mask = ~(a_file << 7);
    const uint64_t capture_left_mask = ~a_file;

    const uint64_t promotion_rank = board->white_to_move ? first_rank >> 48 : first_rank >> 8;

    uint64_t moved_pawns;
    // Non pinned pawns
    uint64_t pawns = board->color_bbs[!board->white_to_move] & board->piece_bbs[Pawn] & ~board->pins;
    if (board->white_to_move)
    {
        moved_pawns = (pawns & ~promotion_rank & capture_left_mask) >> 9 & enemy_pieces & check_mask;
        SplatPawnMoves(moves, num_moves, 9, 0, moved_pawns);

        moved_pawns = (pawns & ~promotion_rank & capture_right_mask) >> 7 & enemy_pieces & check_mask;
        SplatPawnMoves(moves, num_moves, 7, 0, moved_pawns);

        const uint64_t promotion_pawns = pawns & promotion_rank;
        moved_pawns = (promotion_pawns & capture_left_mask) >> 9 & enemy_pieces & check_mask;
        SplatPawnMoves(moves, num_moves, 9, PromoteQueen, moved_pawns);
        SplatPawnMoves(moves, num_moves, 9, PromoteKnight, moved_pawns);
        SplatPawnMoves(moves, num_moves, 9, PromoteRook, moved_pawns);
        SplatPawnMoves(moves, num_moves, 9, PromoteBishop, moved_pawns);

        moved_pawns = (promotion_pawns & capture_right_mask) >> 7 & enemy_pieces & check_mask;
        SplatPawnMoves(moves, num_moves, 7, PromoteQueen, moved_pawns);
        SplatPawnMoves(moves, num_moves, 7, PromoteKnight, moved_pawns);
        SplatPawnMoves(moves, num_moves, 7, PromoteRook, moved_pawns);
        SplatPawnMoves(moves, num_moves, 7, PromoteBishop, moved_pawns);
    }
    else
    {
        moved_pawns = (pawns & ~promotion_rank & capture_left_mask) << 7 & enemy_pieces & check_mask;
        SplatPawnMoves(moves, num_moves, -7, 0, moved_pawns);

        moved_pawns = (pawns & ~promotion_rank & capture_right_mask) << 9 & enemy_pieces & check_mask;
        SplatPawnMoves(moves, num_moves, -9, 0, moved_pawns);

        const uint64_t promotion_pawns = pawns & promotion_rank;
        moved_pawns = (promotion_pawns & capture_left_mask) << 7 & enemy_pieces & check_mask;
        SplatPawnMoves(moves, num_moves, -7, PromoteQueen, moved_pawns);
        SplatPawnMoves(moves, num_moves, -7, PromoteKnight, moved_pawns);
        SplatPawnMoves(moves, num_moves, -7, PromoteRook, moved_pawns);
        SplatPawnMoves(moves, num_moves, -7, PromoteBishop, moved_pawns);

        moved_pawns = (promotion_pawns & capture_right_mask) << 9 & enemy_pieces & check_mask;
        SplatPawnMoves(moves, num_moves, -9, PromoteQueen, moved_pawns);
        SplatPawnMoves(moves, num_moves, -9, PromoteKnight, moved_pawns);
        SplatPawnMoves(moves, num_moves, -9, PromoteRook, moved_pawns);
        SplatPawnMoves(moves, num_moves, -9, PromoteBishop, moved_pawns);
    }

    // Pinned pawns
    pawns = board->color_bbs[!board->white_to_move] & board->piece_bbs[Pawn] & board->pins & ~king_rank;
    if (board->white_to_move)
    {
        moved_pawns = (pawns & ~promotion_rank & capture_left_mask) >> 9 & enemy_pieces & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, 9, 0, king_sq, moved_pawns);

        moved_pawns = (pawns & ~promotion_rank & capture_right_mask) >> 7 & enemy_pieces & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, 7, 0, king_sq, moved_pawns);

        const uint64_t promotion_pawns = pawns & promotion_rank;
        moved_pawns = (promotion_pawns & capture_left_mask) >> 9 & enemy_pieces & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, 9, PromoteQueen, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, 9, PromoteKnight, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, 9, PromoteRook, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, 9, PromoteBishop, king_sq, moved_pawns);

        moved_pawns = (promotion_pawns & capture_right_mask) >> 7 & enemy_pieces & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, 7, PromoteQueen, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, 7, PromoteKnight, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, 7, PromoteRook, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, 7, PromoteBishop, king_sq, moved_pawns);
    }
    else
    {
        moved_pawns = (pawns & ~promotion_rank & capture_left_mask) << 7 & enemy_pieces & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, -7, 0, king_sq, moved_pawns);

        moved_pawns = (pawns & ~promotion_rank & capture_right_mask) << 9 & enemy_pieces & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, -9, 0, king_sq, moved_pawns);

        const uint64_t promotion_pawns = pawns & promotion_rank;
        moved_pawns = (promotion_pawns & capture_left_mask) << 7 & enemy_pieces & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, -7, PromoteQueen, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, -7, PromoteKnight, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, -7, PromoteRook, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, -7, PromoteBishop, king_sq, moved_pawns);

        moved_pawns = (promotion_pawns & capture_right_mask) << 9 & enemy_pieces & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, -9, PromoteQueen, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, -9, PromoteKnight, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, -9, PromoteRook, king_sq, moved_pawns);
        SplatPinnedPawnMoves(moves, num_moves, -9, PromoteBishop, king_sq, moved_pawns);
    }
    const int en_passant_file = board->en_passant_square % 8;
    const int en_passant_rank = board->en_passant_square / 8;
    if (can_en_passant){
        uint64_t pseudo_occupied;
        const int square = en_passant_rank * 8 + en_passant_file;
        const int captured_sq = square + (board->white_to_move ? 8 : -8);
        if (board->white_to_move) {
            pseudo_occupied = (occupied | 1ULL << square) & ~(1ULL << captured_sq | 1ULL << (square + 9));
            if (en_passant_file + 1 < 8 && board->squares[square + 9] == WhitePawn && !(AttackersToSquare(board, king_sq, pseudo_occupied) & pseudo_occupied & enemy_pieces)) {
                moves[(*num_moves)++] = MoveConstructor(square + 9, square, EnPassant);
            }
            pseudo_occupied = (occupied | 1ULL << square) & ~(1ULL << captured_sq | 1ULL << (square + 7));
            if (en_passant_file - 1 >= 0 && board->squares[square + 7] == WhitePawn && !(AttackersToSquare(board, king_sq, pseudo_occupied) & pseudo_occupied & enemy_pieces)) {
                moves[(*num_moves)++] = MoveConstructor(square + 7, square, EnPassant);
            }
        }else {
            pseudo_occupied = (occupied | 1ULL << square) & ~(1ULL << captured_sq | 1ULL << (square - 7));
            if (en_passant_file + 1 < 8 && board->squares[square - 7] == BlackPawn && !(AttackersToSquare(board, king_sq, pseudo_occupied) & pseudo_occupied & enemy_pieces)) {
                moves[(*num_moves)++] = MoveConstructor(square - 7, square, EnPassant);
            }
            pseudo_occupied = (occupied | 1ULL << square) & ~(1ULL << captured_sq | 1ULL << (square - 9));
            if (en_passant_file - 1 >= 0 && board->squares[square - 9] == BlackPawn && !(AttackersToSquare(board, king_sq, pseudo_occupied) & pseudo_occupied & enemy_pieces)) {
                moves[(*num_moves)++] = MoveConstructor(square - 9, square, EnPassant);
            }
        }
    }
}

int GetCaptures(const Board *board, Move* moves)
{
    int num_moves = 0;
    uint64_t enemy_pieces = board->color_bbs[board->white_to_move];
    uint64_t friendly_pieces = board->color_bbs[!board->white_to_move];
    uint64_t occupied = enemy_pieces | friendly_pieces;
    uint64_t king = friendly_pieces & board->piece_bbs[King];
    uint64_t pieces = friendly_pieces & ~(board->piece_bbs[Knight] | board->piece_bbs[Pawn] | king);
    const int king_sq = getlsb(king);
    const uint64_t check_lines = GetCheckers(board, king_sq);
    const uint64_t filter = (check_lines ? check_lines : ~0ULL) & enemy_pieces;

    // When in double check only the king can move
    if (__builtin_popcountll(check_lines & enemy_pieces) >= 2)
    {
        GetKingCaptures(board, moves, &num_moves, poplsb(&king), enemy_pieces);
        return num_moves;
    }

    GetPawnCaptures(board, check_lines, moves, &num_moves);
    GetKnightMoves(board, filter, moves, &num_moves, friendly_pieces & board->piece_bbs[Knight]);
    GetKingCaptures(board, moves, &num_moves, poplsb(&king), enemy_pieces);

    while(pieces){
        const int square = poplsb(&pieces);
        switch (GetType(board->squares[square])) {
        case Bishop:
            GetBishopMoves(board, filter, king_sq, moves, &num_moves, square, occupied);
            break;
        case Rook:
            GetRookMoves(board, filter, king_sq, moves, &num_moves, square, occupied);
            break;
        case Queen:
            GetRookMoves(board, filter, king_sq, moves, &num_moves, square, occupied);
            GetBishopMoves(board, filter, king_sq, moves, &num_moves, square, occupied);
            break;
        default:
            exit(-1);
        }
    }

    return num_moves;
}

void GetKingQuiets(const Board *board, const uint64_t check_lines, Move *moves, int *num_moves, int square, uint64_t occupied){
    uint64_t bitboard = king_moves[square] & ~occupied & ~board->threat_bb;

    while(bitboard){
        const int target_square = poplsb(&bitboard);
        moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
    }

    uint64_t castle_bb;
    if (check_lines)
    {
        return;
    }
    if (board->white_to_move){
        castle_bb = 1Ull << 61 | 1ULL << 62;
        if (board->white_kingside && !((occupied | board->threat_bb) & castle_bb)){
            moves[(*num_moves)++] = MoveConstructor(square, 62, Castle);
        }
        castle_bb = 1Ull << 57 | 1ULL << 58 | 1ULL << 59;
        if (board->white_queenside && !(occupied & castle_bb) && !(board->threat_bb & (1ULL << 58 | 1ULL << 59))){
            moves[(*num_moves)++] = MoveConstructor(square, 58, Castle);
        }
    }
    else{
        castle_bb = 1Ull << 5 | 1ULL << 6;
        if (board->black_kingside && !((occupied | board->threat_bb) & castle_bb)){
            moves[(*num_moves)++] = MoveConstructor(square, 6, Castle);
        }
        castle_bb = 1Ull << 1 | 1ULL << 2 | 1ULL << 3;
        if (board->black_queenside && !(occupied & castle_bb) && !(board->threat_bb & (1ULL << 2 | 1ULL << 3))){
            moves[(*num_moves)++] = MoveConstructor(square, 2, Castle);
        }
    }
}

void GetPawnQuiets(const Board *board, const uint64_t check_lines, Move *moves, int *num_moves) {
    const int king_sq = getlsb(board->color_bbs[!board->white_to_move] & board->piece_bbs[King]);
    const uint64_t king_rank = first_rank >> (8 * GetRank(FlipRank(king_sq)));

    const uint64_t occupied = GetOccupied(board);
    const uint64_t empty = ~occupied;
    const uint64_t check_mask = check_lines ? check_lines : ~0ULL;

    const uint64_t promotion_rank = board->white_to_move ? first_rank >> 48 : first_rank >> 8;
    const uint64_t double_push_rank = board->white_to_move ? first_rank >> 8 : first_rank >> 48;

    uint64_t moved_pawns;
    // Non pinned pawns
    uint64_t pawns = board->color_bbs[!board->white_to_move] & board->piece_bbs[Pawn] & ~board->pins;
    if (board->white_to_move)
    {
        moved_pawns = (pawns & ~promotion_rank) >> 8 & empty & check_mask;
        SplatPawnMoves(moves, num_moves, 8, 0, moved_pawns);

        moved_pawns = (pawns & double_push_rank) >> 16 & empty >> 8 & empty & check_mask;
        SplatPawnMoves(moves, num_moves, 16, DoublePush, moved_pawns);

        const uint64_t promotion_pawns = pawns & promotion_rank;
        moved_pawns = promotion_pawns >> 8 & empty & check_mask;
        SplatPawnMoves(moves, num_moves, 8, PromoteQueen, moved_pawns);
        SplatPawnMoves(moves, num_moves, 8, PromoteKnight, moved_pawns);
        SplatPawnMoves(moves, num_moves, 8, PromoteRook, moved_pawns);
        SplatPawnMoves(moves, num_moves, 8, PromoteBishop, moved_pawns);
    }
    else
    {
        moved_pawns = (pawns & ~promotion_rank) << 8 & empty & check_mask;
        SplatPawnMoves(moves, num_moves, -8, 0, moved_pawns);

        moved_pawns = (pawns & double_push_rank) << 16 & empty << 8 & empty & check_mask;
        SplatPawnMoves(moves, num_moves, -16, DoublePush, moved_pawns);

        const uint64_t promotion_pawns = pawns & promotion_rank;
        moved_pawns = promotion_pawns << 8 & empty & check_mask;
        SplatPawnMoves(moves, num_moves, -8, PromoteQueen, moved_pawns);
        SplatPawnMoves(moves, num_moves, -8, PromoteKnight, moved_pawns);
        SplatPawnMoves(moves, num_moves, -8, PromoteRook, moved_pawns);
        SplatPawnMoves(moves, num_moves, -8, PromoteBishop, moved_pawns);
    }

    // Pinned pawns
    pawns = board->color_bbs[!board->white_to_move] & board->piece_bbs[Pawn] & board->pins & ~king_rank;
    if (board->white_to_move)
    {
        moved_pawns = (pawns & ~promotion_rank) >> 8 & empty & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, 8, 0, king_sq, moved_pawns);

        moved_pawns = (pawns & double_push_rank) >> 16 & empty >> 8 & empty & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, 16, DoublePush, king_sq, moved_pawns);
    }
    else
    {
        moved_pawns = (pawns & ~promotion_rank) << 8 & empty & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, -8, 0, king_sq, moved_pawns);

        moved_pawns = (pawns & double_push_rank) << 16 & empty << 8 & empty & check_mask & board->pins;
        SplatPinnedPawnMoves(moves, num_moves, -16, DoublePush, king_sq, moved_pawns);
    }
}

/// Legal moves
int GetQuiets(const Board *board, Move* moves){
    int num_moves = 0;
    uint64_t enemy_pieces = board->color_bbs[board->white_to_move];
    uint64_t friendly_pieces = board->color_bbs[!board->white_to_move];
    uint64_t occupied = enemy_pieces | friendly_pieces;
    uint64_t king = friendly_pieces & board->piece_bbs[King];
    uint64_t pieces = friendly_pieces & ~(board->piece_bbs[Knight] | board->piece_bbs[Pawn] | king);
    const int king_sq = getlsb(king);
    const uint64_t check_lines = GetCheckers(board, king_sq);
    const uint64_t filter = (check_lines ? check_lines : ~0ULL) & ~occupied;

    // When in double check only the king can move
    if (__builtin_popcountll(check_lines & enemy_pieces) >= 2)
    {
        GetKingQuiets(board, check_lines, moves, &num_moves, poplsb(&king), occupied);
        return num_moves;
    }

    GetPawnQuiets(board, check_lines, moves, &num_moves);
    GetKnightMoves(board, filter, moves, &num_moves, friendly_pieces & board->piece_bbs[Knight]);
    GetKingQuiets(board, check_lines, moves, &num_moves, poplsb(&king), occupied);

    while(pieces){
        const int square = poplsb(&pieces);
        switch (GetType(board->squares[square])) {
            case Bishop:
                GetBishopMoves(board, filter, king_sq, moves, &num_moves, square, occupied);
                break;
            case Rook:
                GetRookMoves(board, filter, king_sq, moves, &num_moves, square, occupied);
                break;
            case Queen:
                GetRookMoves(board, filter, king_sq, moves, &num_moves, square, occupied);
                GetBishopMoves(board, filter, king_sq, moves, &num_moves, square, occupied);
                break;
            default:
                exit(-1);
        }
    }
    return num_moves;
}