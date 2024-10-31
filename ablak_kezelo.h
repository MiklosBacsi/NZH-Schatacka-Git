#ifndef ABLAK_KEZELO_H
#define ALBLAK_KEZELO_H

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "debugmalloc.h"

typedef enum Ablak_tipus {MENU = 0, JATEK, SUGO, DICS_LISTA} Ablak_tipus;

typedef struct Ablak {
    SDL_Window* ablak;
    SDL_Renderer* megjelenito;
    int szelesseg, magassag;
    char* cim;
    Ablak_tipus abl_tip;
    SDL_Texture* logo;
} Ablak;

typedef struct Betutipusok{
    SDL_Surface* felirat;
    SDL_Texture* felirat_t;
    TTF_Font* reg20;
    TTF_Font* bold20;
} Betutipusok;


void ablakot_letrehoz (Ablak* ablak);

void logot_rajzol(Ablak* ablak, int x, int y);

#endif