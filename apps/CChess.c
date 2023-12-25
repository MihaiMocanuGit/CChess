#include <SDL2/SDL.h>
#include "project/piece.h"
#include "project/table.h"

int main(int argc, char *argv[])
{
    Piece piece = piece_construct(WHITE, PAWN, 0, 0);
    Team team;
    Table table;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          680, 480,
                                          0);
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(5000);
    return 0;
}