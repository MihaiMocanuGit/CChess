#ifndef CCHESS_UICOMPONENT_H
#define CCHESS_UICOMPONENT_H

#include <SDL2/SDL.h>
#include "vector.h"

typedef enum
{
    DEFAULT
} UiClickType_e;

typedef struct {
    UiClickType_e typeE;
    void *additionalData;
} UiClick;

typedef struct {
    UiClick (*onClick)();

    SDL_Rect region;
    SDL_Surface *surface;
    SDL_Texture *texture;
    Vector UiSubRegions;
}UiRegion;
#endif //CCHESS_UICOMPONENT_H
