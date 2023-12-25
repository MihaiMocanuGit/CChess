#ifndef CCHESS_TABLE_H
#define CCHESS_TABLE_H

#include "piece.h"
#include "team.h"


#define TABLE_X 8
#define TABLE_Y 8

typedef enum
{
    PLAYING,
    WHITE_CHECKED,
    BLACK_CHECKED,
    WHITE_WON,
    BLACK_WON,
    TIE
} TableState_e;

typedef struct
{
    TableState_e state;
    Piece *table[TABLE_Y][TABLE_X];

    Team whiteTeam;


    Team blackTeam;
    Piece takenByBlack[TEAM_INITIAL_NO_PIECES];
    unsigned noTakenByBlack;

} Table;

#endif //CCHESS_TABLE_H
