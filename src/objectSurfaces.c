#include "../include/objectSurfaces.h"


void objectSurfaces_init(ObjectSurfaces *objectSurfaces)
{
    objectSurfaces->background = SDL_LoadBMP(OBJECT_SURFACES_BACKGROUND_BMP_PATH);

    objectSurfaces->whitePawn = SDL_LoadBMP(OBJECT_SURFACES_WHITE_PAWN_BMP_PATH);
    objectSurfaces->blackPawn = SDL_LoadBMP(OBJECT_SURFACES_BLACK_PAWN_BMP_PATH);

    objectSurfaces->whiteBishop = SDL_LoadBMP(OBJECT_SURFACES_WHITE_BISHOP_BMP_PATH);
    objectSurfaces->blackBishop = SDL_LoadBMP(OBJECT_SURFACES_BLACK_BISHOP_BMP_PATH);

    objectSurfaces->whiteRook = SDL_LoadBMP(OBJECT_SURFACES_WHITE_ROOK_BMP_PATH);
    objectSurfaces->blackRook = SDL_LoadBMP(OBJECT_SURFACES_BLACK_ROOK_BMP_PATH);

    objectSurfaces->whiteKnight = SDL_LoadBMP(OBJECT_SURFACES_WHITE_KNIGHT_BMP_PATH);
    objectSurfaces->blackKnight = SDL_LoadBMP(OBJECT_SURFACES_BLACK_KNIGHT_BMP_PATH);

    objectSurfaces->whiteQueen = SDL_LoadBMP(OBJECT_SURFACES_WHITE_QUEEN_BMP_PATH);
    objectSurfaces->blackQueen = SDL_LoadBMP(OBJECT_SURFACES_BLACK_QUEEN_BMP_PATH);

    objectSurfaces->whiteKing= SDL_LoadBMP(OBJECT_SURFACES_WHITE_KING_BMP_PATH);
    objectSurfaces->blackKing= SDL_LoadBMP(OBJECT_SURFACES_BLACK_KING_BMP_PATH);
}


ObjectSurfaces objectSurfaces_construct()
{
    ObjectSurfaces surfaces;
    objectSurfaces_init(&surfaces);
    return surfaces;
}

void objectSurfaces_freeAllSurfaces(ObjectSurfaces *objectSurfaces)
{
    SDL_FreeSurface(objectSurfaces->background);
    SDL_FreeSurface(objectSurfaces->whiteKing);
    SDL_FreeSurface(objectSurfaces->whiteQueen);
    SDL_FreeSurface(objectSurfaces->whiteRook);
    SDL_FreeSurface(objectSurfaces->whiteKnight);
    SDL_FreeSurface(objectSurfaces->whiteBishop);
    SDL_FreeSurface(objectSurfaces->whitePawn);
    SDL_FreeSurface(objectSurfaces->blackKing);
    SDL_FreeSurface(objectSurfaces->blackQueen);
    SDL_FreeSurface(objectSurfaces->blackRook);
    SDL_FreeSurface(objectSurfaces->blackKnight);
    SDL_FreeSurface(objectSurfaces->blackBishop);
    SDL_FreeSurface(objectSurfaces->blackPawn);
}