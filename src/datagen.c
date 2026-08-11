#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <inttypes.h>

#include "nnue.h"
#include "board.h"
#include "search.h"
#include "datagen.h"
#include "utility.h"
#include "zobrist.h"
#include "moveGeneration.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #define sleep_ms(ms) Sleep(ms) // Windows accepts milliseconds
#else
    #include <unistd.h>
    #define sleep_ms(ms) usleep((ms) * 1000) // Linux accepts microseconds
#endif

pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;

Piece ConvertPiece(Piece piece) {
    return piece == 0 ? 0 : ((piece & 0b0111) - 1) | (piece & 0b1000);
}

Move ConvertMove(Move move) {
    const int flag = GetFlag(move);

    int new_flag = 0;
    int new_start_square = 0;
    int new_target_square = 0;

    if (flag == Castle) {
        switch (TargetSquare(move)) {
            case 62:
                new_target_square = 7;
                break;
            case 58:
                new_target_square = 0;
                break;
            case 6:
                new_target_square = 63;
                break;
            case 2:
                new_target_square = 56;
                break;
        }
        new_flag = 0b1000;
    }
    else {
        new_target_square = FlipRank(TargetSquare(move));
        if (flag == EnPassant) {
            new_flag = 0b0100;
        }
        else if (IsPromotion(move)) {
            new_flag = 0b1100;
            switch (flag) {
                case PromoteQueen:
                    new_flag |= 0b0011;
                    break;
                case PromoteKnight:
                    new_flag |= 0b0000;
                    break;
                case PromoteBishop:
                    new_flag |= 0b0001;
                    break;
                case PromoteRook:
                    new_flag |= 0b0010;
                    break;
            }
        }
    }
    new_start_square = FlipRank(StartSquare(move));

    return MoveConstructor(new_target_square, new_start_square, new_flag);


}

bool IsCheckmate(Board* board){
    Move moves[256];
    int num_moves = GetMoves(board, moves);

    return num_moves == 0;
}

Board GenerateRandomPosition(uint64_t *seed) {
    Board board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Board prev_copy = board;
    int num_rand_moves = ((*seed >> 43) & 1ULL) == 1 ? 8 : 9;
    for (int num_deep = 0; num_deep < num_rand_moves; num_deep++) {
        Move moves[256];
        int num_moves = GetMoves(&board, moves);

        if (num_moves == 0) {
            board = prev_copy;
            num_deep--;
            PseudorandomNumber(seed);
        }
        else {
            uint64_t rand_index = *seed % num_moves;
            PseudorandomNumber(seed);
            assert(moves[rand_index].value != 0);
            MakeMove(&board, moves[rand_index]);
        }
        prev_copy = board;
        if (IsCheckmate(&board)){
            board = prev_copy;
            num_deep--;
            PseudorandomNumber(seed);
        }
    }
    return board;
}

uint64_t GetViriOccupied(Board *board) {
    uint64_t occupied = 0;

    for (int i = 0; i < 64; i++) {
        if (board->squares[FlipRank(i)]) {
            occupied |= 1ULL << i;
        }
    }

    return occupied;
}

Board PrepareGame(DatagenInfo *this, Thread* thread) {
    uint64_t* seed = &this->seed;
    PseudorandomNumber(seed);
    Board rand_pos = GenerateRandomPosition(seed);
    init_accumulator_stack(thread, &rand_pos, &thread->nnue);
    // Try at most 100 different positions
    for (int i = 0; i < 100; i++){
        thread->board = rand_pos;
        if (IsCheckmate(&rand_pos))
        {
            rand_pos = GenerateRandomPosition(seed);
            continue;
        }
        const SearchResult result = search(thread);
        if (abs(result.score) < 2000) break;
        rand_pos = GenerateRandomPosition(seed);
    }
    this->game.occupied = GetViriOccupied(&rand_pos);

    for (int i = 0; i < 16; i++) this->game.pieces[i] = 0;

    int index = 0;
    for (int i = 0; i < 64; i++) {
        const int square = FlipRank(i);
        if (this->game.occupied & (1ULL << i)) {
            uint8_t piece = ConvertPiece(rand_pos.squares[square]);

            if ((piece & 0b0111) == 0b0011) {// Is a rook
                piece &= 0b1000;
                if (rand_pos.white_kingside && square == 63) {
                    piece |= 0b0110;
                }
                else if (rand_pos.white_queenside && square == 56) {
                    piece |= 0b0110;
                }
                else if (rand_pos.black_kingside && square == 7) {
                    piece |= 0b0110;
                }
                else if (rand_pos.black_queenside && square == 0) {
                    piece |= 0b0110;
                }
                else {
                    piece |= 0b0011;
                }
            }

            this->game.pieces[index / 2] |= piece << ((index % 2) * 4);
            index++;
        }
    }

    this->game.stm_enPassant = 0;
    this->game.stm_enPassant |= rand_pos.en_passant_square == -1 ? 64 : FlipRank(rand_pos.en_passant_square);
    this->game.stm_enPassant |= !rand_pos.white_to_move << 7;

    this->game.half_move = 0;
    this->game.full_move = 1;
    this->game.score = 0;

    this->game.result = 3;
    return rand_pos;
}

