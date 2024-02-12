#include "piece.h"

Piece piece_construct(PieceTeam_e team, PieceType_e type, int x, int y)
{
    Piece piece = { .team = team, .type = type, .x = x, .y = y, .movesMade = 0};
    return piece;
}

void piece_init(Piece *piece, PieceTeam_e team, PieceType_e type, int x, int y)
{
    piece->team = team;
    piece->type = type;
    piece->x = x;
    piece->y = y;
    piece->movesMade = 0;
}
