#include "../include/objectTextures.h"


void objectTextures_init(ObjectTextures* objectTextures, SDL_Renderer *renderer, const ObjectSurfaces *surfaces)
{
    objectTextures->background = SDL_CreateTextureFromSurface(renderer, surfaces->background);

    objectTextures->whitePawn = SDL_CreateTextureFromSurface(renderer, surfaces->whitePawn);
    objectTextures->blackPawn = SDL_CreateTextureFromSurface(renderer, surfaces->blackPawn);

    objectTextures->whiteBishop = SDL_CreateTextureFromSurface(renderer, surfaces->whiteBishop);
    objectTextures->blackBishop = SDL_CreateTextureFromSurface(renderer, surfaces->blackBishop);

    objectTextures->whiteRook = SDL_CreateTextureFromSurface(renderer, surfaces->whiteRook);
    objectTextures->blackRook = SDL_CreateTextureFromSurface(renderer, surfaces->blackRook);

    objectTextures->whiteKnight = SDL_CreateTextureFromSurface(renderer, surfaces->whiteKnight);
    objectTextures->blackKnight = SDL_CreateTextureFromSurface(renderer, surfaces->blackKnight);

    objectTextures->whiteQueen = SDL_CreateTextureFromSurface(renderer, surfaces->whiteQueen);
    objectTextures->blackQueen = SDL_CreateTextureFromSurface(renderer, surfaces->blackQueen);

    objectTextures->whiteKing = SDL_CreateTextureFromSurface(renderer, surfaces->whiteKing);
    objectTextures->blackKing = SDL_CreateTextureFromSurface(renderer, surfaces->blackKing);

    objectTextures->moveCircle = SDL_CreateTextureFromSurface(renderer, surfaces->moveCircle);
    objectTextures->captureCircle = SDL_CreateTextureFromSurface(renderer, surfaces->captureCircle);
}
ObjectTextures objectTextures_construct(SDL_Renderer *renderer, const ObjectSurfaces *surfaces)
{
    ObjectTextures textures;
    objectTextures_init(&textures, renderer, surfaces);

    return textures;
}

void objectTextures_freeAllTextures(ObjectTextures* objectTextures)
{
    SDL_DestroyTexture(objectTextures->background);
    SDL_DestroyTexture(objectTextures->whiteKing);
    SDL_DestroyTexture(objectTextures->whiteQueen);
    SDL_DestroyTexture(objectTextures->whiteRook);
    SDL_DestroyTexture(objectTextures->whiteKnight);
    SDL_DestroyTexture(objectTextures->whiteBishop);
    SDL_DestroyTexture(objectTextures->whitePawn);
    SDL_DestroyTexture(objectTextures->blackKing);
    SDL_DestroyTexture(objectTextures->blackQueen);
    SDL_DestroyTexture(objectTextures->blackRook);
    SDL_DestroyTexture(objectTextures->blackKnight);
    SDL_DestroyTexture(objectTextures->blackBishop);
    SDL_DestroyTexture(objectTextures->blackPawn);

    SDL_DestroyTexture(objectTextures->moveCircle);
    SDL_DestroyTexture(objectTextures->captureCircle);
}