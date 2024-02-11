#include "table.h"

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
    //  0   R   K   B   Q/K K/Q B   K   R
    //  1   P   P   P   P   P   P   P   P
    team_initEmpty(team, color);
    for (int i = 0; i < 8; ++i)
    {
        Piece pawn = piece_construct(color, PAWN, i, 1);
        team_addPiece(team, &pawn);
    }

    Piece rookLeft = piece_construct(color, ROOK, 0, 0);
    team_addPiece(team, &rookLeft);
    team->rooks[0] = m_getLastAddedPiece(team);

    Piece knightLeft = piece_construct(color, KNIGHT, 1, 0);
    team_addPiece(team, &knightLeft);

    Piece bishopLeft = piece_construct(color, BISHOP, 2, 0);
    team_addPiece(team, &bishopLeft);

    //The queen will be placed on a spot of the same color
    if (color == WHITE)
    {
        Piece queenBlack = piece_construct(BLACK, QUEEN, 3, 0);
        team_addPiece(team, &queenBlack);

        Piece kingBlack = piece_construct(BLACK, KING, 4, 0);
        team_addPiece(team, &kingBlack);
        team->king = m_getLastAddedPiece(team);
    }
    else
    {
        Piece kingWhite = piece_construct(WHITE, KING, 3, 0);
        team_addPiece(team, &kingWhite);
        team->king = m_getLastAddedPiece(team);

        Piece queenWhite = piece_construct(WHITE, QUEEN, 4, 0);
        team_addPiece(team, &queenWhite);
    }

    Piece bishopRight = piece_construct(color, BISHOP, 5, 0);
    team_addPiece(team, &bishopRight);

    Piece knightRight = piece_construct(color, KNIGHT, 6, 0);
    team_addPiece(team, &knightRight);

    Piece rookRight = piece_construct(color, ROOK, 7, 0);
    team_addPiece(team, &rookRight);
    team->rooks[1] = m_getLastAddedPiece(team);


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

Table table_construct()
{
    Table table;
    table_init(&table);
    return table;
}
