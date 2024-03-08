#ifndef CCHESS_TABLE_MOUSE_CONTROLLER_H
#define CCHESS_TABLE_MOUSE_CONTROLLER_H


#include <SDL2/SDL_rect.h>
#include "../lib/ChessEngine/include/table.h"
#include "screen.h"
#include "../lib/ChessEngine/include/pieceRules.h"


typedef enum
{
    EMPTY_HAND,                     //we have nothing in hand, happens after a move
    //UNINIT_STATE,                   //represents an uninitialised state, it's not exactly empty yet
    CLICKED_PICK_UP_PIECE,          //we had an empty hand, and then we picked up a new piece
    //CLICKED_SAME_TEAM_PIECE,        //already had a piece in hand and we get another one
    CLICKED_CAPTURE_PIECE,          //we have a piece in hand and want to make a legal capture
    CLICKED_ADVANCE_PIECE,          //we have a piece in hand and want to make a legal move
    CLICKED_CASTLE_KING,            //we have the king in hand and want to make a legal castle
    CLICKED_PROMOTE_PAWN,           //we have a pawn in hand from the last row and want to do legal promote
    CLICKED_PROMOTE_PAWN_CHOICE,           //we have a pawn in hand from the last row and want to do legal promote
    CLICKED_EN_PASSANT,
    CLICKED_CANCEL,                 //we clicked RMB
    CLICKED_INVALID                 //we made a wrong click, do nothing in this case
} TableClickType_e;

typedef struct
{
    const PieceTeam_e fromPerspective;
    Screen * const screen;
    Table * const table;

    //const SDL_Rect region;

    //In table coords, not screen coords
    //TODO: Rename this to picked up piece coords
    SDL_Point oldClickPos;
    //TODO: Separate click type for picked up piece, meaning, add new tags specifically for this
    TableClickType_e oldClickType; // this might not be useful

    //In table coords, not screen coords
    //TODO: Rename this to next click/move coords
    SDL_Point newClickPos;
    //TODO: Separate click type, meaning, add new tags specifically for this
    TableClickType_e newClickType;


    // We need to have access to the possible moves the current held piece can do
    // Ideally we should have this data in a single spot and access it through references from different places
    // Where should this spot be, would it be fine for it to live in main, if we make sure it never gets out of scope?
    // We would need to update this location after every click
    //LegalMoves * const movesForHeldPiece;
    LegalMoves movesForHeldPiece;
    int makeMoveAtIndex;
}TableMouseController;

TableMouseController tableMouseController_construct(PieceTeam_e fromPerspective, Screen *screen, Table *table); //, const SDL_Rect *region);
TableClickType_e tableMouseController_onClick(TableMouseController *controller, const SDL_MouseButtonEvent *e);


#endif //CCHESS_TABLE_MOUSE_CONTROLLER_H
