#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include "ablak_kezelo.h"
#include "jatek_vezerlo.h"
#include "debugmalloc.h"

void inicializalas();
void felszabadit(Ablak* ablakok);
void texturak_torlese(Ablak* ablakok);

int main(void) {
    Ablak* ablakok = NULL;

    ablakok = (Ablak*) malloc(4 * sizeof(Ablak));        
    
    SDL_Window *menu_ablak=NULL, *jatek_ablak=NULL, *sugo_ablak=NULL, *dics_lista_ablak=NULL;
    SDL_Renderer *menu_megj=NULL, *jatek_megj=NULL, *sugo_megj=NULL, *dics_lista_megj=NULL;
    SDL_Texture *menu_logo=NULL, *jatek_logo=NULL, *sugo_logo=NULL, *dics_lista_logo=NULL;
    
    ablakok[MENU] = (Ablak) { menu_ablak, menu_megj, 1000, 500, "Schatacka - Menu", MENU, menu_logo};
    ablakok[JATEK] = (Ablak) { jatek_ablak, jatek_megj, 1280, 960, "Schatacka - Jatek", JATEK, jatek_logo};
    ablakok[SUGO] = (Ablak) { sugo_ablak, sugo_megj, 1000, 500, "Schatacka - Sugo", SUGO, sugo_logo};
    ablakok[DICS_LISTA] = (Ablak) { dics_lista_ablak, dics_lista_megj, 1000, 500, "Schatacka - Dicsoseg Lista", DICS_LISTA, dics_lista_logo};

    inicializalas(ablakok);
    
    
    
    //Atmeneti - torold ki!!!!!!!!
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
    SDL_Quit();
    felszabadit(ablakok);

    return 0;
}

void inicializalas(Ablak* ablakok) {
    /* SDL inicializalasa */
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    
    ablakot_letrehoz(&ablakok[MENU]);

    logot_rajzol(&ablakok[MENU], 50, 50);

}

void felszabadit(Ablak* ablakok) {
    free(ablakok);
}

void texturak_torlese(Ablak* ablakok) {
    for (int i=0; i<4; ++i) {
        SDL_DestroyTexture(ablakok[i].logo);
    }
}