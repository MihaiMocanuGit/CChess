#include <SDL2/SDL.h>

#include <stdbool.h>

#include "lib/ChessEngine/chessStructure.h"


#include "include/Screen.h"



int main(int argc, char *argv[])
{
    Team whiteTeam;
    table_initTeamDefaultBottom(&whiteTeam, WHITE);

    Team blackTeam;
    table_initTeamDefaultTop(&blackTeam, WHITE);

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
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int pieceX, pieceY;
        screen_screenPositionToPiecePosition(&screen, mouseX, mouseY, &pieceX, &pieceY);
        printf("X: %d \t Y: %d\n", pieceX, pieceY);

        SDL_RenderCopy(screen.renderer, screen.textures.background, NULL, NULL);

        for (int i = 0; i < whiteTeam.noPieces; ++i)
        {
            const Piece *piece = &whiteTeam.pieces[i];
            SDL_Rect piecePosition = screen_piecePositionToScreenPosition(&screen, piece);

            switch (piece->type)
            {

                case PAWN:
                    SDL_RenderCopy(screen.renderer, screen.textures.whitePawn, NULL, &piecePosition);
                    break;
                case BISHOP:
                    SDL_RenderCopy(screen.renderer, screen.textures.whiteBishop, NULL, &piecePosition);
                    break;
                case KNIGHT:
                    SDL_RenderCopy(screen.renderer, screen.textures.whiteKnight, NULL, &piecePosition);
                    break;
                case ROOK:
                    SDL_RenderCopy(screen.renderer, screen.textures.whiteRook, NULL, &piecePosition);
                    break;
                case QUEEN:
                    SDL_RenderCopy(screen.renderer, screen.textures.whiteQueen, NULL, &piecePosition);
                    break;
                case KING:
                    SDL_RenderCopy(screen.renderer, screen.textures.whiteKing, NULL, &piecePosition);
                    break;
            }
        }
        for (int i = 0; i < blackTeam.noPieces; ++i)
        {
            const Piece *piece = &blackTeam.pieces[i];
            SDL_Rect piecePosition = screen_piecePositionToScreenPosition(&screen, piece);

            switch (piece->type)
            {

                case PAWN:
                    SDL_RenderCopy(screen.renderer, screen.textures.blackPawn, NULL, &piecePosition);
                    break;
                case BISHOP:
                    SDL_RenderCopy(screen.renderer, screen.textures.blackBishop, NULL, &piecePosition);
                    break;
                case KNIGHT:
                    SDL_RenderCopy(screen.renderer, screen.textures.blackKnight, NULL, &piecePosition);
                    break;
                case ROOK:
                    SDL_RenderCopy(screen.renderer, screen.textures.blackRook, NULL, &piecePosition);
                    break;
                case QUEEN:
                    SDL_RenderCopy(screen.renderer, screen.textures.blackQueen, NULL, &piecePosition);
                    break;
                case KING:
                    SDL_RenderCopy(screen.renderer, screen.textures.blackKing, NULL, &piecePosition);
                    break;
            }
        }


        SDL_RenderPresent(screen.renderer);

    }

    screen_free(&screen);
    SDL_Quit();
    return 0;
}