#include <stdlib.h>
#include <stdio.h>
#include "preComputedData.h"
#include "moveGeneration.h"
#include "utility.h"
#include "board.h"
#include "move.h"

void GetRookMoves(Move *moves, int *num_moves, int square, uint64_t friendly_pieces, uint64_t occupied){
    uint64_t move_bb = rook_attack(occupied, square) & ~friendly_pieces;
    while(move_bb){
        const int target_square = poplsb(&move_bb);
        moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
    }
}

void GetBishopMoves(Move *moves, int *num_moves, int square, uint64_t friendly_pieces, uint64_t occupied){
    uint64_t move_bb = bishop_attack(occupied, square) & ~friendly_pieces;
    while(move_bb){
        const int target_square = poplsb(&move_bb);
        moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
    }
}

void GetKnightMoves(Move *moves, int *num_moves, uint64_t knights, uint64_t friendly_pieces){
    while (knights){
        const int square = poplsb(&knights);
        uint64_t bitboard = knight_moves[square] & ~friendly_pieces;

        while(bitboard){
            const int target_square = poplsb(&bitboard);
            moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
        }
    }
}

void GetKingMoves(Board *board, Move *moves, int *num_moves, int square, uint64_t friendly_pieces){
    unsigned long long bitboard = king_moves[square] & ~friendly_pieces;
    uint64_t occupied = GetOccupied(board);

    while(bitboard){
        const int target_square = poplsb(&bitboard);
        moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
    }

    if (board->white_to_move){
        if (board->white_kingside && !(occupied & ((1Ull << 61) | (1ULL << 62)))){
            moves[(*num_moves)++] = MoveConstructor(square, 62, Castle);
        }
        if (board->white_queenside && !(occupied & ((1Ull << 57) | (1ULL << 58) | (1ULL << 59)))){
            moves[(*num_moves)++] = MoveConstructor(square, 58, Castle);
        }
    }else{
        if (board->black_kingside && !(occupied & ((1Ull << 5) | (1ULL << 6)))){
            moves[(*num_moves)++] = MoveConstructor(square, 6, Castle);
        }

        if (board->black_queenside && !(occupied & ((1Ull << 1) | (1ULL << 2) | (1ULL << 3)))){
            moves[(*num_moves)++] = MoveConstructor(square, 2, Castle);
        }
    }
}

