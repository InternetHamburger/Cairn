#include <stdlib.h>
#include <stdio.h>
#include "moveGeneration.h"
#include "boardUtility.h"
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

/// Pseudolegal moves
Move* GetMoves(Board *board, int *num_moves){
    Move* moves = (Move*)malloc(sizeof(Move) * 256);

    for (int square = 0; square < 64; square++){
        if (board->squares[square] && IsColor(board->white_to_move, board->squares[square])){
            switch (GetType(board->squares[square])) {
                case Pawn:
                    break;
                case Knight:
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
                    break;
                default:
                    exit(-1);
            }

        }
    }


    return moves;
}