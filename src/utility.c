#include "utility.h"
#include "board.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool IsOppositeColor(bool white_to_move, int piece){
    return (white_to_move == (piece >> 3)) && piece;
}

bool IsColor(bool white_to_move, int piece){
    return (white_to_move != (piece >> 3)) && piece;
}

int FlipSquare(int square) {
    return square ^ 0b111000;
}


char* SquareToString(int square){
    const int rank = square / 8;
    const int file = square % 8;

    const char ranks[] = "87654321";
    const char files[] = "abcdefgh";

    char* string_square = malloc(sizeof(char) * 2);
    string_square[0] = files[file];
    string_square[1] = ranks[rank];

    return string_square;
}

int StringToSquare(char* square){
    const char rank_char = square[1];
    const char file_char = square[0];

    const char ranks[] = "87654321";
    const char files[] = "abcdefgh";

    int rank;
    int file;
    for (int i = 0; i < 8; i++){
        if (rank_char == ranks[i]) rank = i;
        if (file_char == files[i]) file = i;
    }

    return rank * 8 + file;
}

Piece GetPromotionPiece(Move move){
    switch (GetFlag(move)) {
        case PromoteQueen:
            return BlackQueen;
        case PromoteKnight:
            return BlackKnight;
        case PromoteRook:
            return BlackRook;
        case PromoteBishop:
            return BlackBishop;
    }
}

char* MoveToString(const Move move){
    char* from = SquareToString(StartSquare(move));
    char* to = SquareToString(TargetSquare(move));

    char* string_move = malloc(5);

    string_move[0] = from[0];
    string_move[1] = from[1];
    string_move[2] = to[0];
    string_move[3] = to[1];
    string_move[4] = '\0';  // null-terminate
    if (IsPromotion(move)){
        string_move = realloc(string_move, 6);
        string_move[4] = PieceToChar(GetPromotionPiece(move));
        string_move[5] = '\0';  // null-terminate
    }

    free(from);
    free(to);

    return string_move;
}

Move StringToMove(char* move, Board *board) {
    const int startSquare = StringToSquare(move);
    const int targetSquare = StringToSquare(move + 2);
    int promotionRank = board->white_to_move ? 1 : 6;
    Piece piece = board->squares[startSquare];

    const int start_file = startSquare % 8;
    const int start_rank = startSquare / 8;

    const int target_file = targetSquare % 8;
    const int target_rank = targetSquare / 8;

    int MoveFlag = none;


    if (GetType(piece) == Pawn)
    {
        if (start_file != target_file)
        {

            if (board->squares[targetSquare] == 0)
            {
                MoveFlag = EnPassant;
            }
        }
        if (abs(start_rank - target_rank) == 2)
        {
            MoveFlag = DoublePush;
        }
        if (start_rank == promotionRank)
        {
            PieceType promotionPieceType = GetType(CharToPiece(move[4]));

            switch (promotionPieceType)
            {
                case Queen:
                    MoveFlag = PromoteQueen;
                    break;
                case Rook:
                    MoveFlag = PromoteRook;
                    break;
                case Knight:
                    MoveFlag = PromoteKnight;
                    break;
                case Bishop:
                    MoveFlag = PromoteBishop;
                    break;

            }
        }

    }
    else if (GetType(piece) == King)
    {
        if ((startSquare == 60 && (targetSquare == 62 || targetSquare == 58)) ||
            (startSquare == 4 && (targetSquare == 6 || targetSquare == 2)))
        {
            MoveFlag = Castle;
        }

    }

    return MoveConstructor(startSquare, targetSquare, MoveFlag);
}

char PieceToChar(const Piece piece) {
    switch (piece) {
        case None:
            return ' ';
        case WhitePawn:
            return 'P';
        case WhiteKnight:
            return 'N';
        case WhiteBishop:
            return 'B';
        case WhiteRook:
            return 'R';
        case WhiteQueen:
            return 'Q';
        case WhiteKing:
            return 'K';
        case BlackPawn:
            return 'p';
        case BlackKnight:
            return 'n';
        case BlackBishop:
            return 'b';
        case BlackRook:
            return 'r';
        case BlackQueen:
            return 'q';
        case BlackKing:
            return 'k';
        default:
            exit(-1);
    }
}

