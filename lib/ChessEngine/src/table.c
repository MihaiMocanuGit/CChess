#include "table.h"
#include "move.h"

#include <string.h>

//Gets the last successfully added piece,
//Does not check for empty data;
static Piece* m_getLastAddedPiece(Team *team)
{
    return &(team->pieces[team->noPieces - 1]);
}

void m_initTeamDefaultTop(Team *team, PieceTeam_e color)
{   //
    //      0|W 1|B 2|W 3|B 4|W 5|B 6|W 7|B
    //  0   R   K   B   Q   K   B   K   R
    //  1   P   P   P   P   P   P   P   P
    team_initEmpty(team, color);

    //inserting the King first in order to not have to deal with subsequent element swaps
    //inside the team array that can happen when a team is captured or a pawn promoted (perhaps)
    //This way, if the king is the first element and we know that it cannot be captured (Without loosing the game)
    //we will keep it at the front of the array;
    Piece king = piece_construct(color, KING, 4, 0);
    team_addPiece(team, &king);

    for (int i = 0; i < 8; ++i)
    {
        Piece pawn = piece_construct(color, PAWN, i, 1);
        team_addPiece(team, &pawn);
    }

    Piece rookLeft = piece_construct(color, ROOK, 0, 0);
    team_addPiece(team, &rookLeft);

    Piece knightLeft = piece_construct(color, KNIGHT, 1, 0);
    team_addPiece(team, &knightLeft);

    Piece bishopLeft = piece_construct(color, BISHOP, 2, 0);
    team_addPiece(team, &bishopLeft);


    Piece queen = piece_construct(color, QUEEN, 3, 0);
    team_addPiece(team, &queen);

    // The first element of the array will be king
//    Piece king = piece_construct(color, KING, 4, 0);
//    team_addPiece(team, &king);
//    team->king = m_getLastAddedPiece(team);


    Piece bishopRight = piece_construct(color, BISHOP, 5, 0);
    team_addPiece(team, &bishopRight);

    Piece knightRight = piece_construct(color, KNIGHT, 6, 0);
    team_addPiece(team, &knightRight);

    Piece rookRight = piece_construct(color, ROOK, 7, 0);
    team_addPiece(team, &rookRight);


}

Team m_constructTeamDefaultTop(PieceTeam_e color)
{
    Team team;
    m_initTeamDefaultTop(&team, color);
    return team;
}


void m_initTeamDefaultBottom(Team *team, PieceTeam_e color)
{
    //place the pieces at the top first
    m_initTeamDefaultTop(team, color);

    //and reflect them to the bottom
    for (int i = 0; i < team->noPieces; ++i)
        team->pieces[i].y = (8 - 1) - team->pieces[i].y;

}
Team m_constructTeamDefaultBottom(PieceTeam_e color)
{
    Team team;
    m_initTeamDefaultBottom(&team, color);
    return team;
}

void table_init(Table *table)
{
    table->state = PLAYING;
    table->turn = WHITE_TO_MOVE;

    table->whiteTeam = m_constructTeamDefaultBottom(WHITE);
    table->blackTeam = m_constructTeamDefaultTop(BLACK);

    memset(table->table, 0, sizeof(table->table));

    int i;
    for (i = 0; i < table->whiteTeam.noPieces && i < table->blackTeam.noPieces; ++i)
    {
        Piece *piece = &table->whiteTeam.pieces[i];
        table->table[piece->y][piece->x] = piece;

        piece = &table->blackTeam.pieces[i];
        table->table[piece->y][piece->x] = piece;
    }

    //if the rules of chess remain unchanged, we should not enter into the next
    //2 for loops, as the no of pieces between teams is equal (both equal to 16)
    for (; i < table->whiteTeam.noPieces; ++i)
    {
        Piece *piece = &table->whiteTeam.pieces[i];
        table->table[piece->y][piece->x] = piece;
    }

    for (; i < table->blackTeam.noPieces; ++i)
    {
        Piece *piece = &table->blackTeam.pieces[i];
        table->table[piece->y][piece->x] = piece;
    }
}
void table_deepCopy(Table *newTable, const Table *original)
{
    newTable->state = original->state;
    newTable->turn = original->turn;

    newTable->whiteTeam = original->whiteTeam;
    newTable->blackTeam = original->blackTeam;

    memset(newTable->table, 0, sizeof(newTable->table));

    int i;
    for (i = 0; i < newTable->whiteTeam.noPieces && i < newTable->blackTeam.noPieces; ++i)
    {
        Piece *piece = &newTable->whiteTeam.pieces[i];
        newTable->table[piece->y][piece->x] = piece;

        piece = &newTable->blackTeam.pieces[i];
        newTable->table[piece->y][piece->x] = piece;
    }

    //if the rules of chess remain unchanged, we should not enter into the next
    //2 for loops, as the no of pieces between teams is equal (both equal to 16)
    for (; i < newTable->whiteTeam.noPieces; ++i)
    {
        Piece *piece = &newTable->whiteTeam.pieces[i];
        newTable->table[piece->y][piece->x] = piece;
    }

    for (; i < newTable->blackTeam.noPieces; ++i)
    {
        Piece *piece = &newTable->blackTeam.pieces[i];
        newTable->table[piece->y][piece->x] = piece;
    }
}

