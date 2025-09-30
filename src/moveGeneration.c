#include <stdlib.h>
#include <stdio.h>
#include "preComputedData.h"
#include "moveGeneration.h"
#include "utility.h"
#include "board.h"
#include "move.h"

void GetRookMoves(Move *moves, int *num_moves, int square, uint64_t enemy_pieces, uint64_t occupied){

    // From whites perspective
    // Right, left, up, down
    const int rank_directions[] = {0, 0, -1, 1};
    const int file_directions[] = {1, -1, 0, 0};

    for (int direction = 0; direction < 4; direction++){
        int curr_rank = square / 8;
        int curr_file = square % 8;
        while (1){
            curr_rank += rank_directions[direction];
            curr_file += file_directions[direction];

            const int target_square = 8 * curr_rank + curr_file;

            if (!(curr_rank >= 0 && curr_rank < 8 && curr_file >= 0 && curr_file < 8)){
                break;
            }
            if (occupied & 1ULL << target_square){
                if (enemy_pieces & 1ULL << target_square){
                    moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
                }
                break;
            }
            moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);


        }
    }

}

void GetBishopMoves(Move *moves, int *num_moves, int square, uint64_t enemy_pieces, uint64_t occupied){
    // From whites perspective
    // right-up, left-up, right-down, left-down
    const int rank_directions[] = {-1, -1, 1, 1};
    const int file_directions[] = {-1, 1, -1, 1};

    for (int direction = 0; direction < 4; direction++){
        int curr_rank = square / 8;
        int curr_file = square % 8;
        while (1){
            curr_rank += rank_directions[direction];
            curr_file += file_directions[direction];

            int target_square = 8 * curr_rank + curr_file;

            if (!(curr_rank >= 0 && curr_rank < 8 && curr_file >= 0 && curr_file < 8)){
                break;
            }
            if (occupied & 1ULL << target_square){
                if (enemy_pieces & 1ULL << target_square){
                    moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
                }
                break;
            }
            moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
        }
    }
}

void GetKnightMoves(Move *moves, int *num_moves, uint64_t knights, uint64_t friendly_pieces){
    while (knights){
        const int square = poplsb(&knights);
        unsigned long long bitboard = knight_moves[square] & ~friendly_pieces;

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

    const uint64_t pawns = board->bitboards[board->white_to_move ? WhitePawn : BlackPawn];
    const uint64_t occupied = GetOccupied(board);
    const uint64_t empty = ~occupied;
    const uint64_t enemy_pieces = board->white_to_move ? GetBlackBitboard(board) : GetWhiteBitboard(board);

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

    uint64_t enemy_pieces = board->white_to_move ? GetBlackBitboard(board) : GetWhiteBitboard(board);
    uint64_t friendly_pieces = board->white_to_move ? GetWhiteBitboard(board) : GetBlackBitboard(board);
    uint64_t occupied = GetOccupied(board);

    // Max number of moves in a position is 218
    GetPawnMoves(board, moves, num_moves);
    GetKnightMoves(moves, num_moves, board->bitboards[board->white_to_move ? WhiteKnight : BlackKnight], friendly_pieces);
    for (int square = 0; square < 64; square++){
        if (friendly_pieces & 1ULL << square){
            switch (GetType(board->squares[square])) {
                case Pawn:
                    break;
                case Knight:
                    break;
                case Bishop:
                    GetBishopMoves(moves, num_moves, square, enemy_pieces, occupied);
                    break;
                case Rook:
                    GetRookMoves(moves, num_moves, square, enemy_pieces, occupied);
                    break;
                case Queen:
                    GetRookMoves(moves, num_moves, square, enemy_pieces, occupied);
                    GetBishopMoves(moves, num_moves, square, enemy_pieces, occupied);
                    break;
                case King:
                    GetKingMoves(board, moves, num_moves, square, friendly_pieces);
                    break;
                default:
                    exit(-1);
            }
        }
    }
}