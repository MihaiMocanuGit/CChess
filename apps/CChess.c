#include <SDL2/SDL.h>

#include <stdbool.h>

#include "ChessEngine/piece.h"
#include "ChessEngine/table.h"

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

    SDL_Surface *background = SDL_LoadBMP("../data/background.bmp");
    SDL_Texture *textureBackground = SDL_CreateTextureFromSurface(renderer, background);

    SDL_Surface *whitePawn = SDL_LoadBMP("../data/white_pawn.bmp");
    SDL_Texture *textureWhitePawn = SDL_CreateTextureFromSurface(renderer, whitePawn);

    SDL_Surface *blackPawn = SDL_LoadBMP("../data/black_pawn.bmp");
    SDL_Texture *textureBlackPawn = SDL_CreateTextureFromSurface(renderer, blackPawn);

    SDL_Surface *whiteBishop = SDL_LoadBMP("../data/white_bishop.bmp");
    SDL_Texture *textureWhiteBishop = SDL_CreateTextureFromSurface(renderer, whiteBishop);

    SDL_Surface *blackBishop = SDL_LoadBMP("../data/black_bishop.bmp");
    SDL_Texture *textureBlackBishop = SDL_CreateTextureFromSurface(renderer, blackBishop);

    SDL_Surface *whiteRook = SDL_LoadBMP("../data/white_rook.bmp");
    SDL_Texture *textureWhiteRook= SDL_CreateTextureFromSurface(renderer, whiteRook);

    SDL_Surface *blackRook = SDL_LoadBMP("../data/black_rook.bmp");
    SDL_Texture *textureBlackRook = SDL_CreateTextureFromSurface(renderer, blackRook);

    SDL_Surface *whiteKnight = SDL_LoadBMP("../data/white_knight.bmp");
    SDL_Texture *textureWhiteKnight = SDL_CreateTextureFromSurface(renderer, whiteKnight);

    SDL_Surface *blackKnight = SDL_LoadBMP("../data/black_knight.bmp");
    SDL_Texture *textureBlackKnight = SDL_CreateTextureFromSurface(renderer, blackKnight);

    SDL_Surface *whiteQueen = SDL_LoadBMP("../data/white_queen.bmp");
    SDL_Texture *textureWhiteQueen = SDL_CreateTextureFromSurface(renderer, whiteQueen);

    SDL_Surface *blackQueen = SDL_LoadBMP("../data/black_queen.bmp");
    SDL_Texture *textureBlackQueen = SDL_CreateTextureFromSurface(renderer, blackQueen);

    SDL_Surface *whiteKing = SDL_LoadBMP("../data/white_king.bmp");
    SDL_Texture *textureWhiteKing = SDL_CreateTextureFromSurface(renderer, whiteKing);

    SDL_Surface *blackKing = SDL_LoadBMP("../data/black_king.bmp");
    SDL_Texture *textureBlackKing = SDL_CreateTextureFromSurface(renderer, blackKing);

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



        SDL_RenderCopy(renderer, textureBackground, NULL, NULL);

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
                    SDL_RenderCopy(renderer, textureWhitePawn, NULL, &piecePosition);
                    break;
                case BISHOP:
                    SDL_RenderCopy(renderer, textureWhiteBishop, NULL, &piecePosition);
                    break;
                case KNIGHT:
                    SDL_RenderCopy(renderer, textureWhiteKnight, NULL, &piecePosition);
                    break;
                case ROOK:
                    SDL_RenderCopy(renderer, textureWhiteRook, NULL, &piecePosition);
                    break;
                case QUEEN:
                    SDL_RenderCopy(renderer, textureWhiteQueen, NULL, &piecePosition);
                    break;
                case KING:
                    SDL_RenderCopy(renderer, textureWhiteKing, NULL, &piecePosition);
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
                    SDL_RenderCopy(renderer, textureBlackPawn, NULL, &piecePosition);
                    break;
                case BISHOP:
                    SDL_RenderCopy(renderer, textureBlackBishop, NULL, &piecePosition);
                    break;
                case KNIGHT:
                    SDL_RenderCopy(renderer, textureBlackKnight, NULL, &piecePosition);
                    break;
                case ROOK:
                    SDL_RenderCopy(renderer, textureBlackRook, NULL, &piecePosition);
                    break;
                case QUEEN:
                    SDL_RenderCopy(renderer, textureBlackQueen, NULL, &piecePosition);
                    break;
                case KING:
                    SDL_RenderCopy(renderer, textureBlackKing, NULL, &piecePosition);
                    break;
            }
        }
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyTexture(textureBackground);
    SDL_DestroyTexture(textureWhiteKing);
    SDL_DestroyTexture(textureWhiteQueen);
    SDL_DestroyTexture(textureWhiteRook);
    SDL_DestroyTexture(textureWhiteKnight);
    SDL_DestroyTexture(textureWhiteBishop);
    SDL_DestroyTexture(textureWhitePawn);
    SDL_DestroyTexture(textureBlackKing);
    SDL_DestroyTexture(textureBlackQueen);
    SDL_DestroyTexture(textureBlackRook);
    SDL_DestroyTexture(textureBlackKnight);
    SDL_DestroyTexture(textureBlackBishop);
    SDL_DestroyTexture(textureBlackPawn);

    SDL_FreeSurface(background);
    SDL_FreeSurface(whiteKing);
    SDL_FreeSurface(whiteQueen);
    SDL_FreeSurface(whiteRook);
    SDL_FreeSurface(whiteKnight);
    SDL_FreeSurface(whiteBishop);
    SDL_FreeSurface(whitePawn);
    SDL_FreeSurface(blackKing);
    SDL_FreeSurface(blackQueen);
    SDL_FreeSurface(blackRook);
    SDL_FreeSurface(blackKnight);
    SDL_FreeSurface(blackBishop);
    SDL_FreeSurface(blackPawn);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}