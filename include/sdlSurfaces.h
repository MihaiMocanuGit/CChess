#ifndef CCHESS_TEXTURES_H
#define CCHESS_TEXTURES_H

#include <SDL2/SDL.h>

#define BACKGROUND_BMP_PATH "../data/background.bmp"
#define WHITE_PAWN_BMP_PATH "../data/white_pawn.bmp"
#define BLACK_PAWN_BMP_PATH "../data/black_pawn.bmp"
#define WHITE_BISHOP_BMP_PATH "../data/white_bishop.bmp"
#define BLACK_BISHOP_BMP_PATH "../data/black_bishop.bmp"
#define WHITE_ROOK_BMP_PATH "../data/white_rook.bmp"
#define BLACK_ROOK_BMP_PATH "../data/black_rook.bmp"
#define WHITE_KNIGHT_BMP_PATH "../data/white_knight.bmp"
#define BLACK_KNIGHT_BMP_PATH "../data/black_knight.bmp"
#define WHITE_QUEEN_BMP_PATH "../data/white_queen.bmp"
#define BLACK_QUEEN_BMP_PATH "../data/black_queen.bmp"
#define WHITE_KING_BMP_PATH "../data/white_king.bmp"
#define BLACK_KING_BMP_PATH "../data/black_king.bmp"


SDL_Surface *getWhitePawnSurface();
SDL_Surface *getBlackPawnSurface();

SDL_Surface *getWhiteBishopSurface();
SDL_Surface *getBlackBishopSurface();

SDL_Surface *getWhiteRookSurface();
SDL_Surface *getBlackRookSurface();

SDL_Surface *getWhiteKnightSurface();
SDL_Surface *getBlackKnightSurface();

SDL_Surface *getWhiteQueenSurface();
SDL_Surface *getBlackQueenSurface();

SDL_Surface *getWhiteKingSurface();
SDL_Surface *getBlackKingSurface();

void freeAllSurfaces();

#endif //CCHESS_TEXTURES_H
