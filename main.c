#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include "ablak_kezelo.h"
#include "jatek_vezerlo.h"
#include "debugmalloc.h"

void inicializalas();
void felszabadit(Ablak* ablakok);
void texturak_torlese(Ablak* ablakok);
TTF_Font* betutipus_betoltese(char* nev, int meret);
void betutipusok_bezarasa(Betutipusok* bt);

int main(void) {
    Ablak* ablakok = NULL;

    ablakok = (Ablak*) malloc(4 * sizeof(Ablak));
    
    SDL_Window *menu_ablak=NULL, *jatek_ablak=NULL, *sugo_ablak=NULL, *dics_lista_ablak=NULL;
    SDL_Renderer *menu_megj=NULL, *jatek_megj=NULL, *sugo_megj=NULL, *dics_lista_megj=NULL;
    SDL_Texture *menu_logo=NULL, *jatek_logo=NULL, *sugo_logo=NULL, *dics_lista_logo=NULL;
    
    ablakok[MENU] = (Ablak) { menu_ablak, menu_megj, 840, 360, "Schatacka - Menu", MENU, menu_logo};
    ablakok[JATEK] = (Ablak) { jatek_ablak, jatek_megj, 1280, 960, "Schatacka - Jatek", JATEK, jatek_logo};
    ablakok[SUGO] = (Ablak) { sugo_ablak, sugo_megj, 1000, 500, "Schatacka - Sugo", SUGO, sugo_logo};
    ablakok[DICS_LISTA] = (Ablak) { dics_lista_ablak, dics_lista_megj, 1000, 500, "Schatacka - Dicsoseg Lista", DICS_LISTA, dics_lista_logo};

    Betutipusok bt = {NULL, NULL, NULL, NULL};

    inicializalas(ablakok, &bt);
    
    
    
    //Atmeneti - torold ki!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    /* az elvegzett rajzolasok a kepernyore */
    SDL_RenderPresent(ablakok[MENU].megjelenito);
    
    /* varunk a kilepesre */
    SDL_Event ev;
    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT) {
        /* SDL_RenderPresent(renderer); - MacOS Mojave esetén */
    }
    // EDDIG!!!

    
    
    
    /* Ablak bezarasa */
    texturak_torlese(ablakok);
    betutipusok_bezarasa(&bt);
    SDL_Quit();
    felszabadit(ablakok);

    return 0;
}

void inicializalas(Ablak* ablakok, Betutipusok* bt) {
    /* SDL inicializalasa */
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    
    ablakot_letrehoz(&ablakok[MENU]);

    logot_rajzol(&ablakok[MENU], 50, 10);

    /* Bal teglalap */
    rectangleRGBA(ablakok[MENU].megjelenito, 20, 80, 330, 330, 255, 255, 255, 255);
    /* Jobb teglalap */
    rectangleRGBA(ablakok[MENU].megjelenito, 350, 80, 820, 330, 255, 255, 255, 255);


    /* Betutipus betoltese, 20 pont magassaggal */
    TTF_Init();
    bt->reg20 = betutipus_betoltese("OpenSans-Regular.ttf", 20);
    bt->bold20 = betutipus_betoltese("OpenSans-Bold.ttf", 20);



    //ddddddddddddddddddddddddddddddddddd@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    SDL_Rect hova = { 0, 0, 0, 0 };
    SDL_Color feher = {255, 255, 255};//, piros = {255, 0, 0};


    // DEMO -- TESZT -- EZT TÖRÖLD KIIIIII !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    SDL_Surface* felirat;
    SDL_Texture* felirat_t;
    
    felirat = TTF_RenderUTF8_Blended(bt->reg20, "TTF_RenderUTF8_Blended()", feher);

    felirat_t = SDL_CreateTextureFromSurface(ablakok[MENU].megjelenito, felirat);
    
    printf("Felirat->w: %d\nKiscicaaaaaaaaaaaaaaaaaaaaaaaaa\n", felirat->w);
    
    hova.x = (480 - felirat->w) / 2;
    hova.y = 100;                               
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(ablakok[MENU].megjelenito, felirat_t, NULL, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
    //EDDIG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!





    /* ha sajat kodban hasznalod, csinalj belole fuggvenyt! !!!!!!!!!!!!!!!!!!!!!!!!!!*/
    /*
    bt->felirat = TTF_RenderUTF8_Shaded(bt->reg20, "Jatekmod", feher, piros);
    bt->felirat_t = SDL_CreateTextureFromSurface(ablakok[MENU].megjelenito, bt->felirat);
    
    hova.x = 300;//(480 - bt->felirat->w) / 2;
    hova.y = 60;
    hova.w = 30;//bt->felirat->w;
    hova.h = 20;//bt->felirat->h;
    
    SDL_RenderCopy(ablakok[MENU].megjelenito, bt->felirat_t, NULL, &hova);
    SDL_FreeSurface(bt->felirat);
    SDL_DestroyTexture(bt->felirat_t);
    */
}

void felszabadit(Ablak* ablakok) {
    free(ablakok);
}

void texturak_torlese(Ablak* ablakok) {
    for (int i=0; i<4; ++i) {
        SDL_DestroyTexture(ablakok[i].logo);
    }
}

void betutipusok_bezarasa(Betutipusok* bt) {
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