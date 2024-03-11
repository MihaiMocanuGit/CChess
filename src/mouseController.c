#include "../include/mouseController.h"
#include "../lib/ChessEngine/include/pieceRules.h"



MouseController mouseController_construct(PieceTeam_e fromPerspective, Screen *screen, Table *table)
{
    MouseController result = {
            .fromPerspective = fromPerspective,
            .screen = screen,
            .table = table,
            .heldPieceCoords = {.x = -1, .y = -1},
            .heldPieceState = EMPTY_HAND,
            .actionClickCoords = {.x = -1, .y = -1},
            .actionClickState = CLICKED_NOTHING,
            .makeMoveAtIndex = -1,
            .showPromoteWindow = false,
            .movesMustBeGenerated = false,
            .previousClickResult = INVALID
    };
    return result;
}

SDL_Point m_getTableCoords(const MouseController *controller, const SDL_MouseButtonEvent *e)
{
    SDL_Point point;
    screen_screenPositionToPiecePosition(controller->screen, e->x, e->y, &point.x, &point.y);
    return point;
}

bool m_tableCoordsAreValid(SDL_Point coords)
{
    return coords.x >= 0 && coords.y >= 0 && coords.x < TABLE_WIDTH && coords.y  < TABLE_HEIGHT;
}
bool m_pawnSelectionCoordsAreValid(SDL_Point coords)
{
    return PAWN_PROMOTION_CHOICE_ZONE_X_START <= coords.x && coords.x <= PAWN_PROMOTION_CHOICE_ZONE_X_END &&
            PAWN_PROMOTION_CHOICE_ZONE_Y_START <= coords.y && coords.y <= PAWN_PROMOTION_CHOICE_ZONE_Y_END;
}

Piece *m_getPieceAt(const Table *table, SDL_Point coords)
{
    return table->table[coords.y][coords.x];
}

bool m_isTherePieceAt(const Table *table, SDL_Point coords)
{
    return m_getPieceAt(table, coords);
}

