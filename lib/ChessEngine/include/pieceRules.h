#ifndef CCHESS_PIECE_RULES_H
#define CCHESS_PIECE_RULES_H

#include "piece.h"
#include "table.h"

void pieceRules_findMovesPawn(const Piece *pawn, const Table *table, bool kingInCheck, LegalMoves *outMoves);
void pieceRules_findMovesBishop(const Piece *bishop, const Table *table, bool kingInCheck, LegalMoves *outMoves);
void pieceRules_findMovesRook(const Piece *rook, const Table *table, bool kingInCheck, LegalMoves *outMoves);
void pieceRules_findMovesKnight(const Piece *knight, const Table *table, bool kingInCheck, LegalMoves *outMoves);
void pieceRules_findMovesQueen(const Piece *queen, const Table *table, bool kingInCheck, LegalMoves *outMoves);
void pieceRules_findMovesKing(const Piece *king, const Table *table, bool kingInCheck, LegalMoves *outMoves);
#endif //CCHESS_PIECE_RULES_H
