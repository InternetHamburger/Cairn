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
            break;
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
            break;
        }

        moves[(*num_moves)++] = MoveConstructor(square, target_square, 0);
    }

    if (board->white_to_move){
        if (board->white_kingside && !board->squares[62] && !board->squares[61]){
            moves[(*num_moves)++] = MoveConstructor(square, 62, Castle);
        }
        if (board->white_queenside && !board->squares[57] && !board->squares[58] && !board->squares[59]){
            moves[(*num_moves)++] = MoveConstructor(square, 57, Castle);
        }
    }else{
        if (board->black_kingside && !board->squares[6] && !board->squares[5]){
            moves[(*num_moves)++] = MoveConstructor(square, 6, Castle);
        }
        if (board->black_queenside && !board->squares[1] && !board->squares[2] && !board->squares[3]){
            moves[(*num_moves)++] = MoveConstructor(square, 1, Castle);
        }
    }
}

/// Pseudolegal moves
Move* GetMoves(Board *board, int *num_moves){
    Move* moves = (Move*)malloc(sizeof(Move) * 256);

    for (int square = 0; square < 64; square++){
        if (board->squares[square] && IsColor(board->white_to_move, board->squares[square])){
            switch (GetType(board->squares[square])) {
                case Pawn:
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