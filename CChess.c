#include <SDL2/SDL.h>

#include <stdbool.h>

#include "lib/ChessEngine/chessStructure.h"

#include "screen.h"
#include "mouseControl.h"


int main(int argc, char *argv[])
{
    Table table;
    table_init(&table);

    SDL_Init(SDL_INIT_VIDEO);
    Screen screen = screen_construct("CChess", 800, 600, 0, 0);




    Piece* heldPiece = NULL;

    bool quit = false;
    bool updateScreen = true;
    while (!quit)
    {
        if (updateScreen)
        {
            SDL_RenderClear(screen.renderer);
            SDL_RenderCopy(screen.renderer, screen.textures.background, NULL, NULL);

            screen_drawTeams(&screen, &table);

            if (heldPiece != NULL)
            {
                LegalMoves moves = legalMoves_constructEmpty();

                switch (heldPiece->type)
                {
                    case PAWN:
                        pieceRules_findMovesPawn(heldPiece, &table, false, &moves);
                        break;
                    case BISHOP:
                        pieceRules_findMovesBishop(heldPiece, &table, false, &moves);
                        break;
                    case KNIGHT:
                        pieceRules_findMovesKnight(heldPiece, &table, false, &moves);
                        break;
                    case ROOK:
                        pieceRules_findMovesRook(heldPiece, &table, false, &moves);
                        break;
                    case QUEEN:
                        pieceRules_findMovesQueen(heldPiece, &table, false, &moves);
                        break;
                    case KING:
                        pieceRules_findMovesKing(heldPiece, &table, false, &moves);
                        break;
                }

                SDL_Rect heldPieceRect = screen_tablePositionToScreenPosition(&screen, heldPiece->x, heldPiece->y);
                SDL_RenderCopy(screen.renderer, screen.textures.selectHue, NULL, &heldPieceRect);
                for (int i = 0; i < moves.noMoves; ++i)
                {
                    const int x = moves.moves[i].x;
                    const int y = moves.moves[i].y;

                    SDL_Rect rect = screen_tablePositionToScreenPosition(&screen, x, y);
                    switch (moves.moves[i].type)
                    {

                        case MOVE:
                            SDL_RenderCopy(screen.renderer, screen.textures.moveCircle, NULL, &rect);
                            break;
                        case CAPTURE:
                            SDL_RenderCopy(screen.renderer, screen.textures.captureCircle, NULL, &rect);
                            break;
                        case CASTLE:
                            SDL_RenderCopy(screen.renderer, screen.textures.castleCircle, NULL, &rect);
                            break;
                        case PROMOTE:
                            SDL_RenderCopy(screen.renderer, screen.textures.promoteCircle, NULL, &rect);
                            break;
                        case EN_PASSANT:
                            SDL_RenderCopy(screen.renderer, screen.textures.captureCircle, NULL, &rect);
                            break;
                    }
                }

            }

            SDL_RenderPresent(screen.renderer);
            updateScreen = false;
        }

        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouseControl_btnPressed(&event.button, &heldPiece, &table, &screen);
                    updateScreen = true;
                    break;
            }
        }
    }

    screen_free(&screen);
    SDL_Quit();
    return 0;
}