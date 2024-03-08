#include "../include/TableMouseController.h"



TableMouseController tableMouseController_construct(PieceTeam_e fromPerspective, Screen *screen, Table *table)//, const SDL_Rect *region)
{
    TableMouseController result = {
                                    .fromPerspective = fromPerspective,
                                    .screen = screen,
                                    .table = table,
                                    //.region = *region,
                                    .oldClickPos = {.x = -1, .y = -1},
                                    .oldClickType = EMPTY_HAND,
                                    .newClickPos = {.x = -1, .y = -1},
                                    .newClickType = EMPTY_HAND
                                    };
    return result;
}

SDL_Point m_getTableCoords(const TableMouseController *controller, const SDL_MouseButtonEvent *e)
{
    SDL_Point point;
    screen_screenPositionToPiecePosition(controller->screen, e->x, e->y, &point.x, &point.y);
    return point;
}


bool m_tableCoordsAreValid(SDL_Point coords)
{
    return coords.x >= 0 && coords.y >= 0 && coords.x < TABLE_WIDTH && coords.y  < TABLE_HEIGHT;
}

Piece *m_getPieceAt(const Table *table, SDL_Point coords)
{
    return table->table[coords.y][coords.x];
}
bool m_isPieceAt(const Table *table, SDL_Point coords)
{
    return m_getPieceAt(table, coords);
}
bool m_isPieceOfTeamAt(const Table *table, SDL_Point coords, PieceTeam_e team)
{
    return m_isPieceAt(table, coords) && m_getPieceAt(table, coords)->team == team;
}

void m_computeMoves(const Table *table, SDL_Point heldPieceCoords, LegalMoves *outMoves)
{
    Piece *heldPiece = m_getPieceAt(table, heldPieceCoords);
    switch (heldPiece->type)
    {
        case PAWN:
            pieceRules_findMovesPawn(heldPiece, table, outMoves);
            break;
        case BISHOP:
            pieceRules_findMovesBishop(heldPiece, table, outMoves);
            break;
        case KNIGHT:
            pieceRules_findMovesKnight(heldPiece, table, outMoves);
            break;
        case ROOK:
            pieceRules_findMovesRook(heldPiece, table, outMoves);
            break;
        case QUEEN:
            pieceRules_findMovesQueen(heldPiece, table, outMoves);
            break;
        case KING:
            pieceRules_findMovesKing(heldPiece, table, outMoves);
            break;
    }
}

