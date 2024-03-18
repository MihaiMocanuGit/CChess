#ifndef CCHESS_PIECE_H
#define CCHESS_PIECE_H

#define PIECE_PAWN_VALUE 1
#define PIECE_KNIGHT_VALUE 3
#define PIECE_BISHOP_VALUE 3
#define PIECE_ROOK_VALUE 5
#define PIECE_QUEEN_VALUE 9
#define PIECE_KING_VALUE 50 //TODO: Should the king have a value?

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
    BLACK
} PieceTeam_e;

typedef struct
{
    PieceTeam_e team;
    PieceType_e type;
    int x, y;
    int movesMade;
    int value;

} Piece;

Piece piece_construct(PieceTeam_e team, PieceType_e type, int x, int y, int value);
void piece_init(Piece *piece, PieceTeam_e team, PieceType_e type, int x, int y, int value);


#endif //CCHESS_PIECE_H
