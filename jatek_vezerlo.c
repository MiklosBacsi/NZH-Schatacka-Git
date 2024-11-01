#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "ablak_kezelo.h"
#include "jatek_vezerlo.h"
#include <stdbool.h>
#include "debugmalloc.h"


void billentyuk_erzekelese(SDL_Event* event, Billentyuk* bill, Ablak* ablakok) {
    switch (event->type) {
            /* Felengedett billentyuk */
            case SDL_KEYUP:
                switch (event->key.keysym.sym) {
                    // Esc
                    case SDLK_ESCAPE:
                        // Mert nem mindig erzekeli jol az ablakot (ablak pointer mar nem letezik), es funkcionalitasban nem valtoztat
                        bill->menu_Esc = false;
                        bill->jatek_Esc = false;
                        bill->sugo_Esc = false;
                        bill->dics_Esc = false;
                        break;                    
                    // F1
                    case SDLK_F1: if (event->key.windowID == SDL_GetWindowID(ablakok[MENU].ablak)) bill->menu_F1 = false; break;
                    // F2
                    case SDLK_F2: if (event->key.windowID == SDL_GetWindowID(ablakok[MENU].ablak)) bill->menu_F2 = false; break;
                    // F3
                    case SDLK_F3: if (event->key.windowID == SDL_GetWindowID(ablakok[MENU].ablak)) bill->menu_F3 = false; break;
                    // F10
                    case SDLK_F10: if (event->key.windowID == SDL_GetWindowID(ablakok[MENU].ablak)) bill->menu_F10 = false; break;
                    // F11
                    case SDLK_F11: if (event->key.windowID == SDL_GetWindowID(ablakok[MENU].ablak)) bill->menu_F11 = false; break;
                    
                    // Mas billentyuk
                    //case SDLK_LEFT: left = false; rajz = true; break;
                    //case SDLK_RIGHT: right = false; rajz = true; break;
                    
                }
                break;

            /* Lenyomott billentyuk */                
            case SDL_KEYDOWN:
                switch (event->key.keysym.sym) {
                    case SDLK_ESCAPE:
                        if (event->key.windowID == SDL_GetWindowID(ablakok[MENU].ablak))
                            bill->menu_Esc = true;
                        else if (event->key.windowID == SDL_GetWindowID(ablakok[JATEK].ablak))
                            bill->jatek_Esc = true;
                        else if (event->key.windowID == SDL_GetWindowID(ablakok[SUGO].ablak))
                            bill->sugo_Esc = true;
                        else if (event->key.windowID == SDL_GetWindowID(ablakok[DICS_LISTA].ablak))
                            bill->dics_Esc = true;
                        break;
                    // F1
                    case SDLK_F1: if (event->key.windowID == SDL_GetWindowID(ablakok[MENU].ablak)) bill->menu_F1 = true; break;
                    // F2
                    case SDLK_F2: if (event->key.windowID == SDL_GetWindowID(ablakok[MENU].ablak)) bill->menu_F2 = true; break;
                    // F3
                    case SDLK_F3: if (event->key.windowID == SDL_GetWindowID(ablakok[MENU].ablak)) bill->menu_F3 = true; break;
                    // F10
                    case SDLK_F10: if (event->key.windowID == SDL_GetWindowID(ablakok[MENU].ablak)) bill->menu_F10 = true; break;
                    // F11
                    case SDLK_F11: if (event->key.windowID == SDL_GetWindowID(ablakok[MENU].ablak)) bill->menu_F11 = true; break;
                    
                    // Mas billentyuk
                    //case SDLK_RIGHT: right = true; rajz = true; break;
                }
                break;
            
            case SDL_QUIT:
                /* Nem lehet bezarni az ablakot kiikszelessel (csak ha egy van nyitva)
                 * window, button, key, user stb. <- nem mukodnek :c*/
                bill->menu_Esc = true;
                break;
        }
}