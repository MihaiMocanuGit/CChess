#include "team.h"

#include <stdlib.h>


void team_addPiece(Team *team, const Piece *piece)
{
    team->pieces[team->noPieces++] = *piece;
}


void team_initEmpty(Team *team, PieceTeam_e color)
{
    team->teamColor = color;
    team->noPieces = 0;
    team->rooks[0] = NULL;
    team->rooks[1] = NULL;
    team->king = NULL;
    team->noLostPieces = 0;
}
Team team_constructEmpty(PieceTeam_e color)
{
    Team team;
    team_initEmpty(&team, color);
    return team;
}
