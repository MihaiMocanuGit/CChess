#include "pieceRules.h"

#include <stddef.h>

void pieceMove_initMove(Move *move, int x, int y, MoveType_e moveType, Piece *movePartner)
{
    move->x = x;
    move->y = y;
    move->type = moveType;
    move->movePartner = movePartner;
}

Move pieceMove_constructMove(int x, int y, MoveType_e moveType, Piece *movePartner)
{
    Move move;
    pieceMove_initMove(&move, x, y, moveType, movePartner);
    return move;
}


void pieceMoves_initEmptyLegalMoves(LegalMoves *legalMoves)
{
    legalMoves->noMoves = 0;
    legalMoves->subjectPiece = NULL;
}

LegalMoves legalMoves_constructEmpty()
{
    LegalMoves moves;
    pieceMoves_initEmptyLegalMoves(&moves);
    return moves;
}

void m_pieceMoves_addMove(LegalMoves *moves,  Move move)
{
    moves->moves[moves->noMoves++] = move;
}

void m_pieceRules_moveGenerator(const Table *table, PieceTeam_e subjectTeam, int startX, int startY,
                                int stepX, int stepY, int noSteps, LegalMoves *outMoves)
{
    for (int i = 1; i <= noSteps; ++i)
    {
        const int newX = startX + i * stepX;
        const int newY = startY + i * stepY;
        if (table->table[newY][newX] == NULL)
        {
            m_pieceMoves_addMove(outMoves, pieceMove_constructMove(newX, newY, MOVE, NULL));
        }
        else
        {
            if (subjectTeam != table->table[newY][newX]->team)
                m_pieceMoves_addMove(outMoves, pieceMove_constructMove(newX, newY, CAPTURE, table->table[newY][newX]));
            break;
        }
    }
}
void pieceRules_findMovesBishop(const Piece *bishop, const Table *table, bool kingInCheck, LegalMoves *outMoves)
{
    //TODO: Special case for when the king is in check
    // perhaps move the logic defined here to a private function
    // we would need to know these moves when checking which piece is
    // attacking the king
    *outMoves = legalMoves_constructEmpty();
    const int startX = bishop->x;
    const int startY = bishop->y;

    const int upperLeftSteps = (startX < startY) ? startX : startY;
    m_pieceRules_moveGenerator(table, bishop->team, startX, startY, -1, -1, upperLeftSteps, outMoves);


    const int upperRightSteps = (TABLE_WIDTH - startX - 1 < startY) ? TABLE_WIDTH - startX - 1 : startY;
    m_pieceRules_moveGenerator(table, bishop->team, startX, startY, +1, -1, upperRightSteps, outMoves);


    const int lowerLeftSteps = (startX < TABLE_HEIGHT - startY - 1) ? startX : TABLE_HEIGHT - startY - 1;
    m_pieceRules_moveGenerator(table, bishop->team, startX, startY, -1, +1, lowerLeftSteps, outMoves);

    const int lowerRightSteps = (TABLE_WIDTH - startX - 1 < TABLE_HEIGHT - startY - 1)  ? TABLE_WIDTH - startX - 1
                                                                                        : TABLE_HEIGHT - startY - 1;
    m_pieceRules_moveGenerator(table, bishop->team, startX, startY, +1, +1, lowerRightSteps, outMoves);
}


