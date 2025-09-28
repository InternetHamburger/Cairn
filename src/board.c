#include "board.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "move.h"
#include "zobrist.h"
#include "utility.h"
#include "preComputedData.h"

void MakeMove(Board *board, const Move move) {
    const int start_square = StartSquare(move);
    const int target_square = TargetSquare(move);

    const int moved_piece = board->squares[start_square];
    const int captured_piece = board->squares[target_square];

    if (board->en_passant_square != -1){
        board->zobrist_hash ^= zobrist_ep_squares[board->en_passant_square];
    }

    const bool init_white_kingside = board->white_kingside;
    const bool init_white_queenside = board->white_queenside;
    const bool init_black_kingside = board->black_kingside;
    const bool init_black_queenside = board->black_queenside;

    board->en_passant_square = -1;

    board->squares[target_square] = board->squares[start_square];
    board->squares[start_square] = 0;

    board->bitboards[moved_piece] ^= (1ULL << start_square) | (1ULL << target_square);

    const int flag = GetFlag(move);

    board->zobrist_hash ^= zobrist_squares[start_square][moved_piece];
    board->zobrist_hash ^= zobrist_squares[target_square][moved_piece];

    if (captured_piece != None){
        board->bitboards[captured_piece] ^= (1ULL << target_square);
        board->zobrist_hash ^= zobrist_squares[target_square][captured_piece];
    }

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
        board->white_king_square = target_square;
    }
    if (moved_piece == BlackKing) {
        board->black_kingside = false;
        board->black_queenside = false;
        board->black_king_square = target_square;
    }

    if (IsPromotion(move)) {
        board->zobrist_hash ^= zobrist_squares[target_square][moved_piece];
        board->bitboards[moved_piece] ^= (1ULL << target_square);
        switch (flag) {
            case PromoteQueen:
                board->squares[target_square] = board->white_to_move ? WhiteQueen : BlackQueen;
                board->bitboards[board->white_to_move ? WhiteQueen : BlackQueen] ^= (1ULL << target_square);
                board->zobrist_hash ^= zobrist_squares[target_square][board->white_to_move ? WhiteQueen : BlackQueen];
                break;
            case PromoteKnight:
                board->squares[target_square] = board->white_to_move ? WhiteKnight : BlackKnight;
                board->bitboards[board->white_to_move ? WhiteKnight : BlackKnight] ^= (1ULL << target_square);
                board->zobrist_hash ^= zobrist_squares[target_square][board->white_to_move ? WhiteKnight : BlackKnight];
                break;
            case PromoteBishop:
                board->squares[target_square] = board->white_to_move ? WhiteBishop : BlackBishop;
                board->bitboards[board->white_to_move ? WhiteBishop : BlackBishop] ^= (1ULL << target_square);
                board->zobrist_hash ^= zobrist_squares[target_square][board->white_to_move ? WhiteBishop : BlackBishop];
                break;
            case PromoteRook:
                board->squares[target_square] = board->white_to_move ? WhiteRook : BlackRook;
                board->bitboards[board->white_to_move ? WhiteRook : BlackRook] ^= (1ULL << target_square);
                board->zobrist_hash ^= zobrist_squares[target_square][board->white_to_move ? WhiteRook : BlackRook];
                break;
            default:
                exit(-1);
        }
    }

    if (flag == DoublePush) {

        board->en_passant_square = target_square + (board->white_to_move ? 8 : -8);
        board->zobrist_hash ^= zobrist_ep_squares[board->en_passant_square];
    }

    if (flag == EnPassant) {
        const bool captures_left = (target_square % 8) < (start_square % 8);
        if (captures_left) {
            const int new_square = start_square - 1;
            board->squares[new_square] = 0;
            board->bitboards[board->white_to_move ? BlackPawn : WhitePawn] ^= (1ULL << new_square);
            board->zobrist_hash ^= zobrist_squares[new_square][board->white_to_move ? BlackPawn : WhitePawn];
        }
        else {
            const int new_square = start_square + 1;
            board->squares[new_square] = 0;
            board->bitboards[board->white_to_move ? BlackPawn : WhitePawn] ^= (1ULL << new_square);
            board->zobrist_hash ^= zobrist_squares[new_square][board->white_to_move ? BlackPawn : WhitePawn];
        }
    }

    if (flag == Castle) {
        if (target_square == 62) {
            board->squares[63] = 0;
            board->squares[61] = WhiteRook;
            board->zobrist_hash ^= zobrist_squares[61][WhiteRook];
            board->zobrist_hash ^= zobrist_squares[63][WhiteRook];
            board->bitboards[WhiteRook] ^= (1ULL << 61) | (1ULL << 63);
        }
        if (target_square == 58) {
            board->squares[56] = 0;
            board->squares[59] = WhiteRook;
            board->zobrist_hash ^= zobrist_squares[56][WhiteRook];
            board->zobrist_hash ^= zobrist_squares[59][WhiteRook];
            board->bitboards[WhiteRook] ^= (1ULL << 56) | (1ULL << 59);

        }
        if (target_square == 6) {
            board->squares[7] = 0;
            board->squares[5] = BlackRook;
            board->zobrist_hash ^= zobrist_squares[7][BlackRook];
            board->zobrist_hash ^= zobrist_squares[5][BlackRook];
            board->bitboards[BlackRook] ^= (1ULL << 7) | (1ULL << 5);
        }
        if (target_square == 2) {
            board->squares[0] = 0;
            board->squares[3] = BlackRook;
            board->zobrist_hash ^= zobrist_squares[0][BlackRook];
            board->zobrist_hash ^= zobrist_squares[3][BlackRook];
            board->bitboards[BlackRook] ^= (1ULL << 0) | (1ULL << 3);
        }
    }
    if (board->white_kingside != init_white_kingside) board->zobrist_hash ^= zobrist_white_kingside;
    if (board->white_queenside != init_white_queenside) board->zobrist_hash ^= zobrist_white_queenside;
    if (board->black_kingside != init_black_kingside) board->zobrist_hash ^= zobrist_black_kingside;
    if (board->black_queenside != init_black_queenside) board->zobrist_hash ^= zobrist_black_queenside;


    board->white_to_move = !board->white_to_move;
    board->zobrist_hash ^= zobrist_stm;
}

