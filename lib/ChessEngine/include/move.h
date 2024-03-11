#ifndef CCHESS_MOVE_H
#define CCHESS_MOVE_H

#include <stddef.h>
#include <stdbool.h>
#include "piece.h"

typedef enum
{
    ADVANCE,
    CAPTURE,
    CASTLE,
    EN_PASSANT,
    ADVANCE_TO_PROMOTE,
    CAPTURE_TO_PROMOTE,

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
    PieceType_e promoteTo;

} Move;
typedef struct
{

    Move moves[31];


    int noMoves;
    int startX;
    int startY;
}LegalMoves;

void pieceMove_initMove(Move *move, int x, int y);

Move pieceMove_constructAdvance(int x, int y);

void pieceMove_initCapture(Move *move, int x, int y, PieceType_e movePartnerType);

Move pieceMove_constructCapture(int x, int y, PieceType_e movePartnerType);

void pieceMove_initEnPassant(Move *move, int pawnToX, int pawnToY, int enemyPawnFromX, int enemyPawnFromY);

Move pieceMove_constructEnPassant(int pawnToX, int pawnToY, int enemyPawnFromX, int enemyPawnFromY);

void pieceMove_initCaptureToPromote(Move *move, int x, int y, PieceType_e movePartner, PieceType_e promoteTo);
Move pieceMove_constructCaptureToPromote(int x, int y, PieceType_e movePartner, PieceType_e promoteTo);
void pieceMove_initAdvanceToPromote(Move *move, int x, int y, PieceType_e movePartner, PieceType_e promoteTo);
Move pieceMove_constructAdvanceToPromote(int x, int y, PieceType_e movePartner, PieceType_e promoteTo);

void pieceMove_initCastle(Move *move, int kingToX, int kingToY, int rookFromX, int rookFromY);

Move pieceMove_constructCastle(int kingToX, int kingToY, int rookFromX, int rookFromY);

void legalMoves_initEmpty(LegalMoves *legalMoves);

void legalMoves_initEmptyWithStart(LegalMoves *legalMoves, int startX, int startY);

LegalMoves legalMoves_constructEmpty();

LegalMoves legalMoves_constructEmptyWithStart(int startX, int startY);


#endif //CCHESS_MOVE_H
