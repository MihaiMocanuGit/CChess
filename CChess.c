#include <SDL2/SDL.h>

#include <stdbool.h>

#include "lib/ChessEngine/chessStructure.h"

#include "screen.h"
#include "mouseControl.h"


void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

int main(int argc, char *argv[])
{
    Table table;
    table_init(&table);

    SDL_Init(SDL_INIT_VIDEO);
    Screen screen = screen_construct("CChess", 800, 600, 0, 0);

    bool quit = false;
    SDL_Event event;

    Piece* heldPiece = NULL;
    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouseControl_btnPressed(&event.button, &heldPiece, &table, &screen);
                break;
        }


        SDL_RenderCopy(screen.renderer, screen.textures.background, NULL, NULL);

        screen_drawTeams(&screen, &table);

        if (heldPiece != NULL && heldPiece->type == BISHOP)
        {
            LegalMoves moves = legalMoves_constructEmpty();
            pieceRules_findMovesBishop(heldPiece, &table, false, &moves);

            for (int i = 0; i < moves.noMoves; ++i)
            {
                const int x = moves.moves[i].x;
                const int y = moves.moves[i].y;


                SDL_Rect rect = screen_tablePositionToScreenPosition(&screen, x, y);
                if (moves.moves[i].type == MOVE)
                    SDL_RenderCopy(screen.renderer, screen.textures.moveCircle, NULL, &rect);
                else if (moves.moves[i].type == CAPTURE)
                    SDL_RenderCopy(screen.renderer, screen.textures.captureCircle, NULL, &rect);
            }

        }

        SDL_RenderPresent(screen.renderer);

    }

    screen_free(&screen);
    SDL_Quit();
    return 0;
}