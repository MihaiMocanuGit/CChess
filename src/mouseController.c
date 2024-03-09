#include "mouseController.h"
#include "pieceRules.h"



MouseController tableMouseController_construct(PieceTeam_e fromPerspective, Screen *screen, Table *table)
{
    PromotePawnMouseController promotionController = {};
    MouseController result = {
            .fromPerspective = fromPerspective,
            .screen = screen,
            .table = table,
            .clickedPieceCoords = {.x = -1, .y = -1},
            .clickedPieceState = EMPTY_HAND,
            .actionClickCoords = {.x = -1, .y = -1},
            .actionClickState = CLICKED_NOTHING,
            .promotionController = promotionController,
            .makeMoveAtIndex = -1
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
    return coords.x == 8 && (coords.y >= 0 || coords.y < 4)
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
ClickResult_e m_leftBtnPressed(MouseController *controller, const SDL_MouseButtonEvent *e)
{
    SDL_Point currentCoords = m_getTableCoords(controller, e);
    if (!m_tableCoordsAreValid(currentCoords) && !m_pawnSelectionCoordsAreValid(currentCoords))
        return INVALID;

    //we previously selected a pawn and we choose to promote it with the following click
    if (controller->actionClickState ==  CLICKED_PROMOTE_PAWN)
    {
        //TODO: Invoke the promotionController
    }

    //we are working strictly inside the table, so the pawn promotion logic hasn't been triggered yet
    if (m_tableCoordsAreValid(currentCoords))
    {
        //if we have an empty hand, we possibly want to pick up a piece
        if(controller->clickedPieceState == EMPTY_HAND)
        {

            if ( m_isTherePieceOfTeamAt(controller->table, currentCoords, controller->fromPerspective))
            {
                controller->clickedPieceState = PICKED_UP_PIECE;
                controller->clickedPieceCoords = currentCoords;

                controller->actionClickState =  CLICKED_NOTHING;

                //TODO: Maybe add a flag for when moves need to be generated (e.g right after this function returns)
                // and leave the responsibility of move generation to another structure
                controller->movesForHeldPiece = legalMoves_constructEmpty();
                m_computeMoves(controller->table, controller->clickedPieceCoords, &controller->movesForHeldPiece);

                return SELECTED_PIECE;
            }
            else // did not click a correct piece (wrong team or empty tile)
            {
                return INVALID;
            }
        }



        //If we already have a valid piece in hand, we would want to make a move
        if(controller->clickedPieceState == PICKED_UP_PIECE)
        {
            //if we click the same exact pawn, and it is in a valid spot for promotion
            if (currentCoords.x == controller->clickedPieceCoords.x &&
                currentCoords.y == controller->clickedPieceCoords.y &&
                m_canPossiblePawnBePromoted(controller->table, currentCoords, controller->fromPerspective))
            {
                controller->actionClickState =  CLICKED_PROMOTE_PAWN;
                controller->actionClickCoords = currentCoords;
            }

            //we choose another piece instead of making a move with the previous piece
            //(must be done after pawn promotion check because in that case we are clicking the same pawn again)
            if (m_isTherePieceOfTeamAt(controller->table, currentCoords, controller->fromPerspective))
            {
                //TODO: Get rid of this code duplication (see higher up)
                controller->clickedPieceState = PICKED_UP_PIECE;
                controller->clickedPieceCoords = currentCoords;

                controller->actionClickState =  CLICKED_NOTHING;

                //TODO: Maybe add a flag for when moves need to be generated (e.g right after this function returns)
                // and leave the responsibility of move generation to another structure
                controller->movesForHeldPiece = legalMoves_constructEmpty();
                m_computeMoves(controller->table, controller->clickedPieceCoords, &controller->movesForHeldPiece);

                return SELECTED_PIECE;
            }
            else //we are making a move;
            {
                bool moveExits = false;

                for (int i = 0; i < controller->movesForHeldPiece.noMoves; ++i)
                {
                    Move *move = &controller->movesForHeldPiece.moves[i];
                    if (currentCoords.x == move->x && currentCoords.y == move->y)
                    {
                        moveExits = true;
                        controller->makeMoveAtIndex = i;
                        return SELECTED_MOVE;
                    }
                }

                return INVALID;
            }
        }

    }
}
ClickResult_e m_rightBtnPressed(MouseController *controller, const SDL_MouseButtonEvent *e)
{
    controller->clickedPieceState = EMPTY_HAND;
    controller->actionClickState = CLICKED_CANCEL;
    controller->movesForHeldPiece = legalMoves_constructEmpty();
    return CANCELED;
}
ClickResult_e tableMouseController_onClick(MouseController *controller, const SDL_MouseButtonEvent *e)
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
            return INVALID;
    }
}