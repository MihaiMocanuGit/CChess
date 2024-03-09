#ifndef CCHESS_MOUSECONTROLLER_H
#define CCHESS_MOUSECONTROLLER_H
#include <SDL2/SDL_rect.h>
#include "screen.h"

typedef enum
{
    EMPTY_HAND,
    PICKED_UP_PIECE,

}ClickedPieceState_e;

typedef enum
{
    CLICKED_NOTHING,
    CLICKED_CANCEL,
    CLICKED_MAKE_MOVE,
    CLICKED_PROMOTE_PAWN

}ActionClickState_e;

typedef struct
{
    const SDL_Rect PROMOTE_PAWN_CHOICES_REGION;
}PromotePawnMouseController;

typedef struct
{
    const PieceTeam_e fromPerspective;
    Screen * const screen;
    Table * const table;


    SDL_Point clickedPieceCoords;
    ClickedPieceState_e clickedPieceState;

    SDL_Point actionClickCoords;
    ActionClickState_e actionClickState;

    PromotePawnMouseController promotionController;

    //bool movesMustBeGenerated;
    LegalMoves movesForHeldPiece;
    int makeMoveAtIndex;

}MouseController;

MouseController mouseController_construct(PieceTeam_e fromPerspective, Screen *screen, Table *table); //, const SDL_Rect *region);



typedef enum
{
    INVALID,
    CANCELED,
    SELECTED_PIECE,
    SELECTED_MOVE,
    STARTED_PROMOTION
} ClickResult_e;
ClickResult_e mouseController_onClick(MouseController *controller, const SDL_MouseButtonEvent *e);

#endif //CCHESS_MOUSECONTROLLER_H
