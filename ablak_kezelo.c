#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ablak_kezelo.h"
#include "bemenet_es_fajlkezeles.h"
#include "debugmalloc.h"

#define PIROS_SDL (SDL_Color) {255, 0, 0}
#define ROZSA_SDL (SDL_Color) {255, 0, 255}
#define ZOLD_SDL (SDL_Color) {0, 255, 0}
#define KEK_SDL (SDL_Color) {0, 100, 255}
#define SZURKE_SDL (SDL_Color) {30, 30, 30}
#define FEHER_SDL (SDL_Color) {255, 255, 255}
#define FEKETE_SDL (SDL_Color) {0, 0, 0}

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
    ablak->nyitva = true;
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
    
    SDL_DestroyTexture(ablak->logo);
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

void fix_menut_kirajzol(Ablak* ablak, Betutipusok* bt) {
    /* *****MENU KIRAJZOLASA***** */
    boxRGBA(ablak->megjelenito, 0, 0, 840, 360, 30, 30, 30, 255);

    logot_rajzol(ablak, 50, 10);
    
    /* Bal es jobb teglalap */
    rectangleRGBA(ablak->megjelenito, 20, 80, 380, 330, 255, 255, 255, 255);
    rectangleRGBA(ablak->megjelenito, 400, 80, 820, 330, 255, 255, 255, 255);

    /* Fix feliratok */
    szoveget_kiir("Játékmód", 50, 65, FEHER_SDL, SZURKE_SDL, bt->med20, ablak->megjelenito, true);
    szoveget_kiir("Játékosok", 430, 65, FEHER_SDL, SZURKE_SDL, bt->med20, ablak->megjelenito, true);

    szoveget_kiir("Súgó: F10", 520, 20, FEHER_SDL, SZURKE_SDL, bt->bold20, ablak->megjelenito, true);
    szoveget_kiir("Dicsőséglista: F11", 640, 20, FEHER_SDL, SZURKE_SDL, bt->bold20, ablak->megjelenito, true);

    /* Szines negyzetek */
    boxRGBA(ablak->megjelenito, 450, 120, 470, 140, 255, 0, 0, 255);
    boxRGBA(ablak->megjelenito, 450, 170, 470, 190, 255, 0, 255, 255);
    boxRGBA(ablak->megjelenito, 450, 220, 470, 240, 0, 255, 0, 255);
    boxRGBA(ablak->megjelenito, 450, 270, 470, 290, 0, 100, 255, 255);

    /* Betuk kiirasa */
    szoveget_kiir("Q", 500, 115, PIROS_SDL, SZURKE_SDL, bt->med20, ablak->megjelenito, true);
    szoveget_kiir("/", 500, 165, ROZSA_SDL, SZURKE_SDL, bt->med20, ablak->megjelenito, true);
    szoveget_kiir("M", 500, 215, ZOLD_SDL, SZURKE_SDL, bt->med20, ablak->megjelenito, true);
    szoveget_kiir("Bal", 500, 265, KEK_SDL, SZURKE_SDL, bt->med20, ablak->megjelenito, true);

    szoveget_kiir("2", 550, 115, FEHER_SDL, SZURKE_SDL, bt->reg20, ablak->megjelenito, true);
    szoveget_kiir("*", 550, 165, FEHER_SDL, SZURKE_SDL, bt->reg20, ablak->megjelenito, true);
    szoveget_kiir("K", 550, 215, FEHER_SDL, SZURKE_SDL, bt->reg20, ablak->megjelenito, true);
    szoveget_kiir("Fel", 550, 265, FEHER_SDL, SZURKE_SDL, bt->reg20, ablak->megjelenito, true);

    szoveget_kiir("W", 600, 115, FEHER_SDL, SZURKE_SDL, bt->reg20, ablak->megjelenito, true);
    szoveget_kiir("-", 600, 165, FEHER_SDL, SZURKE_SDL, bt->reg20, ablak->megjelenito, true);
    szoveget_kiir(",", 600, 215, FEHER_SDL, SZURKE_SDL, bt->reg20, ablak->megjelenito, true);
    szoveget_kiir("Jobb", 600, 265, FEHER_SDL, SZURKE_SDL, bt->reg20, ablak->megjelenito, true);
}

