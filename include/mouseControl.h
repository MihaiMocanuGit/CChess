#ifndef CCHESS_MOUSE_CONTROL_H
#define CCHESS_MOUSE_CONTROL_H

#include <SDL2/SDL_events.h>
#include "../lib/ChessEngine/include/piece.h"
#include "../lib/ChessEngine/include/table.h"
#include "screen.h"

void mouseControl_btnPressed(SDL_MouseButtonEvent *e, Piece **heldPiece, Table *table, Screen* screen);


#endif //CCHESS_MOUSE_CONTROL_H

