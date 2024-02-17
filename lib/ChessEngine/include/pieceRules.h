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
    int movePartnerX;
    int movePartnerY;
    PieceType_e movePartnerType;

} Move;
void pieceMove_initMove(Move *move, int x, int y);
Move pieceMove_constructMove(int x, int y);

void pieceMove_initCapture(Move *move, int x, int y, PieceType_e movePartnerType);
Move pieceMove_constructCapture(int x, int y, PieceType_e movePartnerType);

void pieceMove_initEnPassant(Move *move, int pawnToX, int pawnToY, int enemyPawnFromX, int enemyPawnFromY);
Move pieceMove_constructEnPassant(int pawnToX, int pawnToY, int enemyPawnFromX, int enemyPawnFromY);

void pieceMove_initPromote(Move *move, int x, int y, PieceType_e promoteTo);
Move pieceMove_constructPromote(int x, int y, PieceType_e promoteTo);

void pieceMove_initCastle(Move *move, int kingToX, int kingToY, int rookFromX, int rookFromY);
Move pieceMove_constructCastle(int kingToX, int kingToY, int rookFromX, int rookFromY);

typedef struct
{

    Move moves[31];


    int noMoves;
    int startX;
    int startY;
}LegalMoves;
void legalMoves_initEmptyWithStart(LegalMoves *legalMoves, int startX, int startY);
void legalMoves_initEmpty(LegalMoves *legalMoves);
LegalMoves legalMoves_constructEmptyWithStart(int startX, int startY);
LegalMoves legalMoves_constructEmpty();

void pieceRules_findMovesPawn(const Piece *pawn, const Table *table, bool kingInCheck, LegalMoves *outMoves);
void pieceRules_findMovesBishop(const Piece *bishop, const Table *table, bool kingInCheck, LegalMoves *outMoves);
void pieceRules_findMovesRook(const Piece *rook, const Table *table, bool kingInCheck, LegalMoves *outMoves);
void pieceRules_findMovesKnight(const Piece *knight, const Table *table, bool kingInCheck, LegalMoves *outMoves);
void pieceRules_findMovesQueen(const Piece *queen, const Table *table, bool kingInCheck, LegalMoves *outMoves);
void pieceRules_findMovesKing(const Piece *king, const Table *table, bool kingInCheck, LegalMoves *outMoves);
#endif //CCHESS_PIECE_RULES_H
