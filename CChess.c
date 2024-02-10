#include <SDL2/SDL.h>

#include <stdbool.h>

#include "lib/ChessEngine/chessStructure.h"

#include "include/objectSurfaces.h"
#include "include/objectTextures.h"


#define SCREEN_X 800
#define SCREEN_Y 600
#define SCREEN_TABLE_LENGTH 512
#define SCREEN_TABLE_SQUARE_LENGTH 64
#define SCREEN_PIECE_SIZE SCREEN_TABLE_SQUARE_LENGTH


void piecePositionToScreenPosition(const Piece *piece, int *screenX, int *screenY)
{
    //the table is centered in the middle and its length is SCREEN_TABLE_X_Y
    //the total width and height of the screen is SCREEN_X, respectively SCREEN_Y
    const int offsetX = (SCREEN_X - SCREEN_TABLE_LENGTH) / 2;
    const int offsetY = (SCREEN_Y - SCREEN_TABLE_LENGTH) / 2;
    *screenX = piece->x * SCREEN_TABLE_SQUARE_LENGTH + offsetX;
    *screenY = piece->y * SCREEN_TABLE_SQUARE_LENGTH + offsetY;

}
int main(int argc, char *argv[])
{
    Team whiteTeam;
    table_initTeamDefaultBottom(&whiteTeam, WHITE);

    Team blackTeam;
    table_initTeamDefaultTop(&blackTeam, WHITE);

    SDL_Init(SDL_INIT_VIDEO);


    SDL_Window *window = SDL_CreateWindow("CChess",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_X, SCREEN_Y,
                                          0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    ObjectSurfaces surfaces = objectSurfaces_construct();
    ObjectTextures textures = objectTextures_construct(renderer, &surfaces);

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



        SDL_RenderCopy(renderer, textures.background, NULL, NULL);

        for (int i = 0; i < whiteTeam.noPieces; ++i)
        {
            const Piece *piece = &whiteTeam.pieces[i];
            int pieceScreenX, pieceScreenY;
            SDL_Rect piecePosition;
            piecePositionToScreenPosition(piece, &pieceScreenX, &pieceScreenY);

            piecePosition.x = pieceScreenX;
            piecePosition.y = pieceScreenY;
            piecePosition.w = SCREEN_PIECE_SIZE;
            piecePosition.h = SCREEN_PIECE_SIZE;
            switch (piece->type)
            {

                case PAWN:
                    SDL_RenderCopy(renderer, textures.whitePawn, NULL, &piecePosition);
                    break;
                case BISHOP:
                    SDL_RenderCopy(renderer, textures.whiteBishop, NULL, &piecePosition);
                    break;
                case KNIGHT:
                    SDL_RenderCopy(renderer, textures.whiteKnight, NULL, &piecePosition);
                    break;
                case ROOK:
                    SDL_RenderCopy(renderer, textures.whiteRook, NULL, &piecePosition);
                    break;
                case QUEEN:
                    SDL_RenderCopy(renderer, textures.whiteQueen, NULL, &piecePosition);
                    break;
                case KING:
                    SDL_RenderCopy(renderer, textures.whiteKing, NULL, &piecePosition);
                    break;
            }
        }

        for (int i = 0; i < blackTeam.noPieces; ++i)
        {
            const Piece *piece = &blackTeam.pieces[i];
            int pieceScreenX, pieceScreenY;
            SDL_Rect piecePosition;
            piecePositionToScreenPosition(piece, &pieceScreenX, &pieceScreenY);

            piecePosition.x = pieceScreenX;
            piecePosition.y = pieceScreenY;
            piecePosition.w = SCREEN_PIECE_SIZE;
            piecePosition.h = SCREEN_PIECE_SIZE;
            switch (piece->type)
            {

                case PAWN:
                    SDL_RenderCopy(renderer, textures.blackPawn, NULL, &piecePosition);
                    break;
                case BISHOP:
                    SDL_RenderCopy(renderer, textures.blackBishop, NULL, &piecePosition);
                    break;
                case KNIGHT:
                    SDL_RenderCopy(renderer, textures.blackKnight, NULL, &piecePosition);
                    break;
                case ROOK:
                    SDL_RenderCopy(renderer, textures.blackRook, NULL, &piecePosition);
                    break;
                case QUEEN:
                    SDL_RenderCopy(renderer, textures.blackQueen, NULL, &piecePosition);
                    break;
                case KING:
                    SDL_RenderCopy(renderer, textures.blackKing, NULL, &piecePosition);
                    break;
            }
        }
        SDL_RenderPresent(renderer);

    }

    objectTextures_freeAllTextures(&textures);
    objectSurfaces_freeAllSurfaces(&surfaces);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}