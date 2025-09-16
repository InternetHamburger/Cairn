#include <stdlib.h>
#include <stdio.h>
#include "preComputedData.h"
#include "moveGeneration.h"
#include "utility.h"
#include "board.h"
#include "move.h"

void GetRookMoves(Board *board, Move *moves, int *num_moves, int square){

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
            if (board->squares[target_square]){
                if (IsOppositeColor(board->white_to_move, board->squares[target_square])){
                    moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
                }
                break;
            }
            moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);


        }
    }

}

void GetBishopMoves(Board *board, Move *moves, int *num_moves, int square){
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
            if (board->squares[target_square]){
                if (IsOppositeColor(board->white_to_move, board->squares[target_square])){
                    moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
                }
                break;
            }
            moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
        }
    }
}

void GetKnightMoves(Board *board, Move *moves, int *num_moves, int square){
    unsigned long long bitboard = knight_moves[square];

    while(bitboard){
        const int target_square = poplsb(&bitboard);
        if (board->squares[target_square]){
            if (IsOppositeColor(board->white_to_move, board->squares[target_square])){
                moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
            }
            continue;
        }

        moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
    }
}

void GetKingMoves(Board *board, Move *moves, int *num_moves, int square){
    unsigned long long bitboard = king_moves[square];

    while(bitboard){
        const int target_square = poplsb(&bitboard);
        if (board->squares[target_square]){
            if (IsOppositeColor(board->white_to_move, board->squares[target_square])){
                moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
            }
            continue;
        }

        moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
    }

    if (board->white_to_move){
        if (board->white_kingside && !board->squares[62] && !board->squares[61]){
            moves[(*num_moves)++] = MoveConstructor(square, 62, Castle);
        }
        if (board->white_queenside && !board->squares[57] && !board->squares[58] && !board->squares[59]){
            moves[(*num_moves)++] = MoveConstructor(square, 58, Castle);
        }
    }else{
        if (board->black_kingside && !board->squares[6] && !board->squares[5]){
            moves[(*num_moves)++] = MoveConstructor(square, 6, Castle);
        }
        if (board->black_queenside && !board->squares[1] && !board->squares[2] && !board->squares[3]){
            moves[(*num_moves)++] = MoveConstructor(square, 2, Castle);
        }
    }
}

