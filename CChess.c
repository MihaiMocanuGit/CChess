#include <SDL2/SDL.h>

#include <stdbool.h>

#include "lib/ChessEngine/chessStructure.h"

#include "screen.h"
#include "move.h"
#include "mouseController.h"


void renderMoves(Screen *screen, const LegalMoves *moves);



void applyPieceClickEffects(ClickResult_e clickResult, Table *table, Screen *screen,
                            MouseController *mouseController, bool showMoves, unsigned *turn);

int main(int argc, char *argv[])
{
    Table table;
    table_init(&table);

    SDL_Init(SDL_INIT_VIDEO);
    Screen screen = screen_construct("CChess", 800, 600, 0, 0);

//    TableMouseController mouseControllerWhite = mouseController_construct(WHITE, &screen, &table);
//    TableClickType_e clickTypeWhite;
//
//    TableMouseController mouseControllerBlack = mouseController_construct(BLACK, &screen, &table);
//    TableClickType_e clickTypeBlack;

    MouseController mouseControllerWhite = mouseController_construct(WHITE, &screen, &table);
    ClickResult_e clickResultWhite = INVALID;

    MouseController mouseControllerBlack = mouseController_construct(BLACK, &screen, &table);
    ClickResult_e clickResultBlack = INVALID;

    bool quit = false;
    bool updateScreen = true;
    unsigned turn = 0;
    while (!quit)
    {
        if (updateScreen)
        {
            SDL_RenderClear(screen.renderer);
            SDL_RenderCopy(screen.renderer, screen.textures.background, NULL, NULL);

            if (turn % 2 == 0)
                applyPieceClickEffects(clickResultWhite, &table, &screen, &mouseControllerWhite, true, &turn);
            else
                applyPieceClickEffects(clickResultBlack, &table, &screen, &mouseControllerBlack, true, &turn);


            screen_drawTeams(&screen, &table);

            SDL_RenderPresent(screen.renderer);
            updateScreen = false;
        }

        SDL_Event event;
        SDL_WaitEvent(&event);
        SDL_Event debugging;
        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_EXPOSED:
                        updateScreen = true;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                // Cursor does not work in Clion's debugger right after a mouse event
                // I could not find another way to solve this, so I am purposely throwing away a few
                // events in order to get rid of the event that is blocking my cursor
               SDL_WaitEvent(&debugging);
               SDL_WaitEvent(&debugging);
               SDL_WaitEvent(&debugging);
                if (turn % 2 == 0)
                    clickResultWhite = mouseController_onClick(&mouseControllerWhite, &event.button);
                else
                    clickResultBlack = mouseController_onClick(&mouseControllerBlack, &event.button);
                updateScreen = true;
                break;
        }
    }

    screen_free(&screen);
    SDL_Quit();
    return 0;
}

void applyPieceClickEffects(ClickResult_e clickResult, Table *table, Screen *screen,
                            MouseController *mouseController, bool showMoves, unsigned *turn)
{
    if (mouseController->clickedPieceState == PICKED_UP_PIECE && mouseController->actionClickState == CLICKED_NOTHING)
    {
        if (showMoves)
        {
            SDL_Rect heldPieceRect = screen_tablePositionToScreenPosition(screen, mouseController->clickedPieceCoords.x,
                                                                                  mouseController->clickedPieceCoords.y);
            SDL_RenderCopy((*screen).renderer, (*screen).textures.selectHue, NULL, &heldPieceRect);
            renderMoves(screen, &(*mouseController).movesForHeldPiece);
        }
    }
    else if (clickResult == SELECTED_MOVE)
    {
       table_makeMove(table, &mouseController->movesForHeldPiece, mouseController->makeMoveAtIndex);
       mouseController->movesForHeldPiece = legalMoves_constructEmpty();
       (*turn)++;
    }
}


void renderMoves(Screen *screen, const LegalMoves *moves)
{
    for (int i = 0; i < (*moves).noMoves; ++i)
    {
        const int x = (*moves).moves[i].x;
        const int y = (*moves).moves[i].y;

        SDL_Rect rect = screen_tablePositionToScreenPosition(screen, x, y);
        switch ((*moves).moves[i].type)
        {

            case MOVE:
                SDL_RenderCopy(screen->renderer, screen->textures.moveCircle, NULL, &rect);
                break;
            case CAPTURE:
                SDL_RenderCopy(screen->renderer, screen->textures.captureCircle, NULL, &rect);
                break;
            case CASTLE:
                SDL_RenderCopy(screen->renderer, screen->textures.castleCircle, NULL, &rect);
                break;
            case PROMOTE:
                SDL_RenderCopy(screen->renderer, screen->textures.promoteCircle, NULL, &rect);
                break;
            case EN_PASSANT:
                SDL_RenderCopy(screen->renderer, screen->textures.captureCircle, NULL, &rect);
                break;
        }
    }
}