bool m_isTherePieceOfTeamAt(const Table *table, SDL_Point coords, PieceTeam_e team)
{
    return m_isTherePieceAt(table, coords) && m_getPieceAt(table, coords)->team == team;
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

bool m_canPossiblePawnBePromoted(const Table *table, SDL_Point coords, PieceTeam_e team)
{
    Piece *pawn = table->table[coords.y][coords.x];
    return pawn->type == PAWN && (pawn->y == 0 || pawn->y == TABLE_HEIGHT - 1) && pawn->team == team;
}

int m_promoteGetMoveIndexFromMouse(MouseController *controller, SDL_Point currentCoords, const LegalMoves *moves)
{
    if (m_pawnSelectionCoordsAreValid(currentCoords))
    {
        const PromoteOptionsOrder_e PROMOTE_OPTION = currentCoords.y;
        for (int i = 0; i < moves->noMoves; ++i)
        {
            const Move *move = &moves->moves[i];
            if (move->type == CAPTURE_TO_PROMOTE || move->type == ADVANCE_TO_PROMOTE)
            {
                switch (PROMOTE_OPTION)
                {
                    case PROMOTE_QUEEN:
                        if (move->movePartnerType == QUEEN)
                            return i;
                        break;
                    case PROMOTE_KNIGHT:
                        if (move->movePartnerType == KNIGHT)
                            return i;
                        break;
                    case PROMOTE_BISHOP:
                        if (move->movePartnerType == BISHOP)
                            return i;
                        break;
                    case PROMOTE_ROOK:
                        if (move->movePartnerType == ROOK)
                            return i;
                        break;
                }
            }
        }
    }
    else
    {
        return -1;
    }
}

ClickResult_e m_leftBtnPressed(MouseController *controller, const SDL_MouseButtonEvent *e)
{
    SDL_Point currentCoords = m_getTableCoords(controller, e);
    if (!m_tableCoordsAreValid(currentCoords) && !m_pawnSelectionCoordsAreValid(currentCoords))
        return INVALID;

    //we previously selected a pawn and we choose to promote it with the following click
    if (controller->actionClickState == CLICKED_MAKE_MOVE_AND_PROMOTE_PAWN)
    {
        //TODO: Invoke the promotionController
        int index = m_promoteGetMoveIndexFromMouse(controller, currentCoords, &controller->movesForHeldPiece);
        if (index == -1)
            return INVALID;

        controller->makeMoveAtIndex = index;
        return FINISHED_PROMOTION;
    }

    //we are working strictly inside the table, so the pawn promotion logic hasn't been triggered yet
    if (m_tableCoordsAreValid(currentCoords))
    {
        //if we have an empty hand, we possibly want to pick up a piece
        if(controller->heldPieceState == EMPTY_HAND)
        {

            if ( m_isTherePieceOfTeamAt(controller->table, currentCoords, controller->fromPerspective))
            {
                controller->heldPieceState = PICKED_UP_PIECE;
                controller->heldPieceCoords = currentCoords;

                controller->actionClickState =  CLICKED_NOTHING;

                //TODO: Maybe add a flag for when moves need to be generated (e.g right after this function returns)
                // and leave the responsibility of move generation to another structure
                controller->movesForHeldPiece = legalMoves_constructEmpty();
                m_computeMoves(controller->table, controller->heldPieceCoords, &controller->movesForHeldPiece);

                return SELECTED_PIECE;
            }
            else // did not click a correct piece (wrong team or empty tile)
            {
                return INVALID;
            }
        }



        //If we already have a valid piece in hand, we would want to make a move
        if(controller->heldPieceState == PICKED_UP_PIECE)
        {

            //we choose another piece instead of making a move with the previous piece
            //(must be done after pawn promotion check because in that case we are clicking the same pawn again)
            if (m_isTherePieceOfTeamAt(controller->table, currentCoords, controller->fromPerspective))
            {
                //TODO: Get rid of this code duplication (see higher up)
                controller->heldPieceState = PICKED_UP_PIECE;
                controller->heldPieceCoords = currentCoords;

                controller->actionClickState =  CLICKED_NOTHING;

                //TODO: Maybe add a flag for when moves need to be generated (e.g right after this function returns)
                // and leave the responsibility of move generation to another structure
                controller->movesForHeldPiece = legalMoves_constructEmpty();
                m_computeMoves(controller->table, controller->heldPieceCoords, &controller->movesForHeldPiece);

                return SELECTED_PIECE;
            }
            else //we are making a move;
            {

                for (int i = 0; i < controller->movesForHeldPiece.noMoves; ++i)
                {
                    Move *move = &controller->movesForHeldPiece.moves[i];
                    if (currentCoords.x == move->x && currentCoords.y == move->y)
                    {
                        controller->actionClickState = CLICKED_MAKE_MOVE;
                        controller->actionClickCoords = currentCoords;
                        controller->makeMoveAtIndex = i;

                        int pieceX = controller->movesForHeldPiece.startX;
                        int pieceY = controller->movesForHeldPiece.startY;
                        const Piece *pawn = controller->table->table[pieceY][pieceX];

                        if (pawn->type == PAWN && (move->y == 0 || move->y == TABLE_HEIGHT - 1))
                        {
                            controller->actionClickState = CLICKED_MAKE_MOVE_AND_PROMOTE_PAWN;
                            return STARTED_PROMOTION;
                        }
                        else
                            return SELECTED_MOVE;
                    }
                }

                return INVALID;
            }
        }

    }


}


int m_promoteGetMoveIndexFromMouse2(MouseController *controller, SDL_Point currentCoords)
{
    if (m_pawnSelectionCoordsAreValid(currentCoords))
    {
        const PromoteOptionsOrder_e PROMOTE_OPTION = currentCoords.y;
        for (int i = 0; i < controller->promotionMoveVariants.noMoves; ++i)
        {
            const Move *move = &controller->promotionMoveVariants.moves[i];
            if (move->type == CAPTURE_TO_PROMOTE || move->type == ADVANCE_TO_PROMOTE)
            {
                switch (PROMOTE_OPTION)
                {
                    case PROMOTE_QUEEN:
                        if (move->promoteTo == QUEEN)
                            return i;
                        break;
                    case PROMOTE_KNIGHT:
                        if (move->promoteTo == KNIGHT)
                            return i;
                        break;
                    case PROMOTE_BISHOP:
                        if (move->promoteTo == BISHOP)
                            return i;
                        break;
                    case PROMOTE_ROOK:
                        if (move->promoteTo == ROOK)
                            return i;
                        break;
                }
            }
        }
    }
    else
    {
        return -1;
    }
}

bool m_shouldClearController(MouseController *controller)
{
    return (controller->actionClickState == CLICKED_MAKE_MOVE && controller->previousClickResult != INVALID) ||
           controller->previousClickResult == FINISHED_PROMOTION;
}
void m_clearController(MouseController *controller)
{
    controller->showPromoteWindow = false;
    controller->heldPieceState = EMPTY_HAND;
    controller->heldPieceCoords.x = -1;
    controller->heldPieceCoords.y = -1;

    controller->actionClickState = CLICKED_NOTHING;
    controller->actionClickCoords.x = -1;
    controller->actionClickCoords.y = -1;
    controller->makeMoveAtIndex = -1;

    // memset to zero if needed: controller->promotionMoveVariants;

}
ClickResult_e m_leftBtnPressed2(MouseController *controller, const SDL_MouseButtonEvent *e)
{
    SDL_Point currentCoords = m_getTableCoords(controller, e);

    //if we just finished a move, clear the old state
    if (m_shouldClearController(controller))
        m_clearController(controller);

    if (!m_tableCoordsAreValid(currentCoords) && !controller->showPromoteWindow)
    {
        controller->previousClickResult = INVALID;
        return INVALID;
    }
    else if (controller->showPromoteWindow)
    {
        if(!m_pawnSelectionCoordsAreValid(currentCoords))
        {
            controller->previousClickResult = INVALID;
            return INVALID;
        }

        int index = m_promoteGetMoveIndexFromMouse2(controller, currentCoords);
        if (index == -1)
        {
            controller->previousClickResult = INVALID;
            return INVALID;
        }
        else
        {
            controller->makeMoveAtIndex = index;
            controller->movesForHeldPiece = controller->promotionMoveVariants;
            controller->previousClickResult = FINISHED_PROMOTION;
            return FINISHED_PROMOTION;
        }

    }
    else
    {
        if (controller->heldPieceState == EMPTY_HAND)
        {
            if ( m_isTherePieceOfTeamAt(controller->table, currentCoords, controller->fromPerspective))
            {
                controller->heldPieceState = PICKED_UP_PIECE;
                controller->heldPieceCoords = currentCoords;

                controller->actionClickState =  CLICKED_NOTHING;

                //TODO: Maybe add a flag for when moves need to be generated (e.g right after this function returns)
                // and leave the responsibility of move generation to another structure
                controller->movesMustBeGenerated = true;
                controller->movesForHeldPiece = legalMoves_constructEmpty();
                m_computeMoves(controller->table, controller->heldPieceCoords, &controller->movesForHeldPiece);

                controller->previousClickResult = SELECTED_PIECE;
                return SELECTED_PIECE;
            }
            else // did not click a correct piece (wrong team or empty tile)
            {
                controller->previousClickResult = INVALID;
                return INVALID;
            }
        }
        else if(controller->heldPieceState == PICKED_UP_PIECE)
        {
            if ( m_isTherePieceOfTeamAt(controller->table, currentCoords, controller->fromPerspective))
            {
                //TODO: Get rid of this code duplication (see higher up)
                controller->heldPieceState = PICKED_UP_PIECE;
                controller->heldPieceCoords = currentCoords;

                controller->actionClickState =  CLICKED_NOTHING;

                //TODO: Maybe add a flag for when moves need to be generated (e.g right after this function returns)
                // and leave the responsibility of move generation to another structure
                controller->movesMustBeGenerated = true;
                controller->movesForHeldPiece = legalMoves_constructEmpty();
                m_computeMoves(controller->table, controller->heldPieceCoords, &controller->movesForHeldPiece);

                controller->previousClickResult = SELECTED_PIECE;
                return SELECTED_PIECE;
            }
            else //we are making a move;
            {
                const int startX = controller->movesForHeldPiece.startX;
                const int startY = controller->movesForHeldPiece.startY;
                Piece *piece = controller->table->table[startY][startX];

                //special behaviour for pawn promotion
                if (piece->type == PAWN && ( (piece->team == WHITE && startY == 1) ||
                                              (piece->team == BLACK && startY == TABLE_HEIGHT - 2)))
                {
                    LegalMoves promotionMoves;
                    legalMoves_initEmptyWithStart(&promotionMoves, startX, startY);
                    for (int i = 0; i < controller->movesForHeldPiece.noMoves; ++i)
                    {
                        Move *move = &controller->movesForHeldPiece.moves[i];
                        if (currentCoords.x == move->x && currentCoords.y == move->y)
                            promotionMoves.moves[promotionMoves.noMoves++] = *move;

                    }

                    if (promotionMoves.noMoves > 0)
                    {
                        controller->showPromoteWindow = true;
                        controller->actionClickState = CLICKED_MAKE_MOVE_AND_PROMOTE_PAWN;
                        controller->actionClickCoords = currentCoords;
                        controller->promotionMoveVariants = promotionMoves;

                        controller->previousClickResult = STARTED_PROMOTION;
                        return STARTED_PROMOTION;
                    }
                    else
                    {
                        controller->previousClickResult = INVALID;
                        return INVALID;
                    }
                }


                //classic move (simple advance/capture, castle or en'passant
                for (int i = 0; i < controller->movesForHeldPiece.noMoves; ++i)
                {
                    Move *move = &controller->movesForHeldPiece.moves[i];
                    if (currentCoords.x == move->x && currentCoords.y == move->y)
                    {
                        controller->actionClickState = CLICKED_MAKE_MOVE;
                        controller->actionClickCoords = currentCoords;
                        controller->makeMoveAtIndex = i;
                        controller->showPromoteWindow = false;

                        controller->previousClickResult = SELECTED_MOVE;
                        return SELECTED_MOVE;
                    }
                }

                controller->previousClickResult = INVALID;
                return INVALID;
            }
        }
    }


}

ClickResult_e m_rightBtnPressed(MouseController *controller, const SDL_MouseButtonEvent *e)
{
    controller->heldPieceState = EMPTY_HAND;
    controller->actionClickState = CLICKED_CANCEL;
    controller->movesForHeldPiece = legalMoves_constructEmpty();
    return CANCELED;
}
ClickResult_e mouseController_onClick(MouseController *controller, const SDL_MouseButtonEvent *e)
{
    switch (e->button)
    {
        case SDL_BUTTON_LEFT:
            return m_leftBtnPressed2(controller, e);
            break;
        case SDL_BUTTON_RIGHT:
            return m_rightBtnPressed(controller, e);
            break;
        default:
            return INVALID;
    }
}