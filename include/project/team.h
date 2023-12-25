#ifndef CCHESS_TEAM_H
#define CCHESS_TEAM_H

#include "piece.h"

#define TEAM_INITIAL_NO_PIECES 32
typedef struct
{
    Piece pieces[TEAM_INITIAL_NO_PIECES];
    unsigned noPieces;

    //direct access reference to all the pawns in the pieces array
    Piece *pawns[TEAM_INITIAL_NO_PIECES];
    unsigned noPawns;

    //direct access reference to all the bishops in the pieces array
    Piece *bishops[TEAM_INITIAL_NO_PIECES];
    unsigned noBishops;

    //direct access reference to all the rooks in the pieces array
    Piece *rooks[TEAM_INITIAL_NO_PIECES];
    unsigned noRooks;

    //direct access reference to all the queens in the pieces array
    Piece *queens[TEAM_INITIAL_NO_PIECES];
    unsigned noQueens;

    //direct access reference to the king in the pieces array
    Piece *king;
    unsigned noKing;

    //The pieces that are taken by an enemy team are moved into this array
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

#endif //CCHESS_TEAM_H
