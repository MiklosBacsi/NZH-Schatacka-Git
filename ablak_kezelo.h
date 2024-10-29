#ifndef ABLAK_KEZELO_H
#define ALBLAK_KEZELO_H

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"

typedef enum Ablak_tipus {MENU, JATEK, SUGO, DICS_LISTA} Ablak_tipus;

typedef struct Ablak {
    SDL_Window* ablak;
    SDL_Renderer* megjelenito;
    int szelesseg, magassag;
    Ablak_tipus abl_tip;
} Ablak;

void menu_init();



#endif