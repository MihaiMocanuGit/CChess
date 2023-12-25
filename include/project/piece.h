#ifndef CCHESS_PIECE_H
#define CCHESS_PIECE_H


typedef enum
{
    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING
} PieceType_e;

typedef enum
{
    WHITE,
    BLACK,
    TAKEN_BY_WHITE,
    TAKEN_BY_BLACK,
} PieceTeam_e;

typedef struct
{
    PieceTeam_e team;
    PieceType_e type;
    int x, y;

} Piece;

Piece piece_construct(PieceTeam_e team, PieceType_e type, int x, int y);
void piece_init(Piece *piece, PieceTeam_e team, PieceType_e type, int x, int y);


#endif //CCHESS_PIECE_H
