#ifndef CCHESS_OBJECT_SURFACES_H
#define CCHESS_OBJECT_SURFACES_H

#include <SDL2/SDL.h>

#define OBJECT_SURFACES_BACKGROUND_BMP_PATH "../data/background.bmp"
#define OBJECT_SURFACES_WHITE_PAWN_BMP_PATH "../data/white_pawn.bmp"
#define OBJECT_SURFACES_BLACK_PAWN_BMP_PATH "../data/black_pawn.bmp"
#define OBJECT_SURFACES_WHITE_BISHOP_BMP_PATH "../data/white_bishop.bmp"
#define OBJECT_SURFACES_BLACK_BISHOP_BMP_PATH "../data/black_bishop.bmp"
#define OBJECT_SURFACES_WHITE_ROOK_BMP_PATH "../data/white_rook.bmp"
#define OBJECT_SURFACES_BLACK_ROOK_BMP_PATH "../data/black_rook.bmp"
#define OBJECT_SURFACES_WHITE_KNIGHT_BMP_PATH "../data/white_knight.bmp"
#define OBJECT_SURFACES_BLACK_KNIGHT_BMP_PATH "../data/black_knight.bmp"
#define OBJECT_SURFACES_WHITE_QUEEN_BMP_PATH "../data/white_queen.bmp"
#define OBJECT_SURFACES_BLACK_QUEEN_BMP_PATH "../data/black_queen.bmp"
#define OBJECT_SURFACES_WHITE_KING_BMP_PATH "../data/white_king.bmp"
#define OBJECT_SURFACES_BLACK_KING_BMP_PATH "../data/black_king.bmp"
#define OBJECT_SURFACES_MOVE_CIRCLE_BMP_PATH "../data/move_circle.bmp"
#define OBJECT_SURFACES_CAPTURE_CIRCLE_BMP_PATH "../data/capture_circle.bmp"

typedef struct
{
    SDL_Surface *background;

    SDL_Surface *whitePawn;
    SDL_Surface *blackPawn;

    SDL_Surface *whiteBishop;
    SDL_Surface *blackBishop;

    SDL_Surface *whiteRook;
    SDL_Surface *blackRook;

    SDL_Surface *whiteKnight;
    SDL_Surface *blackKnight;

    SDL_Surface *whiteQueen;
    SDL_Surface *blackQueen;

    SDL_Surface *whiteKing;
    SDL_Surface *blackKing;

    SDL_Surface  *moveCircle;
    SDL_Surface  *captureCircle;
} ObjectSurfaces;


void objectSurfaces_init(ObjectSurfaces *objectSurfaces);
ObjectSurfaces objectSurfaces_construct();
void objectSurfaces_freeAllSurfaces(ObjectSurfaces *objectSurfaces);

#endif //CCHESS_OBJECT_SURFACES_H
