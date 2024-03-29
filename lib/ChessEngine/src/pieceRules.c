#include "pieceRules.h"

#include <stddef.h>

#include "kingMoveValidator.h"
#include "move.h"

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
            m_pieceMoves_addMove(outMoves, pieceMove_constructAdvance(newX, newY));
        }
        else
        {
            if (subjectTeam != table->table[newY][newX]->team)
                m_pieceMoves_addMove(outMoves, pieceMove_constructCapture(newX, newY, table->table[newY][newX]->type));
            break;
        }
    }
}


void m_tempMove(Table *tempTable, const LegalMoves *moves, int moveIndex)
{
    int startX = moves->startX;
    int startY = moves->startY;

    int newX = moves->moves[moveIndex].x;
    int newY = moves->moves[moveIndex].y;

    //move the reference to the subjectPiece from the old spot in tempTable to the new one
    tempTable->table[newY][newX] = tempTable->table[startY][startX];
    tempTable->table[startY][startX] = NULL;

    //change the position known by the piece.
    tempTable->table[newY][newX]->x = newX;
    tempTable->table[newY][newX]->y = newY;
}
void m_reverseTempMove(Table *tempTable, const LegalMoves *moves, int moveIndex)
{
    int startX = moves->startX;
    int startY = moves->startY;

    int newX = moves->moves[moveIndex].x;
    int newY = moves->moves[moveIndex].y;

    //move the reference to the subjectPiece from the old spot in tempTable to the new one
    tempTable->table[startY][startX] = tempTable->table[newY][newX];
    tempTable->table[newY][newX] = NULL;

    //change the position known by the piece.
    tempTable->table[startY][startX]->x = startX;
    tempTable->table[startY][startX]->y = startX;
}
void m_tempCapture(Table *tempTable, const LegalMoves *moves, int moveIndex, Team **outCapturedPieceTeam, int* outCapturedPieceIndex)
{

    const Move *move = &moves->moves[moveIndex];

    const Piece *capturedPiece = tempTable->table[move->movePartnerY][move->movePartnerX];
    *outCapturedPieceTeam = &tempTable->whiteTeam; //suppressing pointer may be null warning
    switch (capturedPiece->team)
    {
        case WHITE:
            *outCapturedPieceTeam = &tempTable->whiteTeam;
            break;
        case BLACK:
            *outCapturedPieceTeam = &tempTable->blackTeam;
            break;
    }

    for (int i = 0; i < (*outCapturedPieceTeam)->noPieces; ++i)
    {
        if ((*outCapturedPieceTeam)->pieces[i].x == capturedPiece->x &&
            (*outCapturedPieceTeam)->pieces[i].y == capturedPiece->y)
        {
            //removing the reference owned by tempTable
            tempTable->table[move->movePartnerY][move->movePartnerX] = NULL;
            *outCapturedPieceIndex = i;

            break;
        }
    }
}
void m_reverseTempCapture(Table *table, const LegalMoves *moves, int moveIndex, Team *capturedPieceTeam, int capturedPieceIndex)
{

    Piece *capturedPiece = &capturedPieceTeam->pieces[capturedPieceIndex];
    table->table[capturedPiece->y][capturedPiece->x] = capturedPiece;
}
void m_removeBadMovesInCheck(const Piece *piece, const Table *table, LegalMoves *outMoves)
{
    //TODO: If performance issue arise, we could check the moves on the original table, without
    // creating temporal tables and copying all the information from the original.
    // It should work just fine - possible famous last words
    Table tempTable;
    table_deepCopy(&tempTable, table);
    for (int i = 0; i < outMoves->noMoves; ++i)
    {
        const Move *move = &outMoves->moves[i];
        const Piece *subject = tempTable.table[outMoves->startY][outMoves->startX];
        const Team *subjectTeam;
        switch (subject->team)
        {
            case WHITE:
                subjectTeam = &table->whiteTeam;
                break;
            case BLACK:
                subjectTeam = &table->blackTeam;
                break;
        }

        //make temporal move
        int capturedPieceIndex;
        Team *capturedPieceTeam;
        switch (move->type)
        {
            case EN_PASSANT:
            case CAPTURE_TO_PROMOTE:
            case CAPTURE:
                m_tempCapture(&tempTable, outMoves, i, &capturedPieceTeam, &capturedPieceIndex);
            case ADVANCE_TO_PROMOTE:
            case ADVANCE:
                m_tempMove(&tempTable, outMoves, i);
                break;
            case CASTLE:
                //when generating all castling moves, we are already making sure that it won't result in a check
                break;
        }



        //mark the move that it must be removed if it results in a check
        int kingX = team_king_const(subjectTeam)->x;
        int kingY = team_king_const(subjectTeam)->y;
        bool badMove = !pieceRules_canKingBeInSpot(subjectTeam->teamColor, &tempTable, kingX, kingY);

        //reverse the move so that the table is back to its original state
        switch (move->type)
        {
            case EN_PASSANT:
            case CAPTURE_TO_PROMOTE:
            case CAPTURE:
                m_reverseTempMove(&tempTable, outMoves, i);
                m_reverseTempCapture(&tempTable, outMoves, i, capturedPieceTeam, capturedPieceIndex);
                break;
            case ADVANCE_TO_PROMOTE:
            case ADVANCE:
                m_reverseTempMove(&tempTable, outMoves, i);
                break;
            case CASTLE:
                break;
        }

        //remove the bad move
        if (badMove)
        {
            outMoves->moves[i] = outMoves->moves[--(outMoves->noMoves)];
            i--;
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

void m_addPawnMoveWithPromoteInjection(LegalMoves *outMoves, Move move)
{
    const int startX = outMoves->startX;
    const int startY = outMoves->startY;

    const int moveX = move.x;
    const int moveY = move.y;

    if (moveY == 0 || moveY == TABLE_HEIGHT - 1)
    {
        MoveType_e newType = ADVANCE_TO_PROMOTE;
        if(move.type == CAPTURE)
            newType = CAPTURE_TO_PROMOTE;


        move.type = newType;
        move.promoteTo = BISHOP;
        m_pieceMoves_addMove(outMoves, move);

        move.promoteTo = ROOK;
        m_pieceMoves_addMove(outMoves, move);

        move.promoteTo = KNIGHT;
        m_pieceMoves_addMove(outMoves, move);

        move.promoteTo = QUEEN;
        m_pieceMoves_addMove(outMoves, move);
    }
    else
    {
        m_pieceMoves_addMove(outMoves, move);
    }
}
void pieceRules_findMovesPawn(const Piece *pawn, const Table *table, LegalMoves *outMoves)
{

    const int startX = pawn->x;
    const int startY = pawn->y;
    *outMoves = legalMoves_constructEmptyWithStart(startX, startY);

    //we will make the assumption that white is always at the bottom of the table.
    const int dir = (pawn->team == WHITE)? -1 : 1;

    if (true || startY == 0 || startY == TABLE_HEIGHT - 1)
    {
        //this can never happen as a pawn will always be promoted when reaching the end tile
       /* m_pieceMoves_addMove(outMoves, pieceMove_constructPromote(startX, startY, BISHOP));
        m_pieceMoves_addMove(outMoves, pieceMove_constructPromote(startX, startY, ROOK));
        m_pieceMoves_addMove(outMoves, pieceMove_constructPromote(startX, startY, KNIGHT));
        m_pieceMoves_addMove(outMoves, pieceMove_constructPromote(startX, startY, QUEEN));
        return; //we cannot do anything more. Excluding this case now means that we do not have to validate
                //the y coord in the other cases!!!!!!!*/
    }

    //one tile move
    int newX = startX;
    int newY = startY + dir;
    if (table->table[newY][newX] == NULL)
        m_addPawnMoveWithPromoteInjection(outMoves, pieceMove_constructAdvance(newX, newY));

    // only for the first move the pawn can move 2 tiles
    newX = startX;
    newY = startY + dir * 2;
    if (pawn->movesMade == 0 && table->table[startY + dir][newX] == NULL && table->table[newY][newX] == NULL)
        //by checking for pawn->movesMade == 0, we are sure that newY is in bounds
        m_pieceMoves_addMove(outMoves, pieceMove_constructAdvance(newX, newY));



    if (startX - 1 >= 0)
    {
        //generic capture
        Piece *leftPiece = table->table[startY + dir][startX - 1];
        if (leftPiece != NULL && leftPiece->team != pawn->team)
            m_addPawnMoveWithPromoteInjection(outMoves,
                                              pieceMove_constructCapture(startX - 1, startY + dir, leftPiece->type));

        //en'passant when the piece to be captured is to the left
        leftPiece = table->table[startY][startX - 1];
        if (m_canApplyEnPassant(leftPiece, pawn))
            m_pieceMoves_addMove(outMoves, pieceMove_constructEnPassant(startX - 1, startY + dir, leftPiece->x, leftPiece->y));

    }


    if (startX + 1 < TABLE_WIDTH)
    {
        //generic capture
        Piece *rightPiece = table->table[startY + dir][startX + 1];
        if (rightPiece != NULL && rightPiece->team != pawn->team)
            m_addPawnMoveWithPromoteInjection(outMoves,
                                              pieceMove_constructCapture(startX + 1, startY + dir, rightPiece->type));

        //en'passant when the piece to be captured is to the right
        rightPiece = table->table[startY][startX + 1];
        if (m_canApplyEnPassant(rightPiece, pawn))
                m_pieceMoves_addMove(outMoves, pieceMove_constructEnPassant(startX + 1, startY + dir, rightPiece->x, rightPiece->y));

    }

    m_removeBadMovesInCheck(pawn, table, outMoves);
}
void pieceRules_findMovesRook(const Piece *rook, const Table *table, LegalMoves *outMoves)
{

    const int startX = rook->x;
    const int startY = rook->y;
    *outMoves = legalMoves_constructEmptyWithStart(startX, startY);

    const int leftSteps = startX;
    m_pieceRules_moveGenerator(table, rook->team, startX, startY, -1, 0, leftSteps, outMoves);


    const int rightSteps = TABLE_WIDTH - startX - 1;
    m_pieceRules_moveGenerator(table, rook->team, startX, startY, +1, 0, rightSteps, outMoves);


    const int upSteps = startY;
    m_pieceRules_moveGenerator(table, rook->team, startX, startY, 0, -1, upSteps, outMoves);

    const int downSteps = TABLE_HEIGHT - startY - 1;
    m_pieceRules_moveGenerator(table, rook->team, startX, startY, 0, +1, downSteps, outMoves);

    m_removeBadMovesInCheck(rook, table, outMoves);
}


void pieceRules_findMovesBishop(const Piece *bishop, const Table *table, LegalMoves *outMoves)
{
    //TODO: Special case for when the king is in check
    // perhaps move the logic defined here to a private function. And use the private function when
    // the piece is not in check. If it's in check it would be more complicated as we need to compute
    // the moves a piece can make even if the king is in check as we need to find which tiles are under
    // attack by the piece that is checking the king.


    const int startX = bishop->x;
    const int startY = bishop->y;
    *outMoves = legalMoves_constructEmptyWithStart(startX, startY);

    const int upperLeftSteps = (startX < startY) ? startX : startY;
    m_pieceRules_moveGenerator(table, bishop->team, startX, startY, -1, -1, upperLeftSteps, outMoves);


    const int upperRightSteps = (TABLE_WIDTH - startX - 1 < startY) ? TABLE_WIDTH - startX - 1 : startY;
    m_pieceRules_moveGenerator(table, bishop->team, startX, startY, +1, -1, upperRightSteps, outMoves);


    const int lowerLeftSteps = (startX < TABLE_HEIGHT - startY - 1) ? startX : TABLE_HEIGHT - startY - 1;
    m_pieceRules_moveGenerator(table, bishop->team, startX, startY, -1, +1, lowerLeftSteps, outMoves);

    const int lowerRightSteps = (TABLE_WIDTH - startX - 1 < TABLE_HEIGHT - startY - 1)  ? TABLE_WIDTH - startX - 1
                                                                                        : TABLE_HEIGHT - startY - 1;
    m_pieceRules_moveGenerator(table, bishop->team, startX, startY, +1, +1, lowerRightSteps, outMoves);

    m_removeBadMovesInCheck(bishop, table, outMoves);
}

void pieceRules_findMovesKnight(const Piece *knight, const Table *table, LegalMoves *outMoves)
{

    const int startX = knight->x;
    const int startY = knight->y;
    *outMoves = legalMoves_constructEmptyWithStart(startX, startY);

    const int dirX[8] = {-2, -2, -1, -1, +1, +1, +2, +2};
    const int dirY[8] = {-1, +1, -2, +2, -2, +2, -1, +1};
    for (int i = 0; i <= 7; ++i)
    {
        const int x = dirX[i] + startX;
        const int y = dirY[i] + startY;

        if (pieceRules_areCoordsValid(x, y))
        {
            if (table->table[y][x] == NULL)
                m_pieceMoves_addMove(outMoves, pieceMove_constructAdvance(x, y));
            else if (table->table[y][x]->team != knight->team)
                m_pieceMoves_addMove(outMoves, pieceMove_constructCapture(x, y, table->table[y][x]->type));
        }
    }

    m_removeBadMovesInCheck(knight, table, outMoves);
}

void pieceRules_findMovesQueen(const Piece *queen, const Table *table, LegalMoves *outMoves)
{

    const int startX = queen->x;
    const int startY = queen->y;
    *outMoves = legalMoves_constructEmptyWithStart(startX, startY);

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

    m_removeBadMovesInCheck(queen, table, outMoves);
}


void pieceRules_findMovesKing(const Piece *king, const Table *table, LegalMoves *outMoves)
{

    const int startX = king->x;
    const int startY = king->y;
    *outMoves = legalMoves_constructEmptyWithStart(startX, startY);

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
                m_pieceMoves_addMove(outMoves, pieceMove_constructAdvance(x, y));
            else
                m_pieceMoves_addMove(outMoves, pieceMove_constructCapture(x, y, table->table[y][x]->type));
        }
    }

    //castling

    //left castle
    const int y = king->y;
    Piece *leftRook = table->table[y][0];
    if (leftRook != NULL && leftRook->movesMade == 0 && king->movesMade == 0)
    {
        bool freeSpace = true;
        for (int x = 1; x < king->x; ++x)
        {
            if (table->table[y][x] != NULL)
            {
                freeSpace = false;
                break;
            }
        }
        if (freeSpace)
        {
            bool safeSpace = true;
            //castling is not allowed if the king would go through a spot that is under attack
            for (int x = king->x - 2; x <= king->x; ++x)
            {
                if (!pieceRules_canKingBeInSpot(king->team, table, x, king->y))
                {
                    safeSpace = false;
                    break;
                }

            }

            if (safeSpace)
                m_pieceMoves_addMove(outMoves, pieceMove_constructCastle(king->x - 2, y, leftRook->x, leftRook->y));
        }
    }

    Piece *rightRook = table->table[y][7];
    if (rightRook != NULL && rightRook->movesMade == 0 && king->movesMade == 0)
    {
        bool freeSpace = true;
        for (int x = king->x + 1; x <= 6; ++x)
        {
            if (table->table[y][x] != NULL)
            {
                freeSpace = false;
                break;
            }
        }
        if (freeSpace)
        {
            bool safeSpace = true;
            //castling is not allowed if the king would go through a spot that is under attack
            for (int x = king->x; x <= king->x + 2; ++x)
            {
                if (!pieceRules_canKingBeInSpot(king->team, table, x, king->y))
                {
                    safeSpace = false;
                    break;
                }

            }

            if (safeSpace)
                m_pieceMoves_addMove(outMoves, pieceMove_constructCastle(king->x + 2, y, rightRook->x, rightRook->y));
        }
    }


}


