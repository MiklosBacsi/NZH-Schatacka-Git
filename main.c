#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdlib.h>
#include "ablak_kezelo.h"

void inicializalas();
void felszabadit(Ablak* ablakok);

int main(void) {
    Ablak* ablakok;
    
    inicializalas(ablakok);


    felszabadit(ablakok);

    return 0;
}

void inicializalas(Ablak* ablakok) {
    /* SDL inicializálása */
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }

    ablakok = (Ablak*) malloc(5 * sizeof(Ablak));

    SDL_Window *menu_ablak;
    SDL_Renderer *menu_megjelenito;
    ablakok[MENU] = (Ablak) { menu_ablak, menu_megjelenito, 1000, 500, (Ablak_tipus) MENU};
    

}

void felszabadit(Ablak* ablakok) {
    free(ablakok);
}