#include "../include/screen.h"

#include <math.h>


Screen screen_construct(const char *windowTitle, int w, int h, Uint32 flags, Uint32 rendererFlags)
{
    Screen screen = {.screenTableLength = SCREEN_TABLE_LENGTH, .screenTableCellLength = SCREEN_TABLE_CELL_SIZE,
                     .screenPieceSize = SCREEN_PIECE_SIZE};
    screen.window = SDL_CreateWindow(windowTitle,
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      w, h, flags);
    screen.renderer = SDL_CreateRenderer(screen.window, -1, rendererFlags);
    screen.surfaces = objectSurfaces_construct();
    screen.textures = objectTextures_construct(screen.renderer, &screen.surfaces);
    return screen;
}

void screen_free(Screen *screen)
{
    objectTextures_freeAllTextures(&screen->textures);
    objectSurfaces_freeAllSurfaces(&screen->surfaces);

    SDL_DestroyRenderer(screen->renderer);
    SDL_DestroyWindow(screen->window);
}

SDL_Rect screen_piecePositionToScreenPosition(const Screen *screen, const Piece *piece)
{
    //the table is centered in the middle and its length is SCREEN_TABLE_X_Y
    //the total width and height of the screen is SCREEN_X, respectively SCREEN_Y
    int width, height;
    SDL_GetWindowSize(screen->window, &width, &height);
    const int offsetX = (width - SCREEN_TABLE_LENGTH) / 2;
    const int offsetY = (height - SCREEN_TABLE_LENGTH) / 2;

    const int pieceX = piece->x * SCREEN_TABLE_CELL_SIZE + offsetX;
    const int pieceY = piece->y * SCREEN_TABLE_CELL_SIZE + offsetY;
    SDL_Rect position = {.x = pieceX, .y = pieceY, .w = SCREEN_PIECE_SIZE, .h = SCREEN_PIECE_SIZE};
    return position;
}

void screen_screenPositionToPiecePosition(const Screen *screen, int screenCursorX, int screenCursorY, int *outPieceX, int *outPieceY)
{
    int width, height;
    SDL_GetWindowSize(screen->window, &width, &height);
    const double offsetX = (width - SCREEN_TABLE_LENGTH) / 2.0;
    const double offsetY = (height - SCREEN_TABLE_LENGTH) / 2.0;

    *outPieceX = floor((screenCursorX - offsetX) / SCREEN_TABLE_CELL_SIZE);
    *outPieceY = floor((screenCursorY - offsetY) / SCREEN_TABLE_CELL_SIZE);
}
