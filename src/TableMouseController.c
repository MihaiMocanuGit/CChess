//
// Created by mihai on 3/4/24.
//

#include "../include/TableMouseController.h"




TableMouseController tableMouseController_construct(PieceTeam_e fromPerspective, Screen *screen, Table *table, const SDL_Rect *region)
{
    TableMouseController result = {
                                    .fromPerspective = fromPerspective,
                                    .screen = screen,
                                    .table = table,
                                    .region = *region,
                                    .oldClickPos = {.x = -1, .y = -1},
                                    .oldClickType = EMPTY_HAND,
                                    .newClickPos = {.x = -1, .y = -1},
                                    .newClickType = EMPTY_HAND
                                    };
    return result;
}

SDL_Point m_getTableCoords(const TableMouseController *controller, const SDL_MouseButtonEvent *e)
{
    SDL_Point point;
    screen_screenPositionToPiecePosition(controller->screen, e->x, e->y, &point.x, &point.y);
    return point;
}

bool m_tableCoordsAreValid(SDL_Point coords)
{
    return coords.x >= 0 && coords.y >= 0 && coords.x < TABLE_WIDTH && coords.y  < TABLE_HEIGHT;
}
TableClickType_e m_leftBtnPressed(TableMouseController *controller, const SDL_MouseButtonEvent *e)
{
    SDL_Point tableCoords = m_getTableCoords(controller, e);
    if (!m_tableCoordsAreValid(tableCoords))
        return CLICKED_INVALID;


}

TableClickType_e m_rightBtnPressed(TableMouseController *controller, const SDL_MouseButtonEvent *e)
{
    return CLICKED_CANCEL;
}

TableClickType_e tableMouseController_onClick(TableMouseController *controller, const SDL_MouseButtonEvent *e)
{
    switch (e->button)
    {
        case SDL_BUTTON_LEFT:
            return m_leftBtnPressed(controller, e);
            break;
        case SDL_BUTTON_RIGHT:
            return m_rightBtnPressed(controller, e);
            break;
        default:
            return CLICKED_INVALID;
    }
}