void GetPawnMoves(Board *board, Move *moves, int *num_moves, int square) {
    const bool can_en_passant = board->en_passant_square != -1;

    const int rank = square / 8;
    const int file = square % 8;

    // Left, right
    const int capture_file_change[] = {-1, 1};
    if (board->white_to_move) {
        const int new_rank = rank - 1;
        const bool can_capture_left = (file + capture_file_change[0]) >= 0 && IsOppositeColor(board->white_to_move, board->squares[square - 9]) && board->squares[square - 9];
        const bool can_capture_right = (file + capture_file_change[1]) < 8 && IsOppositeColor(board->white_to_move, board->squares[square - 7]) && board->squares[square - 7];
        if (rank == 1) {
            if (!board->squares[square - 8]) {
                moves[(*num_moves)++] = MoveConstructor(square, square - 8, PromoteQueen);
                moves[(*num_moves)++] = MoveConstructor(square, square - 8, PromoteKnight);
                moves[(*num_moves)++] = MoveConstructor(square, square - 8, PromoteRook);
                moves[(*num_moves)++] = MoveConstructor(square, square - 8, PromoteBishop);
            }
            if (can_capture_left) {
                moves[(*num_moves)++] = MoveConstructor(square, square - 9, PromoteQueen);
                moves[(*num_moves)++] = MoveConstructor(square, square - 9, PromoteKnight);
                moves[(*num_moves)++] = MoveConstructor(square, square - 9, PromoteRook);
                moves[(*num_moves)++] = MoveConstructor(square, square - 9, PromoteBishop);
            }
            if (can_capture_right) {
                moves[(*num_moves)++] = MoveConstructor(square, square - 7, PromoteQueen);
                moves[(*num_moves)++] = MoveConstructor(square, square - 7, PromoteKnight);
                moves[(*num_moves)++] = MoveConstructor(square, square - 7, PromoteRook);
                moves[(*num_moves)++] = MoveConstructor(square, square - 7, PromoteBishop);
            }
        }else {
            if (new_rank >= 0 && !board->squares[square - 8]) {
                moves[(*num_moves)++] = MoveConstructor(square, square - 8, 0);
                if (rank == 6 && !board->squares[square - 16]) {
                    moves[(*num_moves)++] = MoveConstructor(square, square - 16, DoublePush);
                }
            }
            if (can_capture_left) {
                moves[(*num_moves)++] = MoveConstructor(square, square - 9, 0);
            }
            if (can_capture_right) {
                moves[(*num_moves)++] = MoveConstructor(square, square - 7, 0);
            }
            if (rank == 3 && can_en_passant) {
                const int en_passant_file = board->en_passant_square % 8;

                // Capture left
                if (file - 1 == en_passant_file ) {
                    moves[(*num_moves)++] = MoveConstructor(square, square - 9, EnPassant);
                }

                // Capture right
                if (file + 1 == en_passant_file ) {
                    moves[(*num_moves)++] = MoveConstructor(square, square - 7, EnPassant);
                }
            }
        }

    }else {
        const int new_rank = rank + 1;
        const bool can_capture_left = (file + capture_file_change[0]) >= 0 && IsOppositeColor(board->white_to_move, board->squares[square + 7]) && board->squares[square + 7];
        const bool can_capture_right = (file + capture_file_change[1]) < 8 && IsOppositeColor(board->white_to_move, board->squares[square + 9]) && board->squares[square + 9];
        if (rank == 6) {
            if (!board->squares[square + 8]) {
                moves[(*num_moves)++] = MoveConstructor(square, square + 8, PromoteQueen);
                moves[(*num_moves)++] = MoveConstructor(square, square + 8, PromoteKnight);
                moves[(*num_moves)++] = MoveConstructor(square, square + 8, PromoteRook);
                moves[(*num_moves)++] = MoveConstructor(square, square + 8, PromoteBishop);
            }
            if (can_capture_left) {
                moves[(*num_moves)++] = MoveConstructor(square, square + 7, PromoteQueen);
                moves[(*num_moves)++] = MoveConstructor(square, square + 7, PromoteKnight);
                moves[(*num_moves)++] = MoveConstructor(square, square + 7, PromoteRook);
                moves[(*num_moves)++] = MoveConstructor(square, square + 7, PromoteBishop);
            }
            if (can_capture_right) {
                moves[(*num_moves)++] = MoveConstructor(square, square + 9, PromoteQueen);
                moves[(*num_moves)++] = MoveConstructor(square, square + 9, PromoteKnight);
                moves[(*num_moves)++] = MoveConstructor(square, square + 9, PromoteRook);
                moves[(*num_moves)++] = MoveConstructor(square, square + 9, PromoteBishop);
            }
        }else {
            if (new_rank < 8 && !board->squares[square + 8]) {
                moves[(*num_moves)++] = MoveConstructor(square, square + 8, 0);
                if (rank == 1 && !board->squares[square + 16]) {
                    moves[(*num_moves)++] = MoveConstructor(square, square + 16, DoublePush);
                }
            }
            if (can_capture_left) {
                moves[(*num_moves)++] = MoveConstructor(square, square + 7, 0);
            }
            if (can_capture_right) {
                moves[(*num_moves)++] = MoveConstructor(square, square + 9, 0);
            }
            if (rank == 4 && can_en_passant) {
                const int en_passant_file = board->en_passant_square % 8;

                // Capture left
                if (file - 1 == en_passant_file ) {
                    moves[(*num_moves)++] = MoveConstructor(square, square + 7, EnPassant);
                }

                // Capture right
                if (file + 1 == en_passant_file ) {
                    moves[(*num_moves)++] = MoveConstructor(square, square + 9, EnPassant);
                }
            }
        }
    }

}

/// Pseudolegal moves
Move* GetMoves(Board *board, int *num_moves){

    // Max number of moves in a position is 218
    Move* moves = malloc(sizeof(Move) * 256);

    for (int square = 0; square < 64; square++){
        if (board->squares[square] && IsColor(board->white_to_move, board->squares[square])){
            switch (GetType(board->squares[square])) {
                case Pawn:
                    GetPawnMoves(board, moves, num_moves, square);
                    break;
                case Knight:
                    GetKnightMoves(board, moves, num_moves, square);
                    break;
                case Bishop:
                    GetBishopMoves(board, moves, num_moves, square);
                    break;
                case Rook:
                    GetRookMoves(board, moves, num_moves, square);
                    break;
                case Queen:
                    GetRookMoves(board, moves, num_moves, square);
                    GetBishopMoves(board, moves, num_moves, square);
                    break;
                case King:
                    GetKingMoves(board, moves, num_moves, square);
                    break;
                default:
                    exit(-1);
            }
        }
    }


    return moves;
}