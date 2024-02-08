#include "project/team.h"

#include <stdlib.h>


void team_addPiece(Team *team, const Piece *piece)
{
    team->pieces[team->noPieces++] = *piece;
}

//Gets the last successfully added piece,
//Does not check for empty data;
static Piece* getLastAddedPiece(Team *team)
{
    return &(team->pieces[team->noPieces - 1]);
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

void team_initDefaultTop(Team *team, PieceTeam_e color)
{   //
    //      0|W 1|B 2|W 3|B 4|W 5|B 6|W 7|B
    //  0   R   K   B   Q/K K/Q B   K   R
    //  1   P   P   P   P   P   P   P   P
    team_initEmpty(team, color);
    for (int i = 0; i < 8; ++i)
    {
        Piece pawn = piece_construct(color, PAWN, i, 1);
        team_addPiece(team, &pawn);
    }

    Piece rookLeft = piece_construct(color, ROOK, 0, 0);
    team_addPiece(team, &rookLeft);
    team->rooks[0] = getLastAddedPiece(team);

    Piece knightLeft = piece_construct(color, KNIGHT, 1, 0);
    team_addPiece(team, &knightLeft);

    Piece bishopLeft = piece_construct(color, BISHOP, 2, 0);
    team_addPiece(team, &bishopLeft);

    //The queen will be placed on a spot of the same color
    if (color == WHITE)
    {
        Piece queenBlack = piece_construct(BLACK, QUEEN, 3, 0);
        team_addPiece(team, &queenBlack);

        Piece kingBlack = piece_construct(BLACK, KING, 4, 0);
        team_addPiece(team, &kingBlack);
        team->king = getLastAddedPiece(team);
    }
    else
    {
        Piece kingWhite = piece_construct(WHITE, KING, 3, 0);
        team_addPiece(team, &kingWhite);
        team->king = getLastAddedPiece(team);

        Piece queenWhite = piece_construct(WHITE, QUEEN, 4, 0);
        team_addPiece(team, &queenWhite);
    }

    Piece bishopRight = piece_construct(color, BISHOP, 5, 0);
    team_addPiece(team, &bishopRight);

    Piece knightRight = piece_construct(color, KNIGHT, 6, 0);
    team_addPiece(team, &knightRight);

    Piece rookRight = piece_construct(color, ROOK, 7, 0);
    team_addPiece(team, &rookRight);
    team->rooks[1] = getLastAddedPiece(team);


}

Team team_constructDefaultTop(PieceTeam_e color)
{
    Team team;
    team_initDefaultTop(&team, color);
    return team;
}


void team_initDefaultBottom(Team *team, PieceTeam_e color)
{
    //place the pieces at the top first
    team_initDefaultTop(team, color);

    //and reflect them to the bottom
    for (int i = 0; i < team->noPieces; ++i)
        team->pieces[i].y = (8 - 1) - team->pieces[i].y;

}
Team team_constructDefaultBottom(PieceTeam_e color)
{
    Team team;
    team_initDefaultBottom(&team, color);
    return team;
}