void GetPawnMoves(Board *board, Move *moves, int *num_moves) {
    const bool can_en_passant = board->en_passant_square != -1;

    const uint64_t pawns = board->color_bbs[!board->white_to_move] & board->piece_bbs[Pawn];
    const uint64_t occupied = GetOccupied(board);
    const uint64_t empty = ~occupied;
    const uint64_t enemy_pieces = board->color_bbs[board->white_to_move];

    const uint64_t capture_right_mask = ~(a_file << 7);
    const uint64_t capture_left_mask = ~a_file;

    const uint64_t promotion_rank = board->white_to_move ? first_rank >> 48 : first_rank >> 8;
    const uint64_t double_push_rank = board->white_to_move ? first_rank >> 8 : first_rank >> 48;

    uint64_t moved_pawns;
    if (board->white_to_move){
        moved_pawns = ((pawns & ~promotion_rank) >> 8) & empty;
        while(moved_pawns){
            const int target_square = poplsb(&moved_pawns);
            moves[(*num_moves)++] = MoveConstructor(target_square + 8, target_square, 0);
        }
        moved_pawns = ((pawns & double_push_rank) >> 16) & (empty >> 8) & empty;
        while(moved_pawns){
            const int target_square = poplsb(&moved_pawns);
            moves[(*num_moves)++] = MoveConstructor(target_square + 16, target_square, DoublePush);
        }
        moved_pawns = (((pawns & ~promotion_rank) & capture_left_mask) >> 9) & enemy_pieces;
        while(moved_pawns){
            const int target_square = poplsb(&moved_pawns);
            moves[(*num_moves)++] = MoveConstructor(target_square + 9, target_square, 0);
        }
        moved_pawns = (((pawns & ~promotion_rank) & capture_right_mask) >> 7) & enemy_pieces;
        while(moved_pawns){
            const int target_square = poplsb(&moved_pawns);
            moves[(*num_moves)++] = MoveConstructor(target_square + 7, target_square, 0);
        }

        const uint64_t promotion_pawns = pawns & promotion_rank;
        moved_pawns = (promotion_pawns >> 8) & empty;
        while(moved_pawns){
            const int target_square = poplsb(&moved_pawns);
            moves[(*num_moves)++] = MoveConstructor(target_square + 8, target_square, PromoteQueen);
            moves[(*num_moves)++] = MoveConstructor(target_square + 8, target_square, PromoteKnight);
            moves[(*num_moves)++] = MoveConstructor(target_square + 8, target_square, PromoteRook);
            moves[(*num_moves)++] = MoveConstructor(target_square + 8, target_square, PromoteBishop);
        }
        moved_pawns = ((promotion_pawns & capture_left_mask) >> 9) & enemy_pieces;
        while(moved_pawns){
            const int target_square = poplsb(&moved_pawns);
            moves[(*num_moves)++] = MoveConstructor(target_square + 9, target_square, PromoteQueen);
            moves[(*num_moves)++] = MoveConstructor(target_square + 9, target_square, PromoteKnight);
            moves[(*num_moves)++] = MoveConstructor(target_square + 9, target_square, PromoteRook);
            moves[(*num_moves)++] = MoveConstructor(target_square + 9, target_square, PromoteBishop);
        }
        moved_pawns = ((promotion_pawns & capture_right_mask) >> 7) & enemy_pieces;
        while(moved_pawns){
            const int target_square = poplsb(&moved_pawns);
            moves[(*num_moves)++] = MoveConstructor(target_square + 7, target_square, PromoteQueen);
            moves[(*num_moves)++] = MoveConstructor(target_square + 7, target_square, PromoteKnight);
            moves[(*num_moves)++] = MoveConstructor(target_square + 7, target_square, PromoteRook);
            moves[(*num_moves)++] = MoveConstructor(target_square + 7, target_square, PromoteBishop);
        }
    }else{
        moved_pawns = ((pawns & ~promotion_rank) << 8) & empty;
        while(moved_pawns){
            const int target_square = poplsb(&moved_pawns);
            moves[(*num_moves)++] = MoveConstructor(target_square - 8, target_square, 0);
        }
        moved_pawns = ((pawns & double_push_rank) << 16) & (empty << 8) & empty;
        while(moved_pawns){
            const int target_square = poplsb(&moved_pawns);
            moves[(*num_moves)++] = MoveConstructor(target_square - 16, target_square, DoublePush);
        }
        moved_pawns = (((pawns & ~promotion_rank) & capture_left_mask) << 7) & enemy_pieces;
        while(moved_pawns){
            const int target_square = poplsb(&moved_pawns);
            moves[(*num_moves)++] = MoveConstructor(target_square - 7, target_square, 0);
        }
        moved_pawns = (((pawns & ~promotion_rank) & capture_right_mask) << 9) & enemy_pieces;
        while(moved_pawns){
            const int target_square = poplsb(&moved_pawns);
            moves[(*num_moves)++] = MoveConstructor(target_square - 9, target_square, 0);
        }

        const uint64_t promotion_pawns = pawns & promotion_rank;
        moved_pawns = (promotion_pawns << 8) & empty;
        while(moved_pawns){
            const int target_square = poplsb(&moved_pawns);
            moves[(*num_moves)++] = MoveConstructor(target_square - 8, target_square, PromoteQueen);
            moves[(*num_moves)++] = MoveConstructor(target_square - 8, target_square, PromoteKnight);
            moves[(*num_moves)++] = MoveConstructor(target_square - 8, target_square, PromoteRook);
            moves[(*num_moves)++] = MoveConstructor(target_square - 8, target_square, PromoteBishop);
        }
        moved_pawns = ((promotion_pawns & capture_left_mask) << 7) & enemy_pieces;
        while(moved_pawns){
            const int target_square = poplsb(&moved_pawns);
            moves[(*num_moves)++] = MoveConstructor(target_square - 7, target_square, PromoteQueen);
            moves[(*num_moves)++] = MoveConstructor(target_square - 7, target_square, PromoteKnight);
            moves[(*num_moves)++] = MoveConstructor(target_square - 7, target_square, PromoteRook);
            moves[(*num_moves)++] = MoveConstructor(target_square - 7, target_square, PromoteBishop);
        }
        moved_pawns = ((promotion_pawns & capture_right_mask) << 9) & enemy_pieces;
        while(moved_pawns){
            const int target_square = poplsb(&moved_pawns);
            moves[(*num_moves)++] = MoveConstructor(target_square - 9, target_square, PromoteQueen);
            moves[(*num_moves)++] = MoveConstructor(target_square - 9, target_square, PromoteKnight);
            moves[(*num_moves)++] = MoveConstructor(target_square - 9, target_square, PromoteRook);
            moves[(*num_moves)++] = MoveConstructor(target_square - 9, target_square, PromoteBishop);
        }
    }

    const int en_passant_file = board->en_passant_square % 8;
    const int en_passant_rank = board->en_passant_square / 8;
    if (can_en_passant){
        const int square = en_passant_rank * 8 + en_passant_file;
        if (board->white_to_move) {
            if (en_passant_file + 1 < 8 && board->squares[square + 9] == WhitePawn) {
                moves[(*num_moves)++] = MoveConstructor(square + 9, square, EnPassant);
            }
            if (en_passant_file - 1 >= 0 && board->squares[square + 7] == WhitePawn) {
                moves[(*num_moves)++] = MoveConstructor(square + 7, square, EnPassant);
            }
        }else {

            if (en_passant_file + 1 < 8 && board->squares[square - 7] == BlackPawn) {
                moves[(*num_moves)++] = MoveConstructor(square - 7, square, EnPassant);
            }

            if (en_passant_file - 1 >= 0 && board->squares[square - 9] == BlackPawn) {
                moves[(*num_moves)++] = MoveConstructor(square - 9, square, EnPassant);
            }
        }
    }
}

/// Pseudolegal moves
void GetMoves(Board *board, Move* moves, int *num_moves){
    uint64_t enemy_pieces = board->color_bbs[board->white_to_move];
    uint64_t friendly_pieces = board->color_bbs[!board->white_to_move];
    uint64_t occupied = enemy_pieces | friendly_pieces;
    uint64_t king = friendly_pieces & board->piece_bbs[King];
    uint64_t pieces = friendly_pieces & ~(board->piece_bbs[Knight] | board->piece_bbs[Pawn] | king);


    GetPawnMoves(board, moves, num_moves);
    GetKnightMoves(moves, num_moves, friendly_pieces & board->piece_bbs[Knight], friendly_pieces);
    GetKingMoves(board, moves, num_moves, poplsb(&king), friendly_pieces);

    while(pieces){
        const int square = poplsb(&pieces);
        switch (GetType(board->squares[square])) {
            case Bishop:
                GetBishopMoves(moves, num_moves, square, friendly_pieces, occupied);
                break;
            case Rook:
                GetRookMoves(moves, num_moves, square, friendly_pieces, occupied);
                break;
            case Queen:
                GetRookMoves(moves, num_moves, square, friendly_pieces, occupied);
                GetBishopMoves(moves, num_moves, square, friendly_pieces, occupied);
                break;
            default:
                exit(-1);
        }
    }
}