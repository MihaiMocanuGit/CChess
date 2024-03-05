#include <SDL2/SDL.h>

#include <stdbool.h>

#include "lib/ChessEngine/chessStructure.h"

#include "screen.h"
#include "mouseControl.h"
#include "TableMouseController.h"


void renderMoves(Screen *screen, const LegalMoves *moves);

void makeMove(const LegalMoves *moves, int moveIndex, Table *table);


void applyPieceClickEffects(const TableClickType_e *clickType, Table *table, Screen *screen,
                            TableMouseController *mouseController, bool showMoves,  unsigned *turn);

int main(int argc, char *argv[])
{
    Table table;
    table_init(&table);

    SDL_Init(SDL_INIT_VIDEO);
    Screen screen = screen_construct("CChess", 800, 600, 0, 0);

    TableMouseController mouseControllerWhite = tableMouseController_construct(WHITE, &screen, &table);
    TableClickType_e clickTypeWhite;

    TableMouseController mouseControllerBlack = tableMouseController_construct(BLACK, &screen, &table);
    TableClickType_e clickTypeBlack;

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
            {
                applyPieceClickEffects(&clickTypeWhite, &table, &screen, &mouseControllerWhite, true, &turn);
            }
            else
            {
                applyPieceClickEffects(&clickTypeBlack, &table, &screen, &mouseControllerBlack, true, &turn);
            }

            screen_drawTeams(&screen, &table);

            SDL_RenderPresent(screen.renderer);
            updateScreen = false;
        }

        SDL_Event event;
        SDL_WaitEvent(&event);

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
                clickTypeWhite = tableMouseController_onClick(&mouseControllerWhite, &event.button);
                clickTypeBlack = tableMouseController_onClick(&mouseControllerBlack, &event.button);
                updateScreen = true;
                break;
        }
    }

    screen_free(&screen);
    SDL_Quit();
    return 0;
}

void applyPieceClickEffects(const TableClickType_e *clickType, Table *table, Screen *screen,
                            TableMouseController *mouseController, bool showMoves, unsigned *turn)
{
    if ((*clickType) == CLICKED_PICK_UP_PIECE)
    {
        SDL_Rect heldPieceRect = screen_tablePositionToScreenPosition(screen, (*mouseController).oldClickPos.x,
                                                                      (*mouseController).oldClickPos.y);

        if (showMoves)
        {
            SDL_RenderCopy((*screen).renderer, (*screen).textures.selectHue, NULL, &heldPieceRect);
            renderMoves(screen, &(*mouseController).movesForHeldPiece);
        }
    }
    else if((*clickType) != CLICKED_INVALID && (*clickType) != EMPTY_HAND && (*clickType) != CLICKED_CANCEL)
    {
        makeMove(&(*mouseController).movesForHeldPiece, (*mouseController).makeMoveAtIndex, table);
        (*mouseController).movesForHeldPiece = legalMoves_constructEmpty();
        (*turn)++;
    }
}

//TODO: Move this to the chess engine lib
void makeMove(const LegalMoves *moves, int moveIndex, Table *table)
{
    int startX = moves->startX;
    int startY = moves->startY;

    int newX = moves->moves[moveIndex].x;
    int newY = moves->moves[moveIndex].y;

    table->table[startY][startX]->movesMade++;
    const Move *move = &moves->moves[moveIndex];
    //TODO: Implement all movement types
    switch (move->type)
    {
        case EN_PASSANT:
        case CAPTURE:
            //remove captured piece from its team and move it to the capturedPiecesArray
            //remove the reference to said piece from table
            //the captured piece is move->movePartner. It might not live at newX and newY if it's en'passant
            //when removing from the team pieces, use the swap-last and remove method,
            //do not forget to update the reference from table to the swapped piece
        case MOVE:
            //move the reference to the subjectPiece from the old spot in table to the new one

            table->table[newY][newX] = table->table[startY][startX];
            table->table[startY][startX] = NULL;

            //change the position known by the piece.
            table->table[newY][newX]->x = newX;
            table->table[newY][newX]->y = newY;
            break;
        case CASTLE:
            break;
        case PROMOTE:
            break;
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

