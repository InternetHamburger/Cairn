#include "datagen.h"
#include "board.h"
#include "utility.h"
#include "search.h"
#include "zobrist.h"
#include "moveGeneration.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int finished_games = 0;

Piece ConvertPiece(Piece piece) {
    return piece == 0 ? 0 : ((piece & 0b0111) - 1) | (piece & 0b1000);
}

Move ConvertMove(Move move) {
    int flag = GetFlag(move);

    int new_flag = 0;
    int new_start_square = 0;
    int new_target_square = 0;

    if (flag == Castle) {
        switch (TargetSquare(move)) {
            case 62:
                new_target_square = 7;
            case 57:
                new_target_square = 0;
            case 6:
                new_target_square = 63;
            case 1:
                new_target_square = 56;
        }
        new_flag = 0b1000;
    }
    else {
        new_target_square = FlipSquare(TargetSquare(move));
        if (flag == EnPassant) {
            new_flag = 0b0100;
        }
        else if (IsPromotion(move)) {
            new_flag = 0b1100;
            switch (flag) {
                PromoteQueen:
                    new_flag |= 0b0011;
                PromoteKnight:
                    new_flag |= 0b0000;
                PromoteBishop:
                    new_flag |= 0b0001;
                PromoteRook:
                    new_flag |= 0b0010;
            }
        }
    }
    new_start_square = FlipSquare(StartSquare(move));

    return MoveConstructor(new_target_square, new_start_square, new_flag);


}

Board GenerateRandomPosition(unsigned long long *seed) {
    Board board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Board prev_copy = board;
    int num_rand_moves = ((*seed >> 43) & 1ULL) == 1 ? 8 : 9;
    for (int num_deep = 0; num_deep < num_rand_moves; num_deep++) {
        int num_moves = 0;
        Move* moves = GetMoves(&board, &num_moves);

        int num_legal_moves = 0;
        Move* legal_moves = malloc(sizeof(Move) * num_moves);
        const Board copy = board;
        for (int i = 0; i < num_moves; i++) {
            if (GetFlag(moves[i]) == Castle && !IsLegalCastle(&board, moves[i])){
                continue;
            }
            MakeMove(&board, moves[i]);
            if (IsAttackedBySideToMove(&board, board.white_to_move, board.white_to_move ? board.black_king_square : board.white_king_square)) {
                board = copy;
                continue;
            }
            legal_moves[num_legal_moves++] = moves[i];

            board = copy;
        }
        if (num_legal_moves == 0) {
            board = prev_copy;
            num_deep--;
            PseudorandomNumber(seed);
        }
        else {
            unsigned long long rand_index = *seed % num_legal_moves;
            PseudorandomNumber(seed);

            MakeMove(&board, legal_moves[rand_index]);
        }
        prev_copy = copy;
        free(moves);
        free(legal_moves);
    }
    return board;
}

unsigned long long GetOccupied(Board *board) {
    unsigned long long occupied = 0;

    for (int i = 0; i < 64; i++) {
        if (board->squares[i]) {
            occupied |= 1ULL << FlipSquare(i);
        }
    }

    return occupied;
}

Board PrepareGame(Thread *this) {
    unsigned long long* seed = &this->thread_id;
    PseudorandomNumber(seed);

    Board rand_pos = GenerateRandomPosition(seed);
    this->game.occupied = GetOccupied(&rand_pos);

    this->game.pieces_0 = 0;
    for (unsigned long long i = 0; i < 32; i++) {
        const int square = FlipSquare((int)i);
        this->game.pieces_0 |= (unsigned long long)ConvertPiece(rand_pos.squares[square]) << (i * 4);
    }

    this->game.pieces_1 = 1;
    for (unsigned long long i = 32; i < 64; i++) {
        const int square = FlipSquare((int)i);
        this->game.pieces_1 |= (unsigned long long)ConvertPiece(rand_pos.squares[square]) << (i * 4);
    }

    this->game.stm_enPassant_hm = 0;
    this->game.stm_enPassant_hm |= rand_pos.en_passant_square == -1 ? 64 : rand_pos.en_passant_square;
    this->game.stm_enPassant_hm |= (!rand_pos.white_to_move) << 7;

    this->game.full_move = 0;
    this->game.score = 0;

    this->game.result = 3;
    return rand_pos;
}

bool IsCheckmate(Board* board){
    int num_moves = 0;
    Move* moves = GetMoves(board, &num_moves);

    int num_legal_moves = 0;
    const Board copy = *board;
    for (int i = 0; i < num_moves; i++) {
        if (GetFlag(moves[i]) == Castle && !IsLegalCastle(board, moves[i])){
            continue;
        }
        MakeMove(board, moves[i]);
        if (IsAttackedBySideToMove(board, board->white_to_move, board->white_to_move ? board->black_king_square : board->white_king_square)) {
            *board = copy;
            continue;
        }


        *board = copy;

        return false;
    }
    return true;
}

void PlayGame(Thread *this) {

    Board board = PrepareGame(this);

    Stack stack = {
            .nodes = 0,
            .node_limit = 50000,
            .print_info = false,
            .depth_limit = 255,
            .soft_node_limit = 5000,
            .time_limit = INT_MAX,
            .hash_index = 0
    };

    while (1){

        stack.hashes[stack.hash_index] = board.zobrist_hash;
        if (IsCheckmate(&board)){
            this->game.result = board.white_to_move ? 0 : 2;
            break;
        }
        else if (IsRepetition(stack.hashes, stack.hash_index)){
            this->game.result = 1;
            break;
        }

        const SearchResult result = search(&board, &stack);

        stack.nodes = 0;

        MakeMove(&board, result.best_move);
        const Move converted_move = ConvertMove(result.best_move);
        this->game.moves[stack.hash_index] = 0;
        this->game.moves[stack.hash_index] |= converted_move.value;
        this->game.moves[stack.hash_index] |= result.score << 16;

        stack.hash_index++;
    }

    this->game.full_move = stack.hash_index / 2;

    this->thread_id = PseudorandomNumber(&this->thread_id);
}

void WriteGame(Game *game, FILE *file) {

}

void* GameLoop(void* arg) {
    Thread *this = (Thread *)arg;
    const int origin_id = this->thread_id;
    while (1) {
        PlayGame(this);

        pthread_mutex_lock(&mutex);

        finished_games++;
        WriteGame(&this->game, this->file);
        printf("Games finished %d from thread %d\n", finished_games, this->thread_id);
        pthread_mutex_unlock(&mutex);

    }
    return NULL;
}

void Datagen(FILE *file, int num_threads) {
    Thread states[num_threads];
    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; i++) {
        const Thread state = {
            .thread_id = i + 1,
            .file = file
        };
        states[i] = state;
        pthread_create(&threads[i], NULL, GameLoop, &states[i]);
    }
}