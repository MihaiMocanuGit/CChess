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
    while (!quit)
    {
        SDL_WaitEvent(&event);

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