void sugo_es_dics_lista_ablakok_kezelese(Billentyuk* bill, Ablak* ablakok, Betutipusok* bt) {
    /* Sugo bezarasa */
    if (bill->sugo_Esc && ablakok[SUGO].nyitva) {
        SDL_DestroyRenderer(ablakok[SUGO].megjelenito);
        SDL_DestroyWindow(ablakok[SUGO].ablak);
        ablakok[SUGO].ablak = NULL;
        ablakok[SUGO].megjelenito = NULL;
        ablakok[SUGO].nyitva = false;
    }
    /* Dicsoseglista bezarasa */
    if (bill->dics_Esc && ablakok[DICS_LISTA].nyitva) {
        SDL_DestroyRenderer(ablakok[DICS_LISTA].megjelenito);
        SDL_DestroyWindow(ablakok[DICS_LISTA].ablak);
        ablakok[DICS_LISTA].ablak = NULL;
        ablakok[DICS_LISTA].megjelenito = NULL;
        ablakok[DICS_LISTA].nyitva = false;
    }
    /* Sugo megnyitasa */
    if (bill->menu_F10 && !ablakok[SUGO].nyitva) {
        ablakot_letrehoz(ablakok+SUGO);
        SDL_RenderPresent(ablakok[SUGO].megjelenito);
        sugot_kirajzol(ablakok+SUGO, bt);
    }
    /* Dicsoseglista megnyitasa */
    if (bill->menu_F11 && !ablakok[DICS_LISTA].nyitva) {
        ablakot_letrehoz(ablakok+DICS_LISTA);
        SDL_RenderPresent(ablakok[DICS_LISTA].megjelenito);
        dics_listat_kirajzol(ablakok+DICS_LISTA, bt);
    }
}

void sugot_kirajzol(Ablak* ablak, Betutipusok* bt) {
    /* *****SUGO KIRAJZOLASA***** */
    boxRGBA(ablak->megjelenito, 0, 0, 800, 600, 30, 30, 30, 255);  
    
    /* Kepek betoltese */
    SDL_Texture* billenytu_kep = IMG_LoadTexture(ablak->megjelenito, "Schatacka_keyboard.png");
    if (billenytu_kep == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }
    SDL_Rect cel = { 59, 30, 682, 209 };
    /* Kep atmasolasa az ablakra */
    SDL_RenderCopy(ablak->megjelenito, billenytu_kep, NULL, &cel);

    logot_rajzol(ablak, 560, 250);

    szoveget_kiir("A játékosok ki- és bekapcsolása a játékosokhoz tartozó balgomb", 50, 260, FEHER_SDL, SZURKE_SDL, bt->reg15, ablak->megjelenito, false);
    szoveget_kiir("megnyomásával történik (menüben színessel jelölve.)", 50, 280, FEHER_SDL, SZURKE_SDL, bt->reg15, ablak->megjelenito, false);

    szoveget_kiir("A játékosokhoz tartozó vonal a bal- és jobbgombokkal irányítható, illetve lőni is lehet, ha a játékosnak", 50, 315, FEHER_SDL, SZURKE_SDL, bt->reg15, ablak->megjelenito, false);
    szoveget_kiir("elegendő pontja van, vagy ha rendelkezik speciális fegyverrel, amit piktogramok jeleznek a pontszám", 50, 335, FEHER_SDL, SZURKE_SDL, bt->reg15, ablak->megjelenito, false);
    szoveget_kiir("mellett.", 50, 355, FEHER_SDL, SZURKE_SDL, bt->reg15, ablak->megjelenito, false);
    
    szoveget_kiir("Ha egy játékos a játékmenet alatt vonalba, falba fut (kivéve 'Fal nélküli' játékmódban), vagy lövedékkel", 50, 390, FEHER_SDL, SZURKE_SDL, bt->reg15, ablak->megjelenito, false);
    szoveget_kiir("ütközik, akkor az adott körből kiesik, és a még játékban lévő játékosok pontokat kapnak.", 50, 410, FEHER_SDL, SZURKE_SDL, bt->reg15, ablak->megjelenito, false);
    
    szoveget_kiir("A játékmenet alatt felvehető elemek jelennek meg. Ezek lehetőséget adnak a játékosoknak arra, hogy", 50, 445, FEHER_SDL, SZURKE_SDL, bt->reg15, ablak->megjelenito, false);
    szoveget_kiir("(1) a normálnál nagyobb lövedékeket lőhessenek", 50, 465, FEHER_SDL, SZURKE_SDL, bt->reg15, ablak->megjelenito, false);
    szoveget_kiir("(2) normál méretű lövedékeket lőhessenek az előttük lévő 120 fokos tartományba", 50, 485, FEHER_SDL, SZURKE_SDL, bt->reg15, ablak->megjelenito, false);
    szoveget_kiir("(3) pajzs használatával védve legyenek az ütközésektől", 50, 505, FEHER_SDL, SZURKE_SDL, bt->reg15, ablak->megjelenito, false);
    
    szoveget_kiir("A játéknak akkor van vége, ha a legnagyobb pontszám eléri a következőt: (N-1)*40, ahol N a játékosok száma.", 25, 550, FEHER_SDL, SZURKE_SDL, bt->reg15, ablak->megjelenito, false);
    
    SDL_RenderPresent(ablak->megjelenito);

    SDL_DestroyTexture(billenytu_kep);
}

void dics_listat_kirajzol(Ablak* ablak, Betutipusok* bt) {
    SDL_RenderPresent(ablak->megjelenito);
}