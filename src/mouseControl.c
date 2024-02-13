#include "../include/mouseControl.h"

typedef struct
{
    SDL_MouseButtonEvent *e;
    Piece **heldPiece;
    Table *table;
    Screen* screen;
} MouseControl;

void m_initMouse(MouseControl *mouse, SDL_MouseButtonEvent *e, Piece **heldPiece, Table *table, Screen *screen)
{
    mouse->e = e;
    mouse->heldPiece = heldPiece;
    mouse->table = table;
    mouse->screen = screen;

}

MouseControl m_constructMouse(SDL_MouseButtonEvent *e, Piece **heldPiece, Table *table, Screen *screen)
{
    MouseControl mouse;
    m_initMouse(&mouse, e, heldPiece, table, screen);
    return mouse;
}

void m_leftBtnPressed(MouseControl *mouse)
{
    SDL_MouseButtonEvent *e = mouse->e;
    Piece **heldPiece = mouse->heldPiece;
    Table *table = mouse->table;
    Screen *screen = mouse->screen;

    int pieceX = -1, pieceY = -1;
    screen_screenPositionToPiecePosition(screen, e->x, e->y, &pieceX, &pieceY);
    if (pieceX >= 0 && pieceX < TABLE_WIDTH && pieceY >= 0 && pieceY < TABLE_HEIGHT)
    {
        if (*heldPiece == NULL && table->table[pieceY][pieceX] != NULL)
        {
            *heldPiece = table->table[pieceY][pieceX];
        }
        else if (*heldPiece != NULL && table->table[pieceY][pieceX] == NULL)
        {
            table->table[(*heldPiece)->y][(*heldPiece)->x] = NULL;

            (*heldPiece)->x = pieceX;
            (*heldPiece)->y = pieceY;
            table->table[pieceY][pieceX] = *heldPiece;
            *heldPiece = NULL;

        }
        else if (*heldPiece != NULL && table->table[pieceY][pieceX] != NULL)
        {
            // if we are selecting another piece of the same color as our team, place the held
            // piece back and pick the other one
            if (table->table[pieceY][pieceX]->team == (*heldPiece)->team)
                *heldPiece = table->table[pieceY][pieceX];
        }

    }
}

void m_rightBtnPressed(MouseControl *mouse)
{
    //put the held piece back
    *(mouse->heldPiece) = NULL;
}

void mouseControl_btnPressed(SDL_MouseButtonEvent *e, Piece **heldPiece, Table *table, Screen *screen)
{
    MouseControl mouseState = m_constructMouse(e, heldPiece, table, screen);
    switch (e->button)
    {
        case SDL_BUTTON_LEFT:
            m_leftBtnPressed(&mouseState);
            break;
        case SDL_BUTTON_RIGHT:
            m_rightBtnPressed(&mouseState);
            break;
    }

}
