#ifndef CCHESS_TABLE_H
#define CCHESS_TABLE_H

#include "piece.h"
#include "team.h"


#define TABLE_X 8
#define TABLE_Y 8

typedef enum
{
    PLAYING,
    WHITE_IN_CHECK,
    BLACK_IN_CHECK,
    WHITE_WON,
    BLACK_WON,
    TIE
} TableState_e;

typedef enum
{
    WHITE_TO_MOVE,
    BLACK_TO_MOVE
}TableTurn_e;

typedef struct
{
    TableState_e state;
    TableTurn_e turn;

    // the actual pieces are owned by (they live inside) teams, we actually hold only
    // a reference to the pieces.
    Piece *table[TABLE_Y][TABLE_X];

    Team whiteTeam;
    Team blackTeam;

} Table;

//TODO: Should the responsibility of placing a team on the table
// be the role of a team or a table. It would perhaps make more sense to move
// these 4 functions to the table file.
void table_initTeamDefaultTop(Team *team, PieceTeam_e color);
Team table_constructTeamDefaultTop(PieceTeam_e color);

void table_initTeamDefaultBottom(Team *team, PieceTeam_e color);
Team table_constructTeamDefaultBottom(PieceTeam_e color);


#endif //CCHESS_TABLE_H
