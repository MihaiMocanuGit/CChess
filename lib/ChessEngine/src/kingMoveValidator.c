#include "kingMoveValidator.h"

#include <stddef.h>

bool pieceRules_isXValid(int x)
{
    return (x >= 0 && x < TABLE_WIDTH);
}
bool pieceRules_isYValid(int y)
{
    return (y >= 0 && y < TABLE_HEIGHT);
}


bool pieceRules_areCoordsValid(int x, int y)
{
    return pieceRules_isXValid(x) && pieceRules_isYValid(y);
}

bool m_areTherePawns(PieceTeam_e teamKing, const Table *table, int spotX, int spotY)
{
    if (teamKing == WHITE && spotY - 1 >= 0)
    {
        if (spotX - 1 >= 0)
        {
            const Piece *leftPawn = table->table[spotY - 1][spotX - 1];
            if (leftPawn != NULL && leftPawn->type == PAWN && leftPawn->team != teamKing)
                return true;
        }
        if (spotX + 1 < TABLE_WIDTH)
        {
            const Piece *leftPawn = table->table[spotY - 1][spotX + 1];
            if (leftPawn != NULL && leftPawn->type == PAWN && leftPawn->team != teamKing)
                return true;
        }
    }
    else if (teamKing == BLACK && spotY + 1 < TABLE_HEIGHT)
    {
        if (spotX - 1 >= 0)
        {
            const Piece *leftPawn = table->table[spotY + 1][spotX - 1];
            if (leftPawn != NULL && leftPawn->type == PAWN && leftPawn->team != teamKing)
                return true;
        }
        if (spotX + 1 < TABLE_WIDTH)
        {
            const Piece *leftPawn = table->table[spotY + 1][spotX + 1];
            if (leftPawn != NULL && leftPawn->type == PAWN && leftPawn->team != teamKing)
                return true;
        }
    }

    return false;
}

bool m_isThereKing(PieceTeam_e teamKing, const Table *table, int spotX, int spotY)
{
    const int dirX[8] = {-1,  0,  1, -1, 1, -1,  0,  1};
    const int dirY[8] = {-1, -1, -1,  0, 0,  1,  1,  1};

    //we can simplify this by checking the enemy team's reference to the king;
    const Piece *enemyKing = team_king_const(&table->blackTeam);
    if (teamKing == BLACK)
        enemyKing = team_king_const(&table->whiteTeam);

    int distX = spotX - enemyKing->x;
    int distY = spotY - enemyKing->y;
    if (distX < 0) distX *= -1;
    if (distY < 0) distY *= -1;

    return (distX <= 1 && distY <= 1);


//    for (int i = 0; i < 8; ++i)
//    {
//        const int x = dirX[i] + spotX;
//        const int y = dirY[i] + spotY;
//
//        if (pieceRules_areCoordsValid(x, y))
//        {
//            Piece *enemyKing = table->table[y][x];
//            //we will come across the king of the current player when we check a spot to move it
//            if (enemyKing != NULL && enemyKing->team != teamKing && enemyKing->type == KING)
//                return true;
//        }
//    }
    return false;
}

bool m_areThereDiagonalEnemies(PieceTeam_e teamKing, const Table *table, int spotX, int spotY)
{
    const int startX = spotX;
    const int startY = spotY;

    const int upperLeftSteps = (startX < startY) ? startX : startY;
    const Piece *piece = pieceRules_findFirstPieceInLine(table, startX, startY, -1, -1, upperLeftSteps);
    if (piece && (piece->type == BISHOP || piece->type == QUEEN) && piece->team != teamKing)
        return true;


    const int upperRightSteps = (TABLE_WIDTH - startX - 1 < startY) ? TABLE_WIDTH - startX - 1 : startY;
    piece = pieceRules_findFirstPieceInLine(table, startX, startY, +1, -1, upperRightSteps);
    if (piece && (piece->type == BISHOP || piece->type == QUEEN) && piece->team != teamKing)
        return true;


    const int lowerLeftSteps = (startX < TABLE_HEIGHT - startY - 1) ? startX : TABLE_HEIGHT - startY - 1;
    piece = pieceRules_findFirstPieceInLine(table, startX, startY, -1, +1, lowerLeftSteps);
    if (piece && (piece->type == BISHOP || piece->type == QUEEN) && piece->team != teamKing)
        return true;

    const int lowerRightSteps = (TABLE_WIDTH - startX - 1 < TABLE_HEIGHT - startY - 1)  ? TABLE_WIDTH - startX - 1
                                                                                        : TABLE_HEIGHT - startY - 1;
    piece = pieceRules_findFirstPieceInLine(table, startX, startY, +1, +1, lowerRightSteps);
    if (piece && (piece->type == BISHOP || piece->type == QUEEN) && piece->team != teamKing)
        return true;

    return false;
}

