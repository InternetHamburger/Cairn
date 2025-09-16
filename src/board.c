#include "board.h"

#include <stdlib.h>
#include <stdio.h>
#include "move.h"
#include "utility.h"
#include "preComputedData.h"

void MakeMove(Board *board, const Move move) {
    const int start_square = StartSquare(move);
    const int target_square = TargetSquare(move);

    const int moved_piece = board->squares[start_square];
    const int captured_piece = board->squares[target_square]; // None in case of en passant

    board->en_passant_square = -1;

    board->squares[target_square] = board->squares[start_square];
    board->squares[start_square] = 0;

    const int flag = GetFlag(move);

    if (target_square == 63 || start_square == 63) {
        board->white_kingside = false;
    }
    if (target_square == 56 || start_square == 56) {
        board->white_queenside = false;
    }
    if (target_square == 7 || start_square == 7) {
        board->black_kingside = false;
    }
    if (target_square == 0 || start_square == 0) {
        board->black_queenside = false;
    }

    if (moved_piece == WhiteKing) {
        board->white_kingside = false;
        board->white_queenside = false;
    }
    if (moved_piece == BlackKing) {
        board->black_kingside = false;
        board->black_queenside = false;
    }

    if (IsPromotion(move)) {
        switch (flag) {
            case PromoteQueen:
                board->squares[target_square] = board->white_to_move ? WhiteQueen : BlackQueen;
            case PromoteKnight:
                board->squares[target_square] = board->white_to_move ? WhiteKnight : BlackKnight;
            case PromoteBishop:
                board->squares[target_square] = board->white_to_move ? WhiteBishop : BlackBishop;
            case PromoteRook:
                board->squares[target_square] = board->white_to_move ? WhiteRook : BlackRook;
            default:
                exit(-1);
        }
    }

    if (flag == DoublePush) {
        board->en_passant_square = start_square + (board->white_to_move ? 8 : -8);
    }

    if (flag == EnPassant) {
        const bool captures_left = (target_square % 8) < (start_square % 8);
        if (captures_left) {
            const int new_square = start_square - 1;
            board->squares[new_square] = 0;
        }
        else {
            const int new_square = start_square + 1;
            board->squares[new_square] = 0;
        }
    }

    if (flag == Castle) {
        if (target_square == 62) {
            board->squares[63] = 0;
            board->squares[61] = WhiteRook;
            board->white_kingside = false;
        }
        if (target_square == 57) {
            board->squares[56] = 0;
            board->squares[58] = WhiteRook;
            board->white_queenside = false;
        }
        if (target_square == 6) {
            board->squares[7] = 0;
            board->squares[5] = BlackRook;
            board->black_kingside = false;
        }
        if (target_square == 1) {
            board->squares[0] = 0;
            board->squares[2] = BlackRook;
            board->black_queenside = false;
        }
    }
    board->white_to_move = !board->white_to_move;
}

