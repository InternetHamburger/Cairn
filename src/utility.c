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

    const char ranks[] = "12345678";
    const char files[] = "hgfedcba";

    int rank;
    int file;
    for (int i = 0; i < 8; i++){
        if (rank_char == ranks[i]) rank = i;
        if (file_char == files[i]) file = i;
    }

    return rank * 8 + file;
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

    free(from);
    free(to);

    return string_move;
}

Move StringToMove(char* move, Board *board) {
    const int startSquare = StringToSquare(move);
    const int targetSquare = StringToSquare(move + 2);
    int promotionRank = board->white_to_move ? 1 : 6; // Rank for promotion
    Piece piece = board->squares[startSquare];

    const int start_file = startSquare % 8;
    const int start_rank = startSquare / 8;

    const int target_file = startSquare % 8;
    const int target_rank = startSquare / 8;

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
                if (Math.Abs(start_rank - target_rank) == 2)
                {
                    MoveFlag = Move.PawnTwoUpFlag;
                }
                if (BoardUtility.Rank(startSquare) == promotionRank)
                {
                    byte? promotionPieceType = Piece.GetType(move[4]) ?? throw new NotImplementedException();
                    promotionPieceType = (byte)promotionPieceType;

                    byte pievce = promotionPieceType.Value;

                    switch (Piece.PieceType(pievce))
                    {
                        case Piece.Queen:
                            MoveFlag = Move.PromoteToQueenFlag;
                            break;
                        case Piece.Rook:
                            MoveFlag = Move.PromoteToRookFlag;
                            break;
                        case Piece.Knight:
                            MoveFlag = Move.PromoteToKnightFlag;
                            break;
                        case Piece.Bishop:
                            MoveFlag = Move.PromoteToBishopFlag;
                            break;

                    }
                }

            }
            else if (Piece.PieceType(piece) == Piece.King)
            {
                if ((startSquare == 60 && (targetSquare == 62 || targetSquare == 58)) ||
                    (startSquare == 4 && (targetSquare == 6 || targetSquare == 2)))
                {
                    MoveFlag = Move.CastleFlag;
                }

            }

            return new Move(startSquare, targetSquare, MoveFlag);
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