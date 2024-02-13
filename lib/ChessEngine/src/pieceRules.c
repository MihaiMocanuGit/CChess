#include "pieceRules.h"

#include <stddef.h>

#include "kingMoveValidator.h"

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
        // this bounds validation is needed only for knight's and king's move
        // if performance issue appear, we can remove this and manually validate the coords of the moves for king
        // and knight before calling this function. (Or manually checking the moves instead of using this function)
        // if  (!pieceRules_areCoordsValid(newX, newY))
        //    break;

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
bool m_canApplyEnPassant(const Piece* pieceToBeCaptured, const Piece *subjectPawn)
{
    if (pieceToBeCaptured != NULL && pieceToBeCaptured->type == PAWN && pieceToBeCaptured->team != subjectPawn->team &&
        pieceToBeCaptured->movesMade == 1)
    {
        if ((pieceToBeCaptured->team == WHITE && pieceToBeCaptured->y == 4) ||
            (pieceToBeCaptured->team == BLACK && pieceToBeCaptured->y == 3))
            return true;
    }
    return false;
}
void pieceRules_findMovesPawn(const Piece *pawn, const Table *table, bool kingInCheck, LegalMoves *outMoves)
{
    *outMoves = legalMoves_constructEmpty();
    const int startX = pawn->x;
    const int startY = pawn->y;
    //we will make the assumption that white is always at the bottom of the table.
    const int dir = (pawn->team == WHITE)? -1 : 1;

    //promotion
    if (startY == 0 || startY == TABLE_HEIGHT)
    {
        m_pieceMoves_addMove(outMoves, pieceMove_constructMove(startX, startY, PROMOTE, NULL));
        return; //we cannot do anything more. Excluding this case now means that we do not have to validate
                //the y coord in the other cases!!!!!!!
    }

    //one tile move
    int newX = startX;
    int newY = startY + dir;
    if (table->table[newY][newX] == NULL)
        m_pieceMoves_addMove(outMoves, pieceMove_constructMove(newX, newY, MOVE, NULL));

    // only for the first move the pawn can move 2 tiles
    newX = startX;
    newY = startY + dir * 2;
    if (pawn->movesMade == 0 && table->table[startY + dir][newX] == NULL && table->table[newY][newX] == NULL)
        //by checking for pawn->movesMade == 0, we are sure that newY is in bounds
        m_pieceMoves_addMove(outMoves, pieceMove_constructMove(newX, newY, MOVE, NULL));



    if (startX - 1 >= 0)
    {
        //generic capture
        Piece *leftPiece = table->table[startY + dir][startX - 1];
        if (leftPiece != NULL && leftPiece->team != pawn->team)
            m_pieceMoves_addMove(outMoves, pieceMove_constructMove(startX - 1, startY + dir, CAPTURE, leftPiece));

        //en'passant when the piece to be captured is to the left
        leftPiece = table->table[startY][startX - 1];
        if (m_canApplyEnPassant(leftPiece, pawn))
            m_pieceMoves_addMove(outMoves, pieceMove_constructMove(startX - 1, startY + dir, CAPTURE, leftPiece));

    }


    if (startX + 1 < TABLE_WIDTH)
    {
        //generic capture
        Piece *rightPiece = table->table[startY + dir][startX + 1];
        if (rightPiece != NULL && rightPiece->team != pawn->team)
            m_pieceMoves_addMove(outMoves, pieceMove_constructMove(startX + 1, startY + dir, CAPTURE, rightPiece));

        //en'passant when the piece to be captured is to the right
        rightPiece = table->table[startY][startX + 1];
        if (m_canApplyEnPassant(rightPiece, pawn))
                m_pieceMoves_addMove(outMoves, pieceMove_constructMove(startX + 1, startY + dir, CAPTURE, rightPiece));

    }






}
void pieceRules_findMovesRook(const Piece *rook, const Table *table, bool kingInCheck, LegalMoves *outMoves)
{
    *outMoves = legalMoves_constructEmpty();
    const int startX = rook->x;
    const int startY = rook->y;

    const int leftSteps = startX;
    m_pieceRules_moveGenerator(table, rook->team, startX, startY, -1, 0, leftSteps, outMoves);


    const int rightSteps = TABLE_WIDTH - startX - 1;
    m_pieceRules_moveGenerator(table, rook->team, startX, startY, +1, 0, rightSteps, outMoves);


    const int upSteps = startY;
    m_pieceRules_moveGenerator(table, rook->team, startX, startY, 0, -1, upSteps, outMoves);

    const int downSteps = TABLE_HEIGHT - startY - 1;
    m_pieceRules_moveGenerator(table, rook->team, startX, startY, 0, +1, downSteps, outMoves);
}


