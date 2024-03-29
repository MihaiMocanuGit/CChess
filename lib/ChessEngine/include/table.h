#ifndef CCHESS_TABLE_H
#define CCHESS_TABLE_H

#include "piece.h"
#include "team.h"
#include "move.h"


#define TABLE_WIDTH 8
#define TABLE_HEIGHT 8

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
    Piece *table[TABLE_HEIGHT][TABLE_WIDTH];

} Table;


void table_deepCopy(Table *newTable, const Table *original);
void table_init(Table *table);
void table_capturePiece(Table *table, const LegalMoves *moves, int moveIndex);
void table_advancePiece(Table *table, const LegalMoves *moves, int moveIndex);
void table_castleKing(Table *table, const LegalMoves *moves, int moveIndex);
void table_promotePawn(Table *table, const LegalMoves *moves, int moveIndex);

void table_makeMove(Table *table, const LegalMoves *moves, int moveIndex);



#endif //CCHESS_TABLE_H
