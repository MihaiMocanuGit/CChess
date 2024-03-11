#include "move.h"

void pieceMove_initMove(Move *move, int x, int y)
{
    move->x = x;
    move->y = y;
    move->type = ADVANCE;
    move->movePartnerX = -1;
    move->movePartnerY = -1;
}

Move pieceMove_constructAdvance(int x, int y)
{
    Move move = {.x = x, .y = y, .type = ADVANCE};
    return move;
}

void pieceMove_initCapture(Move *move, int x, int y, PieceType_e movePartnerType)
{
    move->x = x;
    move->y = y;
    move->type = CAPTURE;
    move->movePartnerX = x;
    move->movePartnerY = y;
    move->movePartnerType = movePartnerType;
}

Move pieceMove_constructCapture(int x, int y, PieceType_e movePartnerType)
{
    Move move = {.x = x, .y = y, .type = CAPTURE,
            .movePartnerX = x, .movePartnerY = y, .movePartnerType = movePartnerType};
    return move;
}

void pieceMove_initEnPassant(Move *move, int pawnToX, int pawnToY, int enemyPawnFromX, int enemyPawnFromY)
{
    move->x = pawnToX;
    move->y = pawnToY;
    move->type = EN_PASSANT;
    move->movePartnerX = enemyPawnFromX;
    move->movePartnerY = enemyPawnFromY;
    move->movePartnerType = PAWN;
}

Move pieceMove_constructEnPassant(int pawnToX, int pawnToY, int enemyPawnFromX, int enemyPawnFromY)
{
    Move move = {.x = pawnToX, .y = pawnToY, .type = EN_PASSANT,
            .movePartnerX = enemyPawnFromX, .movePartnerY = enemyPawnFromY, .movePartnerType = PAWN};
    return move;
}


void pieceMove_initCaptureToPromote(Move *move, int x, int y, PieceType_e promoteTo)
{
    move->x = x;
    move->y = y;
    move->type = CAPTURE_TO_PROMOTE;
    move->movePartnerX = x;
    move->movePartnerY = y;
    move->movePartnerType = promoteTo;
}


Move pieceMove_constructCaptureToPromote(int x, int y, PieceType_e promoteTo)
{
    Move move = {.x = x, .y = y, .type = CAPTURE_TO_PROMOTE ,
            .movePartnerX = x, .movePartnerY = y, .movePartnerType = promoteTo};
    return move;
}

void pieceMove_initAdvanceToPromote(Move *move, int x, int y, PieceType_e promoteTo)
{
    move->x = x;
    move->y = y;
    move->type = ADVANCE_TO_PROMOTE;
    move->movePartnerX = x;
    move->movePartnerY = y;
    move->movePartnerType = promoteTo;
}


Move pieceMove_constructAdvanceToPromote(int x, int y, PieceType_e promoteTo)
{
    Move move = {.x = x, .y = y, .type = ADVANCE_TO_PROMOTE ,
            .movePartnerX = x, .movePartnerY = y, .movePartnerType = promoteTo};
    return move;
}

void pieceMove_initCastle(Move *move, int kingToX, int kingToY, int rookFromX, int rookFromY)
{
    move->x = kingToX;
    move->y = kingToY;
    move->type = CASTLE;
    move->movePartnerX = rookFromX;
    move->movePartnerY = rookFromY;
    move->movePartnerType = ROOK;
}

Move pieceMove_constructCastle(int kingToX, int kingToY, int rookFromX, int rookFromY)
{
    Move move = {.x = kingToX, .y = kingToY, .type = CASTLE,
            .movePartnerX = rookFromX, .movePartnerY = rookFromY, .movePartnerType = ROOK};
    return move;
}

void legalMoves_initEmpty(LegalMoves *legalMoves)
{
    legalMoves->noMoves = 0;
}

void legalMoves_initEmptyWithStart(LegalMoves *legalMoves, int startX, int startY)
{
    legalMoves_initEmpty(legalMoves);
    legalMoves->startX = startX;
    legalMoves->startY = startY;
}

LegalMoves legalMoves_constructEmpty()
{
    LegalMoves moves;
    legalMoves_initEmpty(&moves);
    return moves;
}

LegalMoves legalMoves_constructEmptyWithStart(int startX, int startY)
{
    LegalMoves moves;
    legalMoves_initEmptyWithStart(&moves, startX, startY);
    return moves;
}