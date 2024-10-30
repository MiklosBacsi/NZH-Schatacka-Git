#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdlib.h>
#include "ablak_kezelo.h"
#include "jatek_vezerlo.h"
#include "debugmalloc.h"

void inicializalas();
void felszabadit(Ablak* ablakok);

int main(void) {
    Ablak* ablakok = NULL;

    ablakok = (Ablak*) malloc(4 * sizeof(Ablak));        
    
    SDL_Window *menu_ablak=NULL, *jatek_ablak=NULL, *sugo_ablak=NULL, *dics_lista_ablak=NULL;
    SDL_Renderer *menu_megj=NULL, *jatek_megj=NULL, *sugo_megj=NULL, *dics_lista_megj=NULL;
    
    ablakok[MENU] = (Ablak) { menu_ablak, menu_megj, 1000, 500, "Schatacka - Menu", MENU};
    ablakok[JATEK] = (Ablak) { jatek_ablak, jatek_megj, 1280, 960, "Schatacka - Jatek", JATEK};
    ablakok[SUGO] = (Ablak) { sugo_ablak, sugo_megj, 1000, 500, "Schatacka - Sugo", SUGO};
    ablakok[DICS_LISTA] = (Ablak) { dics_lista_ablak, dics_lista_megj, 1000, 500, "Schatacka - Dicsoseg Lista", DICS_LISTA};

    inicializalas(ablakok);
    
    
    
    //Átmeneti - töröld ki!!!
    /* az elvegzett rajzolasok a kepernyore */
    SDL_RenderPresent(ablakok[MENU].megjelenito);
    
    /* varunk a kilepesre */
    SDL_Event ev;
    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT) {
        /* SDL_RenderPresent(renderer); - MacOS Mojave esetén */
    }
    // EDDIG!!!

    /* Ablak bezarasa */
    SDL_Quit();
    felszabadit(ablakok);

    return 0;
}

void inicializalas(Ablak* ablakok) {
    /* SDL inicializálása */
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    
    ablakot_letrehoz(&ablakok[MENU]);
    
    /* Menu megnyitása
    ablakok[MENU].ablak = SDL_CreateWindow("Schatacka - Menu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ablakok[MENU].szelesseg, ablakok[MENU].magassag, 0);
    if (ablakok[MENU].ablak == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }

    ablakok[MENU].megjelenito = SDL_CreateRenderer(ablakok[MENU].ablak, -1, SDL_RENDERER_SOFTWARE);
    if (ablakok[MENU].megjelenito == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(ablakok[MENU].megjelenito);
    */
}

void felszabadit(Ablak* ablakok) {
    free(ablakok);
}