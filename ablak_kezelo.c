#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdlib.h>
#include "ablak_kezelo.h"
#include "debugmalloc.h"

void ablakot_letrehoz (Ablak* ablak) {
    /* SDL Ablak létrehozása */
    ablak->ablak = SDL_CreateWindow(ablak->cim, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ablak->szelesseg, ablak->magassag, 0);
    if (ablak->ablak == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    /* SDL Megjelenítő létrehozása */
    ablak->megjelenito = SDL_CreateRenderer(ablak->ablak, -1, SDL_RENDERER_SOFTWARE);
    if (ablak->megjelenito == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(ablak->megjelenito);
}