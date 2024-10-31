#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ablak_kezelo.h"
#include "debugmalloc.h"

void ablakot_letrehoz (Ablak* ablak) {
    /* SDL Ablak letrehozasa */
    ablak->ablak = SDL_CreateWindow(ablak->cim, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ablak->szelesseg, ablak->magassag, 0);
    if (ablak->ablak == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    /* SDL Megjelenito letrehozasa */
    ablak->megjelenito = SDL_CreateRenderer(ablak->ablak, -1, SDL_RENDERER_SOFTWARE);
    if (ablak->megjelenito == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(ablak->megjelenito);
}

void logot_rajzol(Ablak* ablak, int x, int y) {
    /* Logo betoltese */
    ablak->logo = IMG_LoadTexture(ablak->megjelenito, "schatacka_logo.png");
    if (ablak->logo == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }

    SDL_Rect cel = { x, y, 175, 50 };
    /* Logo atmasolasa az ablakra */
    SDL_RenderCopy(ablak->megjelenito, ablak->logo, NULL, &cel);
}

void szoveget_kiir(char* szoveg, int x, int y, SDL_Color betu_szin, SDL_Color hatter_szin, TTF_Font* betu_tip, SDL_Renderer* megjelenito, bool hatterrel) {
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { 0, 0, 0, 0 };

    if (hatterrel)
        felirat = TTF_RenderUTF8_Shaded(betu_tip, szoveg, betu_szin, hatter_szin);
    else    
        felirat = TTF_RenderUTF8_Blended(betu_tip, szoveg, betu_szin);

    felirat_t = SDL_CreateTextureFromSurface(megjelenito, felirat);
    hova.x = x;
    hova.y = y;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(megjelenito, felirat_t, NULL, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}