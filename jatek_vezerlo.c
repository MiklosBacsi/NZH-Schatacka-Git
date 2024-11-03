#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "bemenet_es_fajlkezeles.h"
#include "ablak_kezelo.h"
#include "jatek_vezerlo.h"
#include <stdbool.h>
#include "debugmalloc.h"

void jatek_ablak_kezelese(Billentyuk* bill, Ablak* jatek_ablak, Vezerles* vez) {
    /* Ablak megnyitasa */
    if (bill->menu_Szokoz && !jatek_ablak->nyitva) {
        ablakot_letrehoz(jatek_ablak);
        SDL_RenderPresent(jatek_ablak->megjelenito);
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