TableClickType_e m_leftBtnPressed(TableMouseController *controller, const SDL_MouseButtonEvent *e)
{
    SDL_Point currentCoords = m_getTableCoords(controller, e);
    if (!m_tableCoordsAreValid(currentCoords))
    {
        //we designate the region consisting of one column of 4 table cells position to the right of the table
        //as the zone used for selecting the promotion piece type
        if (controller->oldClickType == CLICKED_PROMOTE_PAWN)
        {
            if (currentCoords.x == 8 && currentCoords.y >= 0 && currentCoords.y < 4)
            {
                controller->newClickPos = currentCoords;
                controller->newClickType = CLICKED_PROMOTE_PAWN_CHOICE;
                controller->makeMoveAtIndex = currentCoords.y;

                return CLICKED_PROMOTE_PAWN_CHOICE;
            }

        }
        else
                return CLICKED_INVALID; //do nothing
    }

    //Click control flow looks like:
    //OldClick <- NewClick <- CurrentClick
    //When we have an empty hand, we go directly to oldClick. It works as an initialisation of the click flow.

    //TODO: Convert all the principal if conditions into a single switch. Also move logic into new functions
    // for every branch

    //if we have an empty hand and pick up a piece
    //Note that in this case, newClickType should also be EMPTY_HAND (or maybe UNINIT_STATE)
    if(controller->oldClickType == EMPTY_HAND)
    {
        if(m_isPieceOfTeamAt(controller->table, currentCoords, controller->fromPerspective))
        {
            if (controller->table->table[currentCoords.y][currentCoords.x]->type == PAWN
                && (currentCoords.y == 0 || currentCoords.y == 7))
                controller->oldClickType = CLICKED_PROMOTE_PAWN;
            else
                controller->oldClickType = CLICKED_PICK_UP_PIECE;
            controller->oldClickPos = currentCoords;

            //generate the moves we can do with the picked piece
            //TODO: Should we make the controller responsible for generating the moves in order to make sure that
            // we have the current moves generated, or leave the caller of this function to be responsible for this?
            controller->movesForHeldPiece = legalMoves_constructEmpty();
            m_computeMoves(controller->table, controller->oldClickPos, &controller->movesForHeldPiece);
            return controller->oldClickType;
        }
        else
        {
            return CLICKED_INVALID; //do nothing as it was a wrong click
        }

    }

    //we picked up a piece and we see what move the player wants to make
    if(controller->oldClickType == CLICKED_PICK_UP_PIECE || controller->oldClickType == CLICKED_PROMOTE_PAWN)
    {
        //if we click another piece of the same team, we put the old piece back and pick up the new one
        if(m_isPieceOfTeamAt(controller->table, currentCoords, controller->fromPerspective))
        {
            if (controller->table->table[currentCoords.y][currentCoords.x]->type == PAWN
                && (currentCoords.y == 0 || currentCoords.y == 7))
                controller->oldClickType = CLICKED_PROMOTE_PAWN;
            else
                controller->oldClickType = CLICKED_PICK_UP_PIECE;
            controller->oldClickPos = currentCoords;

            //generate the moves we can do with the picked piece
            //TODO: Should we make the controller responsible for generating the moves in order to make sure that
            // we have the current moves generated, or leave the caller of this function to be responsible for this?
            controller->movesForHeldPiece = legalMoves_constructEmpty();
            m_computeMoves(controller->table, controller->oldClickPos, &controller->movesForHeldPiece);
            return controller->oldClickType;
        }
        else
        {   // we either clicked a spot to make a move or we chose an invalid spot.

            for (int i = 0; i < controller->movesForHeldPiece.noMoves; ++i)
            {
                if (controller->movesForHeldPiece.moves[i].x == currentCoords.x &&
                    controller->movesForHeldPiece.moves[i].y == currentCoords.y)
                {
                    controller->newClickPos = currentCoords;
                    switch (controller->movesForHeldPiece.moves[i].type)
                    {
                        case MOVE:
                            controller->newClickType = CLICKED_ADVANCE_PIECE;
                            break;
                        case CAPTURE:
                            controller->newClickType = CLICKED_CAPTURE_PIECE;
                            break;
                        case CASTLE:
                            controller->newClickType = CLICKED_CASTLE_KING;
                            break;
                        case PROMOTE:
                            controller->newClickType = CLICKED_PROMOTE_PAWN;
                            break;
                        case EN_PASSANT:
                            controller->newClickType = CLICKED_EN_PASSANT;
                            break;
                    }
                    controller->makeMoveAtIndex = i;
                    return controller->newClickType;
                }
            }

            //if we didn't find our move in the list of all possible moves, then we consider it to be invalid
            return CLICKED_INVALID;
        }
    }




}

TableClickType_e m_rightBtnPressed(TableMouseController *controller, const SDL_MouseButtonEvent *e)
{
    controller->oldClickType = EMPTY_HAND;
    controller->newClickType = EMPTY_HAND;
    controller->movesForHeldPiece = legalMoves_constructEmpty();
    return CLICKED_CANCEL;
}

TableClickType_e tableMouseController_onClick(TableMouseController *controller, const SDL_MouseButtonEvent *e)
{
    switch (e->button)
    {
        case SDL_BUTTON_LEFT:
            return m_leftBtnPressed(controller, e);
            break;
        case SDL_BUTTON_RIGHT:
            return m_rightBtnPressed(controller, e);
            break;
        default:
            return CLICKED_INVALID;
    }
}