double PlayGame(DatagenInfo *this, Thread* thread) {

    thread->nodes = 0;
    thread->node_limit = 500000;
    thread->print_info = false;
    thread->soft_node_limit = 5000;
    thread->board = PrepareGame(this, thread);

    Board* board = &thread->board;
    board->game_ply = 0;
    while (1){
        thread->hashes[board->game_ply] = board->zobrist_hash;

        if (IsCheckmate(board)){
            this->game.result = board->white_to_move ? 0 : 2;
            break;
        }
        if (IsDraw(thread->hashes, board) || board->game_ply > 512){ // Hard limit on length
            this->game.result = 1;
            break;
        }

        const SearchResult result = search(thread);
        this->nodes += result.nodes;
        this->positions++;
        assert(result.best_move.value != 0);

        thread->nodes = 0;
        const Move converted_move = ConvertMove(result.best_move);

        this->game.moves[board->game_ply] = 0;
        this->game.moves[board->game_ply] |= converted_move.value;
        this->game.moves[board->game_ply] |= ((board->white_to_move ? 1 : -1) * result.score) << 16;

        update_accumulators(board, result.best_move, &thread->nnue);
        MakeMove(board, result.best_move);
    }

    this->game.ply = board->game_ply;
    this->seed = PseudorandomNumber(&this->seed);
    this->games++;

    return 0;
}

void WriteGame(Game *game, FILE *file) {
    fwrite(&game->occupied, sizeof(uint64_t), 1, file);
    fwrite(&game->pieces, sizeof(uint8_t), 16, file);
    fwrite(&game->stm_enPassant, sizeof(uint8_t), 1, file);

    fwrite(&game->half_move, sizeof(uint8_t), 1, file);
    fwrite(&game->full_move, sizeof(uint16_t), 1, file);
    fwrite(&game->score, sizeof(int16_t), 1, file);
    fwrite(&game->result, sizeof(uint8_t), 1, file);

    constexpr uint8_t padding = 0;
    fwrite(&padding, sizeof(uint8_t), 1, file); // Padding

    fwrite(&game->moves, sizeof(uint32_t), game->ply, file);

    constexpr unsigned int num = 0;
    fwrite(&num, sizeof(unsigned int), 1, file); // End of game
    fflush(file);
}

void* GameLoop(void *arg) {
    DatagenInfo* this = arg;

    while (1) {
        PlayGame(this, this->thread);
        pthread_mutex_lock(&data_mutex);
        WriteGame(&this->game, this->file);
        pthread_mutex_unlock(&data_mutex);
    }
    return NULL;
}

void PrinfInfo(int num_threads, DatagenInfo* infos, int time_ms)
{
    uint64_t total_nodes = 0;
    uint64_t total_positions = 0;
    int total_games = 0;
    for (int i = 0; i < num_threads; i++)
    {
        printf("\x1b[2KThread %"PRIu64": nodes/sec %"PRIu64"  pos/sec %"PRIu64" games/sec %d\n", infos[i].thread_id, infos[i].nodes * 1000 / time_ms, infos[i].positions * 1000 / time_ms, infos[i].games * 1000 / time_ms);
        total_nodes += infos[i].nodes;
        total_games += infos[i].games;
        total_positions += infos[i].positions;
        infos[i].nodes = 0;
        infos[i].games = 0;
        infos[i].positions = 0;
    }
    printf("\x1b[2KTotal: nodes/sec %"PRIu64"  pos/sec %"PRIu64" games/sec %d\n", total_nodes * 1000 / time_ms, total_positions * 1000 / time_ms, total_games * 1000 / time_ms);
}

void PrintInfoLoop(int num_threads, DatagenInfo* infos)
{
    const int sleep_time = 5000;
    sleep_ms(sleep_time);
    pthread_mutex_lock(&data_mutex);
    PrinfInfo(num_threads, infos, sleep_time);
    pthread_mutex_unlock(&data_mutex);
    while (true)
    {
        sleep_ms(sleep_time);
        pthread_mutex_lock(&data_mutex);
        printf("\x1b[%dA", num_threads + 1);
        PrinfInfo(num_threads, infos, sleep_time);
        fflush(stdout);
        pthread_mutex_unlock(&data_mutex);
    }
}

void Datagen(char* file_path, int num_threads, uint64_t seed) {

    FILE *file = fopen(file_path, "ab");

    DatagenInfo infos[num_threads];

    pthread_t threads[num_threads];
    for (int i = 0; i < num_threads; i++) {
        Thread* thread = malloc(sizeof(Thread));
         *thread = (Thread){
            .depth_limit = 255,
            .time_limit = INT64_MAX,
            .soft_time_limit = INT64_MAX,
        };

        int num_entries = 16 * 1000000 / sizeof(Entry);
        Entry* entries = malloc(num_entries * sizeof(Entry));
        thread->tt.num_entries = num_entries;
        thread->tt.entries = entries;
        ZeroTT(&thread->tt);

        infos[i].file = file;
        infos[i].thread_id = i;
        infos[i].seed = PseudorandomNumber(&seed) + PseudorandomNumber(&seed);
        infos[i].thread = thread;
        infos[i].games = 0;
        infos[i].nodes = 0;
        infos[i].positions = 0;
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_create(&threads[i], nullptr, GameLoop, &infos[i]);
    }

    PrintInfoLoop(num_threads, infos);

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], nullptr);
    }
}