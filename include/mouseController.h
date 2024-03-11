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
    CLICKED_MAKE_MOVE_AND_PROMOTE_PAWN

}ActionClickState_e;

#define PAWN_PROMOTION_CHOICE_ZONE_X_START TABLE_WIDTH
#define PAWN_PROMOTION_CHOICE_ZONE_X_END TABLE_WIDTH
#define PAWN_PROMOTION_CHOICE_ZONE_Y_START 0
#define PAWN_PROMOTION_CHOICE_ZONE_Y_END 3
typedef enum
{
    PROMOTE_QUEEN = 0,
    PROMOTE_KNIGHT = 1,
    PROMOTE_BISHOP = 2,
    PROMOTE_ROOK = 3
} PromoteOptionsOrder_e;

typedef enum
{
    INVALID,
    CANCELED,
    SELECTED_PIECE,
    SELECTED_MOVE,
    STARTED_PROMOTION,
    FINISHED_PROMOTION
} ClickResult_e;

typedef struct
{
    const PieceTeam_e fromPerspective;
    Screen * const screen;
    Table * const table;


    SDL_Point heldPieceCoords;
    ClickedPieceState_e heldPieceState;

    SDL_Point actionClickCoords;
    ActionClickState_e actionClickState;
    bool showPromoteWindow;
    LegalMoves promotionMoveVariants;

    bool movesMustBeGenerated; //not used yet, but I'm setting the status from now to be easier to implement later
    LegalMoves movesForHeldPiece;
    int makeMoveAtIndex;

    ClickResult_e previousClickResult;

}MouseController;

MouseController mouseController_construct(PieceTeam_e fromPerspective, Screen *screen, Table *table); //, const SDL_Rect *region);




ClickResult_e mouseController_onClick(MouseController *controller, const SDL_MouseButtonEvent *e);

#endif //CCHESS_MOUSECONTROLLER_H