void PrintBoard(const Board* board) {
    char pieces[64];
    for (int i = 0; i < 64; i++) {
        pieces[i] = PieceToChar(board->squares[i]);
    }

    printf(" +---+---+---+---+---+---+---+---+\n");
    printf(" | %c | %c | %c | %c | %c | %c | %c | %c | 8\n", pieces[0], pieces[1], pieces[2], pieces[3], pieces[4], pieces[5], pieces[6], pieces[7]);
    printf(" +---+---+---+---+---+---+---+---+\n");
    printf(" | %c | %c | %c | %c | %c | %c | %c | %c | 8\n", pieces[8], pieces[9], pieces[10], pieces[11], pieces[12], pieces[13], pieces[14], pieces[15]);
    printf(" +---+---+---+---+---+---+---+---+\n");
    printf(" | %c | %c | %c | %c | %c | %c | %c | %c | 8\n", pieces[16], pieces[17], pieces[18], pieces[19], pieces[20], pieces[21], pieces[22], pieces[23]);
    printf(" +---+---+---+---+---+---+---+---+\n");
    printf(" | %c | %c | %c | %c | %c | %c | %c | %c | 8\n", pieces[24], pieces[25], pieces[26], pieces[27], pieces[28], pieces[29], pieces[30], pieces[31]);
    printf(" +---+---+---+---+---+---+---+---+\n");
    printf(" | %c | %c | %c | %c | %c | %c | %c | %c | 8\n", pieces[32], pieces[33], pieces[34], pieces[35], pieces[36], pieces[37], pieces[38], pieces[39]);
    printf(" +---+---+---+---+---+---+---+---+\n");
    printf(" | %c | %c | %c | %c | %c | %c | %c | %c | 8\n", pieces[40], pieces[41], pieces[42], pieces[43], pieces[44], pieces[45], pieces[46], pieces[47]);
    printf(" +---+---+---+---+---+---+---+---+\n");
    printf(" | %c | %c | %c | %c | %c | %c | %c | %c | 8\n", pieces[48], pieces[49], pieces[50], pieces[51], pieces[52], pieces[53], pieces[54], pieces[55]);
    printf(" +---+---+---+---+---+---+---+---+\n");
    printf(" | %c | %c | %c | %c | %c | %c | %c | %c | 8\n", pieces[56], pieces[57], pieces[58], pieces[59], pieces[60], pieces[61], pieces[62], pieces[63]);
    printf(" +---+---+---+---+---+---+---+---+\n");
}

bool IsAttackedBySideToMove(const Board *board, const int square) {

    const int rank = square / 8;
    const int file = square % 8;

    const int left_file = file - 1;
    const int right_file = file + 1;
    if (right_file < 8 && left_file >= 0) {
        if (board->white_to_move) {
            const int new_rank = rank - 1;

            if (board->squares[new_rank * 8 + left_file] == BlackPawn || board->squares[new_rank * 8 + left_file] == BlackPawn)
                return true;
        }else {
            const int new_rank = rank + 1;

            if (board->squares[new_rank * 8 + left_file] == WhitePawn || board->squares[new_rank * 8 + left_file] == WhitePawn)
                return true;
        }
    }

    unsigned long long knight_attacks = knight_moves[square];
    unsigned long long king_attacks = king_moves[square];

    if (board->white_to_move) {
        while(knight_attacks){
            const int target_square = poplsb(&knight_attacks);
            if (board->squares[target_square] == BlackKnight){
                return true;
            }
        }
        while(king_attacks){
            const int target_square = poplsb(&king_attacks);
            if (board->squares[target_square] == BlackKing){
                return true;
            }
        }
    }else {
        while(knight_attacks){
            const int target_square = poplsb(&knight_attacks);
            if (board->squares[target_square] == WhiteKnight){
                return true;
            }
        }
        while(king_attacks){
            const int target_square = poplsb(&king_attacks);
            if (board->squares[target_square] == WhiteKing){
                return true;
            }
        }
    }

    // right-up, left-up, right-down, left-down, right, left, up, down
    const int rank_directions[] = {-1, -1, 1, 1, 0, 0, -1, 1};
    const int file_directions[] = {-1, 1, -1, 1, 1, -1, 0, 0};

    for (int direction = 0; direction < 8; direction++){
        int curr_rank = square / 8;
        int curr_file = square % 8;
        while (1){
            curr_rank += rank_directions[direction];
            curr_file += file_directions[direction];

            int target_square = 8 * curr_rank + curr_file;
            PieceType target_type = GetType( board->squares[target_square]);
            if (!(curr_rank >= 0 && curr_rank < 8 && curr_file >= 0 && curr_file < 8)){
                break; // Outside board
            }
            if (board->squares[target_square]){
                if (IsColor(board->white_to_move, board->squares[target_square])){
                    if (target_type == Queen) return true;
                    if (direction < 4 && target_type == Bishop) return true;
                    if (direction > 3 && target_type == Rook) return true;

                }
                break;
            }
        }
    }




    return false;
}