Piece CharToPiece(const char piece) {
    switch (piece) {
        case 'P':
            return WhitePawn;
        case 'N':
            return WhiteKnight;
        case 'B':
            return WhiteBishop;
        case 'R':
            return WhiteRook;
        case 'Q':
            return WhiteQueen;
        case 'K':
            return WhiteKing;
        case 'p':
            return BlackPawn;
        case 'n':
            return BlackKnight;
        case 'b':
            return BlackBishop;
        case 'r':
            return BlackRook;
        case 'q':
            return BlackQueen;
        case 'k':
            return BlackKing;
        default:
            return None;
    }
}

bool IsLegalCastle(const Board *board, Move move){
    if (InCheck(board))
    {
        return false;
    }
    switch (TargetSquare(move))
    {
        case 62: // white short castle (g1)
            if (IsAttackedBySideToMove(board, 0, 61) || IsAttackedBySideToMove(board, 0, 62)) { return false; }
            break;
        case 58: // white loing castle (c1)
            if (IsAttackedBySideToMove(board, 0, 58) || IsAttackedBySideToMove(board, 0, 59)) { return false; }
            break;
        case 6: // black short castle (g8)
            if (IsAttackedBySideToMove(board, 1, 5) || IsAttackedBySideToMove(board, 1, 6)) { return false; }
            break;
        case 2: // black long castle (c8)
            if (IsAttackedBySideToMove(board, 1, 2) || IsAttackedBySideToMove(board, 1, 3)) { return false; }
            break;
    }
    return true;
}

PieceType GetType(Piece piece) {
    return piece & 0b0111;
}

int getlsb(unsigned long long bb) {
    assert(bb);  // lsb(0) is undefined
    return __builtin_ctzll(bb);
}

int poplsb(unsigned long long *bb) {
    int lsb = getlsb(*bb);
    *bb &= *bb - 1;
    return lsb;
}

char* BoardToFen(const Board *board){
    char* fen = malloc(256);
    int fen_index = 0;

    int squares_since_last_piece;
    for (int rank = 0; rank < 8; rank++){
        for (int file = 0; file < 8; file++){
            const int square = rank * 8 + file;

            if (board->squares[square] == None){
                squares_since_last_piece++;
            }
            else{
                if (squares_since_last_piece != 0){
                    fen[fen_index++] = squares_since_last_piece + '0';
                    squares_since_last_piece = 0;
                }
                fen[fen_index++] = PieceToChar(board->squares[square]);
            }
        }
        if (squares_since_last_piece != 0){
            fen[fen_index++] = squares_since_last_piece + '0';
            squares_since_last_piece = 0;
        }
        if (rank != 7)
            fen[fen_index++] = '/';
    }

    fen[fen_index++] = ' ';
    fen[fen_index++] = board->white_to_move ? 'w' : 'b';
    fen[fen_index++] = ' ';

    if (board->white_kingside) fen[fen_index++] = 'K';
    if (board->white_queenside) fen[fen_index++] = 'Q';
    if (board->black_kingside) fen[fen_index++] = 'k';
    if (board->black_queenside) fen[fen_index++] = 'q';

    if (!(board->white_kingside || board->white_queenside || board->black_kingside || board->black_queenside)){
        fen[fen_index++] = '-';
    }
    fen[fen_index++] = ' ';

    if (board->en_passant_square == -1){
        fen[fen_index++] = '-';
    }
    else{
        char* en_passant_square = SquareToString(board->en_passant_square);

        fen[fen_index++] = en_passant_square[0];
        fen[fen_index++] = en_passant_square[1];

        free(en_passant_square);
    }

    fen[fen_index++] = ' ';
    fen[fen_index++] = '0';
    fen[fen_index++] = ' ';
    fen[fen_index++] = '1';
    fen[fen_index++] = '\0';

    return fen;
}

void PrintBitBoard(uint64_t bitboard) {
    char pieces[64];
    for (int i = 0; i < 64; i++) {
        if (bitboard & (1ULL << i)) pieces[i] = '1';
        else pieces[i] = ' ';
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