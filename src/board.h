#ifndef CAIRN_BOARD_H
#define CAIRN_BOARD_H

typedef enum{
    None = 0b0000,
    WhitePawn = 0b0001,
    WhiteKnight = 0b0010,
    WhiteBishop = 0b0011,
    WhiteRook = 0b0100,
    WhiteQueen = 0b0101,
    WhiteKing = 0b0110,
    BlackPawn = 0b1001,
    BlackKnight = 0b1010,
    BlackBishop = 0b1011,
    BlackRook = 0b1100,
    BlackQueen = 0b1101,
    BlackKing = 0b1110,
} Piece;

typedef enum{
    Pawn = 0b0001,
    Knight = 0b0010,
    Bishop = 0b0011,
    Rook = 0b0100,
    Queen = 0b0101,
    King = 0b0110,
} PieceType;

typedef struct{
    Piece squares[64];
    bool white_to_move;
    bool white_kingside;
    bool white_queenside;
    bool black_kingside;
    bool black_queenside;
} Board;


#endif //CAIRN_BOARD_H
