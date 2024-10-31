#ifndef ABLAK_KEZELO_H
#define ALBLAK_KEZELO_H

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
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
    TTF_Font* med20;
    TTF_Font* ita20;
    TTF_Font* reg20;
    TTF_Font* bold20;
} Betutipusok;

typedef enum Szin {FEHER = 0, FEKETE, PIROS, ROZSA, ZOLD, KEK, SZURKE} Szin;


void ablakot_letrehoz (Ablak* ablak);

void logot_rajzol(Ablak* ablak, int x, int y);

void szoveget_kiir(char* szoveg, int x, int y, SDL_Color betu_szin, SDL_Color hatter_szin, TTF_Font* font, SDL_Renderer* megjelenito, bool hatterrel);

void fix_menut_kirajzol(Ablak* ablakok, Betutipusok* bt, SDL_Color* szinek);

#endif