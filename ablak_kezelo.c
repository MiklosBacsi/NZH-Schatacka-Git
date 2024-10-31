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

void fix_menut_kirajzol(Ablak* ablakok, Betutipusok* bt, SDL_Color* szinek) {
    /* *****MENU KIRAJZOLASA***** */
    boxRGBA(ablakok[MENU].megjelenito, 0, 0, 840, 360, 30, 30, 30, 255);

    logot_rajzol(&ablakok[MENU], 50, 10);
    
    /* Bal es jobb teglalap */
    rectangleRGBA(ablakok[MENU].megjelenito, 20, 80, 380, 330, 255, 255, 255, 255);
    rectangleRGBA(ablakok[MENU].megjelenito, 400, 80, 820, 330, 255, 255, 255, 255);

    /* Fix feliratok */
    szoveget_kiir("Játékmód", 50, 65, szinek[FEHER], szinek[SZURKE], bt->med20, ablakok[MENU].megjelenito, true);
    szoveget_kiir("Játékosok", 430, 65, szinek[FEHER], szinek[SZURKE], bt->med20, ablakok[MENU].megjelenito, true);

    szoveget_kiir("Súgó: F10", 520, 20, szinek[FEHER], szinek[SZURKE], bt->bold20, ablakok[MENU].megjelenito, true);
    szoveget_kiir("Dicsőséglista: F11", 640, 20, szinek[FEHER], szinek[SZURKE], bt->bold20, ablakok[MENU].megjelenito, true);

    /* Szines negyzetek */
    boxRGBA(ablakok[MENU].megjelenito, 450, 120, 470, 140, 255, 0, 0, 255);
    boxRGBA(ablakok[MENU].megjelenito, 450, 170, 470, 190, 255, 0, 255, 255);
    boxRGBA(ablakok[MENU].megjelenito, 450, 220, 470, 240, 0, 255, 0, 255);
    boxRGBA(ablakok[MENU].megjelenito, 450, 270, 470, 290, 0, 0, 255, 255);

    /* Betuk kiirasa */
    szoveget_kiir("Q", 500, 115, szinek[PIROS], szinek[SZURKE], bt->med20, ablakok[MENU].megjelenito, true);
    szoveget_kiir("/", 500, 165, szinek[ROZSA], szinek[SZURKE], bt->med20, ablakok[MENU].megjelenito, true);
    szoveget_kiir("M", 500, 215, szinek[ZOLD], szinek[SZURKE], bt->med20, ablakok[MENU].megjelenito, true);
    szoveget_kiir("Bal", 500, 265, szinek[KEK], szinek[SZURKE], bt->med20, ablakok[MENU].megjelenito, true);

    szoveget_kiir("2", 550, 115, szinek[FEHER], szinek[SZURKE], bt->reg20, ablakok[MENU].megjelenito, true);
    szoveget_kiir("*", 550, 165, szinek[FEHER], szinek[SZURKE], bt->reg20, ablakok[MENU].megjelenito, true);
    szoveget_kiir("K", 550, 215, szinek[FEHER], szinek[SZURKE], bt->reg20, ablakok[MENU].megjelenito, true);
    szoveget_kiir("Fel", 550, 265, szinek[FEHER], szinek[SZURKE], bt->reg20, ablakok[MENU].megjelenito, true);

    szoveget_kiir("W", 600, 115, szinek[FEHER], szinek[SZURKE], bt->reg20, ablakok[MENU].megjelenito, true);
    szoveget_kiir("-", 600, 165, szinek[FEHER], szinek[SZURKE], bt->reg20, ablakok[MENU].megjelenito, true);
    szoveget_kiir(",", 600, 215, szinek[FEHER], szinek[SZURKE], bt->reg20, ablakok[MENU].megjelenito, true);
    szoveget_kiir("Jobb", 600, 265, szinek[FEHER], szinek[SZURKE], bt->reg20, ablakok[MENU].megjelenito, true);
}