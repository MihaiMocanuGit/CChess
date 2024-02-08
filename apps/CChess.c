#include <SDL2/SDL.h>

#include <stdbool.h>

#include "project/piece.h"
#include "project/table.h"

int main(int argc, char *argv[])
{
    Piece piece = piece_construct(WHITE, PAWN, 0, 0);
    Team team;
    Table table;

    SDL_Init(SDL_INIT_VIDEO);


    SDL_Window *window = SDL_CreateWindow("CChess",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface *background = SDL_LoadBMP("../data/background.bmp");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, background);
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

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}