bool m_areThereCrossEnemies(PieceTeam_e teamKing, const Table *table, int spotX, int spotY)
{
    const int startX = spotX;
    const int startY = spotY;

    const int leftSteps = startX;
    const Piece *piece = pieceRules_findFirstPieceInLine(table, startX, startY, -1, 0, leftSteps);
    if (piece && (piece->type == ROOK || piece->type == QUEEN) && piece->team != teamKing)
        return true;


    const int rightSteps = TABLE_WIDTH - startX - 1;
    piece = pieceRules_findFirstPieceInLine(table, startX, startY, +1, 0, rightSteps);
    if (piece && (piece->type == ROOK || piece->type == QUEEN) && piece->team != teamKing)
        return true;


    const int upSteps = startY;
    piece = pieceRules_findFirstPieceInLine(table, startX, startY, 0, -1, upSteps);
    if (piece && (piece->type == ROOK || piece->type == QUEEN) && piece->team != teamKing)
        return true;

    const int downSteps = TABLE_HEIGHT - startY - 1;
    piece = pieceRules_findFirstPieceInLine(table, startX, startY, 0, +1, downSteps);
    if (piece && (piece->type == ROOK || piece->type == QUEEN) && piece->team != teamKing)
        return true;

    return false;
}

bool m_areThereKnights(PieceTeam_e teamKing, const Table *table, int spotX, int spotY)
{
    const int dirX[8] = {-2, -2, -1, -1, +1, +1, +2, +2};
    const int dirY[8] = {-1, +1, -2, +2, -2, +2, -1, +1};

    for (int i = 0; i < 8; ++i)
    {
        const int x = dirX[i] + spotX;
        const int y = dirY[i] + spotY;

        if (pieceRules_areCoordsValid(x, y))
        {
            Piece *enemyKnight = table->table[y][x];
            if (enemyKnight != NULL && enemyKnight->type == KNIGHT && enemyKnight->team != teamKing)
                return true;
        }
    }
    return false;

}

Piece *pieceRules_findFirstPieceInLine(const Table *table, int startX, int startY, int stepX, int stepY, int noSteps)
{
    for (int i = 1; i <= noSteps; ++i)
    {
        const int newX = startX + i * stepX;
        const int newY = startY + i * stepY;

        if (table->table[newY][newX] != NULL)
            return table->table[newY][newX];

    }

    return NULL;
}

bool pieceRules_canKingBeInSpot(PieceTeam_e teamKing, const Table *table, int spotX, int spotY)
{
    //search the corners for enemy pawns
    if (m_areTherePawns(teamKing, table, spotX, spotY))
        return false;

    //search the diagonals for bishops or queens
    if (m_areThereDiagonalEnemies(teamKing, table, spotX, spotY))
        return false;

    //search the cross lines for rooks or queens
    if (m_areThereCrossEnemies(teamKing, table, spotX, spotY))
        return false;

    //search for enemy knights
    if (m_areThereKnights(teamKing, table, spotX, spotY))
        return false;

    //search all neighbouring tiles for enemy king
    if (m_isThereKing(teamKing, table, spotX, spotY))
        return false;

    return true;
}
