#include "team.h"

#include <stdlib.h>


void team_addPiece(Team *team, const Piece *piece)
{
    team->pieces[team->noPieces++] = *piece;
}

const Piece *team_king_const(const Team *team)
{
    return &team->pieces[0];
}
Piece *team_king(Team *team)
{
    return &team->pieces[0];
}
void team_initEmpty(Team *team, PieceTeam_e color)
{
    team->kinkInCheck = false;
    team->teamColor = color;
    team->noPieces = 0;
    team->noLostPieces = 0;
}
Team team_constructEmpty(PieceTeam_e color)
{
    Team team;
    team_initEmpty(&team, color);
    return team;
}
