#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ablak_kezelo.h"
#include "jatek_vezerlo.h"
#include "debugmalloc.h"

void inicializalas(Ablak* ablakok, Betutipusok* bt, SDL_Color* szinek);
void felszabadit(Ablak* ablakok, SDL_Color* szinek);
void texturak_torlese(Ablak* ablakok);
TTF_Font* betutipus_betoltese(char* nev, int meret);
void betutipusok_bezarasa(Betutipusok* bt);
SDL_Color* szinek_letrehozasa();

int main(void) {
    Ablak* ablakok = NULL;

    ablakok = (Ablak*) malloc(4 * sizeof(Ablak)); if (!ablakok) printf("Nem sikerult az ablakoknak memoriat foglalni :c");
    
    SDL_Window *menu_ablak=NULL, *jatek_ablak=NULL, *sugo_ablak=NULL, *dics_lista_ablak=NULL;
    SDL_Renderer *menu_megj=NULL, *jatek_megj=NULL, *sugo_megj=NULL, *dics_lista_megj=NULL;
    SDL_Texture *menu_logo=NULL, *jatek_logo=NULL, *sugo_logo=NULL, *dics_lista_logo=NULL;
    
    ablakok[MENU] = (Ablak) { menu_ablak, menu_megj, 840, 360, "Schatacka - Menu", MENU, menu_logo};
    ablakok[JATEK] = (Ablak) { jatek_ablak, jatek_megj, 1280, 960, "Schatacka - Jatek", JATEK, jatek_logo};
    ablakok[SUGO] = (Ablak) { sugo_ablak, sugo_megj, 1000, 500, "Schatacka - Sugo", SUGO, sugo_logo};
    ablakok[DICS_LISTA] = (Ablak) { dics_lista_ablak, dics_lista_megj, 1000, 500, "Schatacka - Dicsoseg Lista", DICS_LISTA, dics_lista_logo};

    Betutipusok bt = {NULL, NULL, NULL, NULL};
    SDL_Color* szinek = szinek_letrehozasa();

    inicializalas(ablakok, &bt, szinek);
    
    
    /* TEST - Sugo ablak */
    //ablakot_letrehoz(&ablakok[SUGO]);
    //SDL_RenderPresent(&ablakok[SUGO]->megjelenito);

    
    //Atmeneti - torold ki!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    /* az elvegzett rajzolasok a kepernyore */
    SDL_RenderPresent(ablakok[MENU].megjelenito);
    
    /* varunk a kilepesre */
    SDL_Event ev;
    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT) {
        /* SDL_RenderPresent(renderer); - MacOS Mojave esetén */
    }
    // EDDIG!!!

    
    
    
    /* KILEPES */
    texturak_torlese(ablakok);
    betutipusok_bezarasa(&bt);
    SDL_Quit();
    felszabadit(ablakok, szinek);

    return 0;
}

void inicializalas(Ablak* ablakok, Betutipusok* bt, SDL_Color* szinek) {
    /* SDL inicializalasa */
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    
    ablakot_letrehoz(&ablakok[MENU]);

    /* Betutipus betoltese, 20 pont magassaggal */
    TTF_Init();
    bt->med20 = betutipus_betoltese("OpenSans-Medium.ttf", 20);
    bt->ita20 = betutipus_betoltese("OpenSans-Italic.ttf", 20);
    bt->reg20 = betutipus_betoltese("OpenSans-Regular.ttf", 20);
    bt->bold20 = betutipus_betoltese("OpenSans-Bold.ttf", 20);

    fix_menut_kirajzol(ablakok, bt, szinek);



    /* !!!OPCIONÁLIS!!! */
    szoveget_kiir("Kiválasztva", 670, 115, szinek[PIROS], szinek[SZURKE], bt->bold20, ablakok[MENU].megjelenito, true);
    szoveget_kiir("Kiválasztva", 670, 165, szinek[ROZSA], szinek[SZURKE], bt->bold20, ablakok[MENU].megjelenito, true);
    szoveget_kiir("Kiválasztva", 670, 215, szinek[ZOLD], szinek[SZURKE], bt->bold20, ablakok[MENU].megjelenito, true);
    szoveget_kiir("Kiválasztva", 670, 265, szinek[KEK], szinek[SZURKE], bt->bold20, ablakok[MENU].megjelenito, true);

    szoveget_kiir("Normál mód (F1)", 70, 130, szinek[FEHER], szinek[SZURKE], bt->bold20, ablakok[MENU].megjelenito, true);
    szoveget_kiir("Fal nélküli (F2)", 70, 190, szinek[FEHER], szinek[SZURKE], bt->reg20, ablakok[MENU].megjelenito, true);
    szoveget_kiir("Felvehető elemek tiltása (F3)", 70, 250, szinek[FEHER], szinek[SZURKE], bt->reg20, ablakok[MENU].megjelenito, true);
    
}

void felszabadit(Ablak* ablakok, SDL_Color* szinek) {
    free(ablakok);
    free(szinek);
}

void texturak_torlese(Ablak* ablakok) {
    for (int i=0; i<4; ++i) {
        SDL_DestroyTexture(ablakok[i].logo);
    }
}

void betutipusok_bezarasa(Betutipusok* bt) {
    TTF_CloseFont(bt->bold20);
    TTF_CloseFont(bt->ita20);
    TTF_CloseFont(bt->reg20);
    TTF_CloseFont(bt->bold20);
}

TTF_Font* betutipus_betoltese(char* nev, int meret) {
    TTF_Font* betutipus = TTF_OpenFont(nev, meret);
    if (!betutipus) {
        SDL_Log("Nem sikerult megnyitni a betutipust! %s\n", TTF_GetError());
        exit(1);
    }
    return betutipus;
}

SDL_Color* szinek_letrehozasa() {
    SDL_Color* szinek = (SDL_Color*) malloc(7 * sizeof(SDL_Color));
    if (!szinek) printf("Nem sikerult a szineknek memoriat foglalni :c\n");

    szinek[FEHER] = (SDL_Color) {255, 255, 255};
    szinek[FEKETE] = (SDL_Color) {0, 0, 0};
    szinek[PIROS] = (SDL_Color) {255, 0, 0};
    szinek[ROZSA] = (SDL_Color) {255, 0, 255};
    szinek[ZOLD] = (SDL_Color) {0, 255, 0};
    szinek[KEK] = (SDL_Color) {0, 0, 255};
    szinek[SZURKE] = (SDL_Color) {30, 30, 30};

    return szinek;
}