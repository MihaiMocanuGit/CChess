#include <SDL2/SDL.h>

#include <stdbool.h>

#include "lib/ChessEngine/chessStructure.h"


#include "include/screen.h"



int main(int argc, char *argv[])
{
    Table table = table_construct();

    SDL_Init(SDL_INIT_VIDEO);
    Screen screen = screen_construct("CChess", 800, 600, 0, 0);

    bool quit = false;
    SDL_Event event;

    Piece* heldPiece = NULL;
    while (!quit)
    {
        SDL_WaitEvent(&event);
        int cursorX, cursorY;
        Uint32 mouseState = SDL_GetMouseState(&cursorX, &cursorY);

        //if left click is pressed
        if (SDL_BUTTON(SDL_BUTTON_LEFT) & mouseState)
        {
            int pieceX = -1, pieceY = -1;
            screen_screenPositionToPiecePosition(&screen, cursorX, cursorY, &pieceX, &pieceY);
            if (pieceX >= 0 && pieceX < TABLE_WIDTH && pieceY >= 0 && pieceY < TABLE_HEIGHT)
            {
                if (heldPiece == NULL && table.table[pieceY][pieceX] != NULL)
                {
                    heldPiece = table.table[pieceY][pieceX];
                }
                else if(heldPiece != NULL && table.table[pieceY][pieceX] == NULL)
                {
                    heldPiece->x = pieceX;
                    heldPiece->y = pieceY;
                    table.table[pieceY][pieceX] = heldPiece;

                }
                else if(heldPiece != NULL && table.table[pieceY][pieceX] != NULL)
                {
                     //opposing team piece might be taken
                }

            }
        }
        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
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