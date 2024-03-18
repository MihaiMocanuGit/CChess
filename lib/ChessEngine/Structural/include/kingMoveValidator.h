#ifndef CCHESS_KING_MOVE_VALIDATOR_H
#define CCHESS_KING_MOVE_VALIDATOR_H


#include <stdbool.h>
#include "piece.h"
#include "table.h"

bool pieceRules_isXValid(int x);

bool pieceRules_isYValid(int y);

bool pieceRules_areCoordsValid(int x, int y);

Piece *pieceRules_findFirstPieceInLine(const Table *table, int startX, int startY, int stepX, int stepY, int noSteps);

bool pieceRules_canKingBeInSpot(PieceTeam_e teamKing, const Table *table, int spotX, int spotY);

#endif //CCHESS_KING_MOVE_VALIDATOR_H
