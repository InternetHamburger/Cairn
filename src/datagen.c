#include "datagen.h"
#include "board.h"
#include "utility.h"
#include "search.h"
#include "zobrist.h"
#include "moveGeneration.h"
#include "transposition.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>

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
        new_target_square = FlipSquare(TargetSquare(move));
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
    new_start_square = FlipSquare(StartSquare(move));

    return MoveConstructor(new_target_square, new_start_square, new_flag);


}

bool IsCheckmate(Board* board){
    int num_moves = 0;
    Move moves[256];
    GetMoves(board, moves, &num_moves);
    const Board copy = *board;
    for (int i = 0; i < num_moves; i++) {

        if (GetFlag(moves[i]) == Castle && !IsLegalCastle(board, moves[i])){
            continue;
        }
        assert(moves[i].value != 0);
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

Board GenerateRandomPosition(unsigned long long *seed) {
    Board board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Board prev_copy = board;
    int num_rand_moves = ((*seed >> 43) & 1ULL) == 1 ? 8 : 9;
    for (int num_deep = 0; num_deep < num_rand_moves; num_deep++) {
        int num_moves = 0;
        Move moves[256];
        GetMoves(&board, moves, &num_moves);


        int num_legal_moves = 0;
        Move* legal_moves = malloc(sizeof(Move) * num_moves);
        const Board copy = board;
        for (int i = 0; i < num_moves; i++) {
            if (GetFlag(moves[i]) == Castle && !IsLegalCastle(&board, moves[i])){
                continue;
            }
            assert(moves[i].value != 0);
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
            assert(legal_moves[rand_index].value != 0);
            MakeMove(&board, legal_moves[rand_index]);
        }
        prev_copy = copy;
        free(legal_moves);
        if (IsCheckmate(&board)){
            board = prev_copy;
            num_deep--;
            PseudorandomNumber(seed);
        }
    }
    return board;
}

unsigned long long GetViriOccupied(Board *board) {
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
    // Try at most 100 different positions
    for (int i = 0; i < 100; i++){
        Stack stack = {
                .nodes = 0,
                .node_limit = 500000,
                .print_info = false,
                .depth_limit = 255,
                .soft_node_limit = 8000,
                .time_limit = INT64_MAX,
                .hash_index = 0
        };
        const SearchResult result = search(&rand_pos, &stack);
        if (abs(result.score) < 750) break;
        rand_pos = GenerateRandomPosition(seed);
    }

    this->game.result = 3;
    return rand_pos;
}

double PlayGame(Thread *this) {
    Board board = PrepareGame(this);

    Stack stack = {
            .nodes = 0,
            .node_limit = 500000,
            .print_info = false,
            .depth_limit = 255,
            .soft_node_limit = 5000,
            .time_limit = INT_MAX,
            .hash_index = 0
    };


    Board end_position;
    while (1){
        stack.hashes[stack.hash_index] = board.zobrist_hash;

        if (IsCheckmate(&board)){
            this->game.result = board.white_to_move ? 0 : 2;
            break;
        }
        if (IsRepetition(stack.hashes, stack.hash_index) || board.fifty_move_counter >= 100 || stack.hash_index > 512){ // Hard limit on length
            this->game.result = 1;
            break;
        }

        const SearchResult result = search(&board, &stack);
        assert(result.best_move.value != 0);

        stack.nodes = 0;
        const Move converted_move = ConvertMove(result.best_move);
        this->game.scores[stack.hash_index] = (board.white_to_move ? 1 : -1) * result.score;

        end_position = board;
        for (int move = 0; move < result.pv.length; move++){
            MakeMove(&end_position, result.pv.line[move]);
        }
        this->game.end_positions[stack.hash_index] = end_position;

        MakeMove(&board, result.best_move);
        stack.hash_index++;

    }

    this->game.ply = stack.hash_index;
    this->thread_id = PseudorandomNumber(&this->thread_id);

    return 0;
}

void WriteGame(Game *game, FILE *file) {
    for (int pos = 0; pos < game->ply; pos++){
        char* fen = BoardToFen(&game->end_positions[pos]);
        fprintf(file, "%s | %d | %f\n", fen, game->scores[pos], game->result / 2.0);
    }
    fflush(file);
}

void* GameLoop(Thread *this) {

    HANDLE hMutex = CreateMutex(NULL, FALSE, "Global\\DatagenFileMutex");

    while (1) {
        PlayGame(this);

        WaitForSingleObject(hMutex, INFINITE);
        WriteGame(&this->game, this->file);
        ReleaseMutex(hMutex);
        break;
    }
    return NULL;
}

void Datagen(char* file_path, char* this_path, int num_threads, uint64_t seed) {
    for (int i = 0; i < num_threads; i++) {

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        HANDLE hJob = CreateJobObject(NULL, NULL);
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = {0};
        jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        SetInformationJobObject(hJob, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli));

        AssignProcessToJobObject(hJob, pi.hProcess);

        char cmdLine[256];
        uint64_t num_0 = PseudorandomNumber(&seed);
        uint64_t num_1 = PseudorandomNumber(&seed);
        seed += num_0 + num_1;
        sprintf(cmdLine, "datagen seed %llu output %s", seed, file_path);
        printf("%s this %s\n", cmdLine, this_path);
        if (!CreateProcess(
                this_path,       // path to executable
                cmdLine,         // command line arguments (NULL if none)
                nullptr,         // process security attributes
                nullptr,         // thread security attributes
                FALSE,           // inherit handles
                0,               // creation flags
                NULL,            // environment
                nullptr,         // current directory
                &si,
                &pi
        )) {
            printf("CreateProcess failed (%lu).\n", GetLastError());
            continue;
        }

        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);

    }
}