void table_capturePiece(Table *table, const LegalMoves *moves, int moveIndex)
{
    //remove captured piece from its team and move it to the capturedPiecesArray
    //remove the reference to said piece from table
    //the captured piece is move->movePartner. It might not live at newX and newY if it's en'passant
    //when removing from the team pieces, use the swap-last and remove method,
    //do not forget to update the reference from table to the swapped piece
    const Move *move = &moves->moves[moveIndex];

    const Piece *capturedPiece = table->table[move->movePartnerY][move->movePartnerX];
    Team *team = &table->whiteTeam; //suppressing pointer may be null warning
    switch (capturedPiece->team)
    {
        case WHITE:
            team = &table->whiteTeam;
            break;
        case BLACK:
            team = &table->blackTeam;
            break;
    }

    //TODO: make this a team function
    team->lostPieces[table->whiteTeam.noPieces++] = *capturedPiece;

    for (int i = 0; i < team->noPieces; ++i)
    {
        if(team->pieces[i].x == capturedPiece->x && team->pieces[i].y == capturedPiece->y)
        {
            //remove by swap with end idiom
            team->pieces[i] = team->pieces[--(team->noPieces)];

            //removing the reference owned by table
            table->table[move->movePartnerY][move->movePartnerX] = NULL;

            //this is now the swapped piece
            int x = team->pieces[i].x;
            int y = team->pieces[i].y;

            //updating reference in table
            table->table[y][x] = &(team->pieces[i]);
            break;
        }
    }

}

void m_advancePiece(Table *table, int fromX, int fromY, int toX, int toY)
{
    //move the reference to the subjectPiece from the old spot in table to the new one
    table->table[toY][toX] = table->table[fromY][fromX];
    table->table[fromY][fromX] = NULL;

    //change the position known by the piece.
    table->table[toY][toX]->x = toX;
    table->table[toY][toX]->y = toY;
}

void table_advancePiece(Table *table, const LegalMoves *moves, int moveIndex)
{
    int startX = moves->startX;
    int startY = moves->startY;

    int newX = moves->moves[moveIndex].x;
    int newY = moves->moves[moveIndex].y;

    m_advancePiece(table, startX, startY, newX, newY);
}
void table_castleKing(Table *table, const LegalMoves *moves, int moveIndex)
{
    const Move *move = &moves->moves[moveIndex];
    Piece *rook = table->table[move->movePartnerY][move->movePartnerX];
    table->table[rook->y][rook->x]->movesMade++;

    int kingNewY = moves->startY;
    int kingNewX = (rook->x < moves->startX)? 2 : 6;

    int rookNewY = rook->y;
    int rookNewX = (rook->x < moves->startX)? 3 : 5;

    m_advancePiece(table, moves->startX, moves->startY, kingNewX, kingNewY);
    m_advancePiece(table, rook->x, rook->y, rookNewX, rookNewY);
}


void table_makeMove(Table *table, const LegalMoves *moves, int moveIndex)
{
    int startX = moves->startX;
    int startY = moves->startY;

    table->table[startY][startX]->movesMade++;
    const Move *move = &moves->moves[moveIndex];
    //TODO: Implement all movement types
    switch (move->type)
    {
        case EN_PASSANT:
        case CAPTURE:
            table_capturePiece(table, moves, moveIndex);
        case MOVE:
            table_advancePiece(table, moves, moveIndex);
            break;
        case CASTLE:
            table_castleKing(table, moves, moveIndex);
            break;
        case PROMOTE:
            break;
    }
}