void pieceRules_findMovesBishop(const Piece *bishop, const Table *table, bool kingInCheck, LegalMoves *outMoves)
{
    //TODO: Special case for when the king is in check
    // perhaps move the logic defined here to a private function. And use the private function when
    // the piece is not in check. If it's in check it would be more complicated as we need to compute
    // the moves a piece can make even if the king is in check as we need to find which tiles are under
    // attack by the piece that is checking the king.

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

void pieceRules_findMovesKnight(const Piece *knight, const Table *table, bool kingInCheck, LegalMoves *outMoves)
{
    *outMoves = legalMoves_constructEmpty();
    const int startX = knight->x;
    const int startY = knight->y;

    const int dirX[] = {-2, -2, -1, -1, +1, +1, +2, +2};
    const int dirY[] = {-1, +1, -2, +2, -2, +2, -1, +1};
    for (int i = 0; i < 7; ++i)
    {
        const int x = dirX[i] + startX;
        const int y = dirY[i] + startY;

        if (pieceRules_areCoordsValid(x, y))
        {
            if (table->table[y][x] == NULL)
                m_pieceMoves_addMove(outMoves, pieceMove_constructMove(x, y, MOVE, NULL));
            else if (table->table[y][x]->team != knight->team)
                m_pieceMoves_addMove(outMoves, pieceMove_constructMove(x, y, CAPTURE, table->table[y][x]));
        }
    }
}

void pieceRules_findMovesQueen(const Piece *queen, const Table *table, bool kingInCheck, LegalMoves *outMoves)
{
    *outMoves = legalMoves_constructEmpty();
    const int startX = queen->x;
    const int startY = queen->y;

    //same as bishop rules
    const int upperLeftSteps = (startX < startY) ? startX : startY;
    m_pieceRules_moveGenerator(table, queen->team, startX, startY, -1, -1, upperLeftSteps, outMoves);


    const int upperRightSteps = (TABLE_WIDTH - startX - 1 < startY) ? TABLE_WIDTH - startX - 1 : startY;
    m_pieceRules_moveGenerator(table, queen->team, startX, startY, +1, -1, upperRightSteps, outMoves);


    const int lowerLeftSteps = (startX < TABLE_HEIGHT - startY - 1) ? startX : TABLE_HEIGHT - startY - 1;
    m_pieceRules_moveGenerator(table, queen->team, startX, startY, -1, +1, lowerLeftSteps, outMoves);

    const int lowerRightSteps = (TABLE_WIDTH - startX - 1 < TABLE_HEIGHT - startY - 1)  ? TABLE_WIDTH - startX - 1
                                                                                        : TABLE_HEIGHT - startY - 1;
    m_pieceRules_moveGenerator(table, queen->team, startX, startY, +1, +1, lowerRightSteps, outMoves);


    //same as rook moves
    const int leftSteps = startX;
    m_pieceRules_moveGenerator(table, queen->team, startX, startY, -1, 0, leftSteps, outMoves);


    const int rightSteps = TABLE_WIDTH - startX - 1;
    m_pieceRules_moveGenerator(table, queen->team, startX, startY, +1, 0, rightSteps, outMoves);


    const int upSteps = startY;
    m_pieceRules_moveGenerator(table, queen->team, startX, startY, 0, -1, upSteps, outMoves);

    const int downSteps = TABLE_HEIGHT - startY - 1;
    m_pieceRules_moveGenerator(table, queen->team, startX, startY, 0, +1, downSteps, outMoves);
}

void pieceRules_findMovesKing(const Piece *king, const Table *table, bool kingInCheck, LegalMoves *outMoves)
{
    *outMoves = legalMoves_constructEmpty();
    const int startX = king->x;
    const int startY = king->y;

    const int dirX[8] = {-1,  0,  1, -1, 1, -1,  0,  1};
    const int dirY[8] = {-1, -1, -1,  0, 0,  1,  1,  1};
    for (int i = 0; i < 8; ++i)
    {
        const int x = dirX[i] + startX;
        const int y = dirY[i] + startY;

        if (pieceRules_areCoordsValid(x, y) && (table->table[y][x] == NULL || table->table[y][x]->team != king->team) &&
                pieceRules_canKingBeInSpot(king->team, table, x, y))
        {
            if (table->table[y][x] == NULL)
                m_pieceMoves_addMove(outMoves, pieceMove_constructMove(x, y, MOVE, NULL));
            else
                m_pieceMoves_addMove(outMoves, pieceMove_constructMove(x, y, CAPTURE, table->table[y][x]));
        }
    }
}