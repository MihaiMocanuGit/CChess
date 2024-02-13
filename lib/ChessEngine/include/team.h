#ifndef CCHESS_TEAM_H
#define CCHESS_TEAM_H

#include <stdbool.h>
#include "piece.h"

#define TEAM_INITIAL_NO_PIECES 32
typedef struct
{
    PieceTeam_e teamColor;
    Piece pieces[TEAM_INITIAL_NO_PIECES];
    unsigned noPieces;


    //The pieces of this team that are taken by an enemy are moved into this array
    Piece lostPieces[TEAM_INITIAL_NO_PIECES];
    unsigned noLostPieces;

    // I've decided to give a team the responsibility of its own captured pieces
    // because if we decide to customize the chess game mode to support more than
    // 2 sides, a single team could capture more than TEAM_INITIAL_NO_PIECES pieces.
    // In this case we cannot know the maximum theoretical amount of pieces one can
    // capture.
    // But it is obvious that one side cannot lose more pieces than it started with.
    // Therefore, modeling it this way makes more practical sense.

} Team;

//Adds the given piece without making any checks on the type of the piece/free space etc
void team_addPiece(Team *team, const Piece *piece);


void team_initEmpty(Team *team, PieceTeam_e color);
Team team_constructEmpty(PieceTeam_e color);

//if the pieces array did not get corrupted, the first element of the array should always be the king;
Piece *team_king(const Team *team);



#endif //CCHESS_TEAM_H
