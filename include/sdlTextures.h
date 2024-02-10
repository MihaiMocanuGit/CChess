#ifndef CCHESS_SDLTEXTURES_H
#define CCHESS_SDLTEXTURES_H
#include <SDL2/SDL.h>

typedef struct
{
    SDL_Texture *background;
    SDL_Texture *whitePawn;
    SDL_Texture *blackPawn;
    SDL_Texture *whiteBishop;
    SDL_Texture *blackBishop;
    SDL_Texture *whiteRook;
    SDL_Texture *blackRook;
    SDL_Texture *whiteKnight;
    SDL_Texture *blackKnight;
    SDL_Texture *whiteQueen;
    SDL_Texture *blackQueen;
    SDL_Texture *whiteKing;
    SDL_Texture *blackKing;

} ObjectTextures;

ObjectTextures objectTextures_construct(const SDL_Renderer *renderer);
void objectTextures_init(ObjectTextures* objectTextures, const SDL_Renderer *renderer);
void objectTextures_free(ObjectTextures* objectTextures);

#endif //CCHESS_SDLTEXTURES_H
