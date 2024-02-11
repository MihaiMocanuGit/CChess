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

    Team whiteTeam;
    Team blackTeam;

    // the actual pieces are owned by (they live inside) teams, we actually hold only
    // a reference to the pieces.
    Piece *table[TABLE_Y][TABLE_X];

} Table;



void table_init(Table *table);
Table table_construct();


#endif //CCHESS_TABLE_H
