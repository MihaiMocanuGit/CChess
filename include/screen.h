#ifndef CCHESS_SCREEN_H
#define CCHESS_SCREEN_H

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

#include "../lib/ChessEngine/include/piece.h"
#include "../lib/ChessEngine/include/table.h"
#include "objectSurfaces.h"
#include "objectTextures.h"


#define SCREEN_TABLE_LENGTH  512
#define SCREEN_TABLE_CELL_SIZE 64
#define SCREEN_PIECE_SIZE SCREEN_TABLE_CELL_SIZE
typedef struct
{
    const int screenTableLength, screenTableCellLength;
    const int screenPieceSize;

    SDL_Window *window;
    SDL_Renderer *renderer;

    ObjectSurfaces surfaces;
    ObjectTextures textures;
} Screen;

Screen screen_construct(const char* windowTitle, int w, int h, Uint32 windowFlags, Uint32 rendererFlags);
void screen_free(Screen *screen);

SDL_Rect screen_tablePositionToScreenPosition(const Screen *screen, int tableX, int tableY);
void screen_screenPositionToPiecePosition(const Screen *screen, int screenCursorX, int screenCursorY,
                                          int *outPieceX, int *outPieceY);

void screen_drawTeams(const Screen *screen, const Table *table);
#endif //CCHESS_SCREEN_H