void PrintBoard(const Board* board) {
    char pieces[64];
    for (int i = 0; i < 64; i++) {
        pieces[i] = PieceToChar(board->squares[i]);
    }

    printf(" +---+---+---+---+---+---+---+---+\n");
    printf(" | %c | %c | %c | %c | %c | %c | %c | %c | 8\n", pieces[0], pieces[1], pieces[2], pieces[3], pieces[4], pieces[5], pieces[6], pieces[7]);
    printf(" +---+---+---+---+---+---+---+---+\n");
    printf(" | %c | %c | %c | %c | %c | %c | %c | %c | 7\n", pieces[8], pieces[9], pieces[10], pieces[11], pieces[12], pieces[13], pieces[14], pieces[15]);
    printf(" +---+---+---+---+---+---+---+---+\n");
    printf(" | %c | %c | %c | %c | %c | %c | %c | %c | 6\n", pieces[16], pieces[17], pieces[18], pieces[19], pieces[20], pieces[21], pieces[22], pieces[23]);
    printf(" +---+---+---+---+---+---+---+---+\n");
    printf(" | %c | %c | %c | %c | %c | %c | %c | %c | 5\n", pieces[24], pieces[25], pieces[26], pieces[27], pieces[28], pieces[29], pieces[30], pieces[31]);
    printf(" +---+---+---+---+---+---+---+---+\n");
    printf(" | %c | %c | %c | %c | %c | %c | %c | %c | 4\n", pieces[32], pieces[33], pieces[34], pieces[35], pieces[36], pieces[37], pieces[38], pieces[39]);
    printf(" +---+---+---+---+---+---+---+---+\n");
    printf(" | %c | %c | %c | %c | %c | %c | %c | %c | 3\n", pieces[40], pieces[41], pieces[42], pieces[43], pieces[44], pieces[45], pieces[46], pieces[47]);
    printf(" +---+---+---+---+---+---+---+---+\n");
    printf(" | %c | %c | %c | %c | %c | %c | %c | %c | 2\n", pieces[48], pieces[49], pieces[50], pieces[51], pieces[52], pieces[53], pieces[54], pieces[55]);
    printf(" +---+---+---+---+---+---+---+---+\n");
    printf(" | %c | %c | %c | %c | %c | %c | %c | %c | 1\n", pieces[56], pieces[57], pieces[58], pieces[59], pieces[60], pieces[61], pieces[62], pieces[63]);
    printf(" +---+---+---+---+---+---+---+---+\n");
    printf("   a   b   c   d   e   f   g   h\n\n");
}

bool InCheck(const Board *board){
    return IsAttackedBySideToMove(board, !board->white_to_move, board->white_to_move ? board->white_king_square : board->black_king_square);
}

