#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "bemenet_es_fajlkezeles.h"
#include "ablak_kezelo.h"
#include "jatek_vezerlo.h"
#include <stdbool.h>
#include <math.h>
#include "debugmalloc.h"

void jatek_ablak_kezelese(Billentyuk* bill, Ablak* jatek_ablak, Vezerles* vez, bool* kiv_jat) {
    
    
    
    /* Ablak megnyitasa */
    if (bill->menu_Szokoz && !jatek_ablak->nyitva && aktiv_jatekosok_szama(kiv_jat) >= 2) {
        ablakot_letrehoz(jatek_ablak);
        
        /* JATEKOSOK LETREHOZASA */
        
    }
    /* Ablak bezarasa */
    else if (bill->jatek_Esc && vez->megallitva_felhasznalo && jatek_ablak->nyitva) {
        SDL_DestroyRenderer(jatek_ablak->megjelenito);
        SDL_DestroyWindow(jatek_ablak->ablak);
        jatek_ablak->ablak = NULL;
        jatek_ablak->megjelenito = NULL;
        
        jatek_ablak->nyitva = false;
        bill->jatek_Esc = false;
        vez->megallitva_felhasznalo = false;
        vez->megallitva_jatek = true;
    }
    /* Megallitas - Felhasznalo */
    else if (bill->jatek_Esc && !vez->megallitva_felhasznalo && jatek_ablak->nyitva) {
        vez->megallitva_felhasznalo = true;
    }
    /* Folytatas - Felhasznalo */
    else if (bill->jatek_Szokoz && vez->megallitva_felhasznalo && jatek_ablak->nyitva) {
        vez->megallitva_felhasznalo = false;
    }
}

void jatek_hatteret_kirajzol(Ablak* jatek_ablak) {
    boxRGBA(jatek_ablak->megjelenito, 0, 0, 1600, 900, 30, 30, 30, 255);
    boxRGBA(jatek_ablak->megjelenito, 1401, 0, 1402, 900, 255, 255, 255, 255);

    logot_rajzol(jatek_ablak, 1425, 10);



    SDL_RenderPresent(jatek_ablak->megjelenito);
}

int aktiv_jatekosok_szama(bool* kiv_jatekosok) {
    int db = 0;
    for (int i=0; i<4; ++i) {
        if (kiv_jatekosok[i]) ++db;
    }
    return db;
}