#include "bemenet_es_fajlkezeles.h"
#include "SDL2/SDL.h"
#include "ablak_kezelo.h"
#include "debugmalloc.h"



void billentyuk_erzekelese(SDL_Event* event, Billentyuk* bill, Uint32* ablakIDk) {
    /* AlbakIDk: MENU=0, JATEK=1, SUGO=2, DICS_LISTA=3 */    
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
                    // F1 - MENU
                    case SDLK_F1: if (event->key.windowID == ablakIDk[0]) bill->menu_F1 = false; break;
                    // F2 - MENU
                    case SDLK_F2: if (event->key.windowID == ablakIDk[0]) bill->menu_F2 = false; break;
                    // F3 - MENU
                    case SDLK_F3: if (event->key.windowID == ablakIDk[0]) bill->menu_F3 = false; break;
                    // F10 - MENU
                    case SDLK_F10: if (event->key.windowID == ablakIDk[0]) bill->menu_F10 = false; break;
                    // F11 - MENU
                    case SDLK_F11: if (event->key.windowID == ablakIDk[0]) bill->menu_F11 = false; break;
                    
                    // Mas billentyuk
                    //case SDLK_LEFT: left = false; rajz = true; break;
                    //case SDLK_RIGHT: right = false; rajz = true; break;
                    
                }
                break;

            /* Lenyomott billentyuk */                
            case SDL_KEYDOWN:
                switch (event->key.keysym.sym) {
                    case SDLK_ESCAPE:
                        if (event->key.windowID == ablakIDk[0])         //MENU
                            bill->menu_Esc = true;
                        else if (event->key.windowID == ablakIDk[1])    //JATEK
                            bill->jatek_Esc = true;
                        else if (event->key.windowID == ablakIDk[2])    //SUGO
                            bill->sugo_Esc = true;
                        else if (event->key.windowID == ablakIDk[3])    //DICS_LISTA
                            bill->dics_Esc = true;
                        break;
                    // F1 - MENU
                    case SDLK_F1: if (event->key.windowID == ablakIDk[0]) bill->menu_F1 = true; break;
                    // F2 - MENU
                    case SDLK_F2: if (event->key.windowID == ablakIDk[0]) bill->menu_F2 = true; break;
                    // F3 - MENU
                    case SDLK_F3: if (event->key.windowID == ablakIDk[0]) bill->menu_F3 = true; break;
                    // F10 - MENU
                    case SDLK_F10: if (event->key.windowID == ablakIDk[0]) bill->menu_F10 = true; break;
                    // F11 - MENU
                    case SDLK_F11: if (event->key.windowID == ablakIDk[0]) bill->menu_F11 = true; break;
                    
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