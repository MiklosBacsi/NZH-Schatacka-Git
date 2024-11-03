#ifndef ABLAK_KEZELO_H
#define ABLAK_KEZELO_H

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "bemenet_es_fajlkezeles.h"
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
    bool nyitva;
} Ablak;

typedef struct Betutipusok{
    TTF_Font* med20;
    TTF_Font* reg15;
    TTF_Font* reg20;
    TTF_Font* bold20;
} Betutipusok;

typedef enum Szin {PIROS = 0, ROZSA, ZOLD, KEK, SZURKE, FEHER, FEKETE} Szin;


void ablakot_letrehoz (Ablak* ablak);

void logot_rajzol(Ablak* ablak, int x, int y);

void szoveget_kiir(char* szoveg, int x, int y, SDL_Color betu_szin, SDL_Color hatter_szin, TTF_Font* font, SDL_Renderer* megjelenito, bool hatterrel);

void fix_menut_kirajzol(Ablak* ablak, Betutipusok* bt, SDL_Color* szinek);

void sugo_es_dics_lista_ablakok_kezelese(Billentyuk* bill, Ablak* ablakok, Betutipusok* bt, SDL_Color* szinek);

void sugot_kirajzol(Ablak* ablakok, Betutipusok* bt, SDL_Color* szinek);

void dics_listat_kirajzol(Ablak* ablak, Betutipusok* bt, SDL_Color* szinek);

#endif