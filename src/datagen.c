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
        if (board->squares[FlipSquare(i)]) {
            occupied |= 1ULL << i;
        }
    }

    return occupied;
}

Board PrepareGame(Thread *this) {
    unsigned long long* seed = &this->thread_id;
    PseudorandomNumber(seed);

    Board rand_pos = GenerateRandomPosition(seed);
    this->game.occupied = GetOccupied(&rand_pos);

    for (int i = 0; i < 16; i++) this->game.pieces[i] = 0;

    int index = 0;
    for (int i = 0; i < 64; i++) {
        const int square = FlipSquare(i);
        if (this->game.occupied & (1ULL << i)) {
            const uint8_t piece = ConvertPiece(rand_pos.squares[square]);

            this->game.pieces[index / 2] |= piece << (((index + 1) % 2) * 4);
            index++;
        }
    }

    this->game.stm_enPassant = 0;
    this->game.stm_enPassant |= rand_pos.en_passant_square == -1 ? 64 : rand_pos.en_passant_square;
    this->game.stm_enPassant |= !rand_pos.white_to_move << 7;

    this->game.half_move = 0;
    this->game.full_move = 1;
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

double PlayGame(Thread *this) {
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
    this->game.ply = stack.hash_index;

    this->thread_id = PseudorandomNumber(&this->thread_id);
    return 0;
}

void WriteGame(Game *game, FILE *file) {
    fwrite(&game->occupied, sizeof(unsigned long long), 1, file);
    fwrite(&game->pieces, sizeof(uint8_t), 16, file);
    fwrite(&game->stm_enPassant, sizeof(uint8_t), 1, file);

    fwrite(&game->half_move, sizeof(uint8_t), 1, file);
    fwrite(&game->full_move, sizeof(uint16_t), 1, file);
    fwrite(&game->score, sizeof(int16_t), 1, file);
    fwrite(&game->result, sizeof(uint8_t), 1, file);

    constexpr uint8_t padding = 0;
    fwrite(&padding, sizeof(uint8_t), 1, file); // Padding

    fwrite(&game->moves, sizeof(unsigned long), game->ply, file);

    constexpr unsigned int num = 0;
    fwrite(&num, sizeof(unsigned int), 1, file); // End of game
    fflush(file);
}

void* GameLoop(void* arg) {
    Thread *this = (Thread *)arg;
    double start = clock();
    int positions = 0;

    while (1) {
        PlayGame(this);

        printf("HMM\n");
        pthread_mutex_lock(&mutex);

        finished_games++;

        WriteGame(&this->game, this->file);
        positions += this->game.ply;
        const double elapsed = clock() - start;
        if (!(finished_games % 50)) {
            printf("Games: %d  Positions: %d  Time elapsed: %lfs\n", finished_games, positions, elapsed / CLOCKS_PER_SEC);
        }
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
        pthread_create(&threads[i], nullptr, GameLoop, &states[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], nullptr);
    }
}