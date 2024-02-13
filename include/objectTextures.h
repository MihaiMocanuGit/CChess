#ifndef CCHESS_OBJECT_TEXTURES_H
#define CCHESS_OBJECT_TEXTURES_H
#include <SDL2/SDL.h>

#include "objectSurfaces.h"

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

    SDL_Texture  *moveCircle;
    SDL_Texture  *captureCircle;
    SDL_Texture  *promoteCircle;
    SDL_Texture  *castleCircle;

    SDL_Texture  *selectHue;

} ObjectTextures;

ObjectTextures objectTextures_construct(SDL_Renderer *renderer, const ObjectSurfaces *surfaces);
void objectTextures_init(ObjectTextures* objectTextures, SDL_Renderer *renderer, const ObjectSurfaces *surfaces);
void objectTextures_freeAllTextures(ObjectTextures* objectTextures);

#endif //CCHESS_OBJECT_TEXTURES_H
