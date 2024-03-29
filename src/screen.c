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

SDL_Rect screen_tablePositionToScreenPosition(const Screen *screen, int tableX, int tableY)
{
    //the table is centered in the middle and its length is SCREEN_TABLE_X_Y
    //the total width and height of the screen is SCREEN_X, respectively SCREEN_Y
    int width, height;
    SDL_GetWindowSize(screen->window, &width, &height);
    const int offsetX = (width - SCREEN_TABLE_LENGTH) / 2;
    const int offsetY = (height - SCREEN_TABLE_LENGTH) / 2;

    const int pieceX = tableX * SCREEN_TABLE_CELL_SIZE + offsetX;
    const int pieceY = tableY * SCREEN_TABLE_CELL_SIZE + offsetY;
    SDL_Rect position = {.x = pieceX, .y = pieceY, .w = SCREEN_TABLE_CELL_SIZE, .h = SCREEN_TABLE_CELL_SIZE};
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

void screen_drawTeams(const Screen *screen, const Table *table)
{
    for (int i = 0; i < table->whiteTeam.noPieces; ++i)
    {
        const Piece *piece = &table->whiteTeam.pieces[i];
        SDL_Rect piecePosition = screen_tablePositionToScreenPosition(screen, piece->x, piece->y);

        switch (piece->type)
        {

            case PAWN:
                SDL_RenderCopy(screen->renderer, screen->textures.whitePawn, NULL, &piecePosition);
                break;
            case BISHOP:
                SDL_RenderCopy(screen->renderer, screen->textures.whiteBishop, NULL, &piecePosition);
                break;
            case KNIGHT:
                SDL_RenderCopy(screen->renderer, screen->textures.whiteKnight, NULL, &piecePosition);
                break;
            case ROOK:
                SDL_RenderCopy(screen->renderer, screen->textures.whiteRook, NULL, &piecePosition);
                break;
            case QUEEN:
                SDL_RenderCopy(screen->renderer, screen->textures.whiteQueen, NULL, &piecePosition);
                break;
            case KING:
                SDL_RenderCopy(screen->renderer, screen->textures.whiteKing, NULL, &piecePosition);
                break;
        }
    }
    for (int i = 0; i < table->blackTeam.noPieces; ++i)
    {
        const Piece *piece = &table->blackTeam.pieces[i];
        SDL_Rect piecePosition = screen_tablePositionToScreenPosition(screen, piece->x, piece->y);

        switch (piece->type)
        {

            case PAWN:
                SDL_RenderCopy(screen->renderer, screen->textures.blackPawn, NULL, &piecePosition);
                break;
            case BISHOP:
                SDL_RenderCopy(screen->renderer, screen->textures.blackBishop, NULL, &piecePosition);
                break;
            case KNIGHT:
                SDL_RenderCopy(screen->renderer, screen->textures.blackKnight, NULL, &piecePosition);
                break;
            case ROOK:
                SDL_RenderCopy(screen->renderer, screen->textures.blackRook, NULL, &piecePosition);
                break;
            case QUEEN:
                SDL_RenderCopy(screen->renderer, screen->textures.blackQueen, NULL, &piecePosition);
                break;
            case KING:
                SDL_RenderCopy(screen->renderer, screen->textures.blackKing, NULL, &piecePosition);
                break;
        }
    }
}

