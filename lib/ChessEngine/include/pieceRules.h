#ifndef CCHESS_PIECE_RULES_H
#define CCHESS_PIECE_RULES_H

#include "piece.h"
#include "table.h"

typedef enum
{
    MOVE,
    CAPTURE,
    CASTLE,
    PROMOTE,
    EN_PASSANT

}MoveType_e;
typedef struct
{
    struct
    {
        int x,y;
        MoveType_e type;

        // To make a move we might need information about another piece.
        // for example:
        //  1) With respect to the King, if we decide to castle we must
        //  know where the rook is
        //  2) With respect to the Pawn, if we use en'passant, it might
        //  help us to know what pawn was attacked.
        //  3) Maybe it would be better for every capture to remember which piece
        //  we are attacking. It is redundant information as we can extract it from
        //  the move position, but it is easier to already have the information
        //  at hand.
        Piece *movePartner;


        //there are 64 spots on the table, so we can't have
        // more than 64 valid spots for a piece to move in
    } moves[TABLE_HEIGHT * TABLE_WIDTH];


    int noMoves;
    Piece *subjectPiece;
}LegalMoves;
void pieceMoves_initEmptyLegalMoves(LegalMoves *legalMoves);
LegalMoves pieceMoves_constructEmptyLegalMoves();

void pieceRules_findMovesPawn(const Piece *pawn, const Table *table, bool kingInCheck, LegalMoves *outMoves);
void pieceRules_findMovesBishop(const Piece *bishop, const Table *table, bool kingInCheck, LegalMoves *outMoves);
void pieceRules_findMovesRook(const Piece *rook, const Table *table, bool kingInCheck, LegalMoves *outMoves);
void pieceRules_findMovesKnight(const Piece *knight, const Table *table, bool kingInCheck, LegalMoves *outMoves);
void pieceRules_findMovesQueen(const Piece *queen, const Table *table, bool kingInCheck, LegalMoves *outMoves);
void pieceRules_findMovesKing(const Piece *king, const Table *table, bool kingInCheck, LegalMoves *outMoves);
#endif //CCHESS_PIECE_RULES_H
