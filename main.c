#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdlib.h>
#include "ablak_kezelo.h"

void inicializalas();

int main(void) {
    inicializalas();

    return 0;
}

void inicializalas(void) {
    /* SDL inicializálása */
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }

    

}