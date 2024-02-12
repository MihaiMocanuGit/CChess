#include <SDL2/SDL.h>

#include <stdbool.h>

#include "lib/ChessEngine/chessStructure.h"


#include "include/screen.h"

void leftMousePressed(SDL_MouseButtonEvent *e, Piece **heldPiece, Table *table, const Screen* screen)
{
    int pieceX = -1, pieceY = -1;
    screen_screenPositionToPiecePosition(screen, e->x, e->y, &pieceX, &pieceY);
    if (pieceX >= 0 && pieceX < TABLE_WIDTH && pieceY >= 0 && pieceY < TABLE_HEIGHT)
    {
        if (*heldPiece == NULL && table->table[pieceY][pieceX] != NULL)
        {
            *heldPiece = table->table[pieceY][pieceX];
        }
        else if (*heldPiece != NULL && table->table[pieceY][pieceX] == NULL)
        {
            table->table[(*heldPiece)->y][(*heldPiece)->x] = NULL;

            (*heldPiece)->x = pieceX;
            (*heldPiece)->y = pieceY;
            table->table[pieceY][pieceX] = *heldPiece;
            *heldPiece = NULL;

        }
        else if (*heldPiece != NULL && table->table[pieceY][pieceX] != NULL)
        {
            //opposing team piece might be taken
        }

    }
}
void rightMousePressed(SDL_MouseButtonEvent *e, Piece **heldPiece)
{
    //put the held piece back
    *heldPiece = NULL;
}
void mousePressed(SDL_MouseButtonEvent *e, Piece **heldPiece, Table *table, const Screen* screen)
{
    switch (e->button)
    {
        case SDL_BUTTON_LEFT:
            leftMousePressed(e, heldPiece, table, screen);
            break;
        case SDL_BUTTON_RIGHT:
            rightMousePressed(e, heldPiece);
            break;
    }

}

int main(int argc, char *argv[])
{
    Table table;
    table_init(&table);

    SDL_Init(SDL_INIT_VIDEO);
    Screen screen = screen_construct("CChess", 800, 600, 0, 0);

    bool quit = false;
    SDL_Event event;

    Piece* heldPiece = NULL;
    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mousePressed(&event.button, &heldPiece, &table, &screen);
                break;
        }


        SDL_RenderCopy(screen.renderer, screen.textures.background, NULL, NULL);

        screen_drawTeams(&screen, &table);

        SDL_RenderPresent(screen.renderer);

    }

    screen_free(&screen);
    SDL_Quit();
    return 0;
}