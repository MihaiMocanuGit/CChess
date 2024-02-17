#include <SDL2/SDL.h>

#include <stdbool.h>

#include "lib/ChessEngine/chessStructure.h"

#include "screen.h"
#include "mouseControl.h"


void getMoves(const Piece *heldPiece, const Table *table, LegalMoves *outMoves);

void renderMoves(Screen *screen, const LegalMoves *moves);

void makeMove(const LegalMoves *moves, int moveIndex, Table *table);

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

                getMoves(heldPiece, &table, &moves);

                SDL_Rect heldPieceRect = screen_tablePositionToScreenPosition(&screen, heldPiece->x, heldPiece->y);
                SDL_RenderCopy(screen.renderer, screen.textures.selectHue, NULL, &heldPieceRect);
                renderMoves(&screen, &moves);

                makeMove(&moves, 0, &table);

            }

            SDL_RenderPresent(screen.renderer);
            updateScreen = false;
        }

        SDL_Event event;
        SDL_WaitEvent(&event);
        //while (SDL_PollEvent(&event) != 0)
        //{
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
                mouseControl_btnPressed(&event.button, &heldPiece, &table, &screen);
                updateScreen = true;
                break;
        }
        //}

    }

    screen_free(&screen);
    SDL_Quit();
    return 0;
}
void makeMove(const LegalMoves *moves, int moveIndex, Table *table)
{
    int startX = moves->startX;
    int startY = moves->startY;

    int newX = moves->moves[moveIndex].x;
    int newY = moves->moves[moveIndex].y;

    table->table[startY][startX]->movesMade++;
    const Move *move = &moves->moves[moveIndex];
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

void getMoves(const Piece *heldPiece, const Table *table, LegalMoves *outMoves)
{
    switch (heldPiece->type)
    {
        case PAWN:
            pieceRules_findMovesPawn(heldPiece, table, false, outMoves);
            break;
        case BISHOP:
            pieceRules_findMovesBishop(heldPiece, table, false, outMoves);
            break;
        case KNIGHT:
            pieceRules_findMovesKnight(heldPiece, table, false, outMoves);
            break;
        case ROOK:
            pieceRules_findMovesRook(heldPiece, table, false, outMoves);
            break;
        case QUEEN:
            pieceRules_findMovesQueen(heldPiece, table, false, outMoves);
            break;
        case KING:
            pieceRules_findMovesKing(heldPiece, table, false, outMoves);
            break;
    }
}