bool IsAttackedBySideToMove(const Board *board, bool white_to_move, const int square) {
    const int rank = square / 8;
    const int file = square % 8;

    const int left_file = file - 1;
    const int right_file = file + 1;
    if (right_file < 8){
        if (white_to_move) {
            const int new_rank = rank + 1;

            if (new_rank < 8 && board->squares[new_rank * 8 + right_file] == WhitePawn)
                return true;

        }else {
            const int new_rank = rank - 1;
            if (new_rank >= 0 && board->squares[new_rank * 8 + right_file] == BlackPawn)
                return true;
        }
    }
    if (left_file >= 0){
        if (white_to_move) {
            const int new_rank = rank + 1;

            if (new_rank < 8 && board->squares[new_rank * 8 + left_file] == WhitePawn)
                return true;

        }else {
            const int new_rank = rank - 1;
            if (new_rank >= 0 && board->squares[new_rank * 8 + left_file] == BlackPawn)
                return true;
        }
    }

    unsigned long long knight_attacks = knight_moves[square];
    unsigned long long king_attacks = king_moves[square];

    if (white_to_move) {
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
    }else {
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
                if (IsColor(white_to_move, board->squares[target_square])){
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

// Start fen: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
Board BoardConstructor(const char* fen){
    const int fen_length = strlen(fen);
    int main_length;
    int empty_encountered = 0;

    Piece squares[64];
    for (int i = 0; i < 64; i++) {
        squares[i] = None;
    }

    uint64_t bitboards[BlackKing + 1];
    for (int i = 0; i < BlackKing + 1; i++) {
        bitboards[i] = 0;
    }

    bool white_to_move;
    bool white_kingside = false;
    bool white_queenside = false;
    bool black_kingside = false;
    bool black_queenside = false;
    int en_passant_square = - 1;
    int white_king_square;
    int black_king_square;
    int i = 0;

    while (i < fen_length && empty_encountered < 3){
        if (fen[i] == ' '){
            switch (empty_encountered) {
                case 0:
                    main_length = i + 1;
                    white_to_move = fen[i + 1] == 'w';
                    break;
                case 1:
                    if (fen[i + 1] != '-') {
                        for (int castle_index = 1; castle_index <= 4; castle_index++){
                            if (fen[i + castle_index] == ' ') break;
                            switch (fen[i + castle_index]) {
                                case 'K':
                                    white_kingside = true;
                                    break;
                                case 'Q':
                                    white_queenside = true;
                                    break;
                                case 'k':
                                    black_kingside = true;
                                    break;
                                case 'q':
                                    black_queenside = true;
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                    break;
                case 2:
                    if (fen[i + 1] != '-') {
                        char* char_square = malloc(sizeof(char) * 2);
                        char_square[0] = fen[i + 1];
                        char_square[1] = fen[i + 2];
                        en_passant_square = StringToSquare(char_square);
                    }
                    break;
            }
            empty_encountered++;
        }
        i++;
    }
    int square_index = 0;
    int fen_index = 0;

    while (fen_index < main_length){
        char c = fen[fen_index];
        if (isdigit((unsigned char)c))
        {
            int num = c - '0';
            square_index += num;
            fen_index++;
            continue;
        }

        Piece piece = CharToPiece(c);
        if (piece == None)
        {
            fen_index++;
        }
        else
        {
            bitboards[piece] |= 1ULL << square_index;
            if (piece == WhiteKing) white_king_square = square_index;
            if (piece == BlackKing) black_king_square = square_index;
            squares[square_index] = piece;
            square_index++;
            fen_index++;
        }
    }

    Board board = {
            .white_to_move = white_to_move,
            .white_kingside = white_kingside,
            .white_queenside = white_queenside,
            .black_kingside = black_kingside,
            .black_queenside = black_queenside,
            .en_passant_square = en_passant_square,
            .black_king_square = black_king_square,
            .white_king_square = white_king_square,
    };

    board.zobrist_hash = 958761493876598375ULL; // Initial hash
    for (i = 0; i < 64; i++) {
        board.squares[i] = squares[i];

        if (board.squares[i] != None)
            board.zobrist_hash ^= zobrist_squares[i][squares[i]];
    }
    for (i = 0; i < BlackKing + 1; i++) {
        board.bitboards[i] = bitboards[i];
    }

    if (en_passant_square != -1) board.zobrist_hash ^= zobrist_ep_squares[en_passant_square];

    if (white_to_move) board.zobrist_hash ^= zobrist_stm;
    if (white_kingside) board.zobrist_hash ^= zobrist_white_kingside;
    if (white_queenside) board.zobrist_hash ^= zobrist_white_queenside;
    if (black_kingside) board.zobrist_hash ^= zobrist_black_kingside;
    if (black_queenside) board.zobrist_hash ^= zobrist_black_queenside;


    return board;
}

bool IsRepetition(const unsigned long long hashes[MAX_NUM_PLY], int idx){
    unsigned long long curr_hash = hashes[idx];
    int num_occasions = 1; // Already seen board once (right now)
    for (int i = idx; i >= 0; i--){
        if (hashes[i] == curr_hash) num_occasions++;
        if (num_occasions > 2) return true;
    }
    return false;
}

uint64_t GetOccupied(const Board *board) {
    return board->bitboards[WhitePawn] | board->bitboards[WhiteKnight] | board->bitboards[WhiteBishop] | board->bitboards[WhiteRook] | board->bitboards[WhiteQueen] | board->bitboards[WhiteKing] |
           board->bitboards[BlackPawn] | board->bitboards[BlackKnight] | board->bitboards[BlackBishop] | board->bitboards[BlackRook] | board->bitboards[BlackQueen] | board->bitboards[BlackKing];
}

uint64_t GetWhiteBitboard(const Board *board) {
    return board->bitboards[WhitePawn] | board->bitboards[WhiteKnight] | board->bitboards[WhiteBishop] | board->bitboards[WhiteRook] | board->bitboards[WhiteQueen] | board->bitboards[WhiteKing];
}

uint64_t GetBlackBitboard(const Board *board) {
    return board->bitboards[BlackPawn] | board->bitboards[BlackKnight] | board->bitboards[BlackBishop] | board->bitboards[BlackRook] | board->bitboards[BlackQueen] | board->bitboards[BlackKing];
}