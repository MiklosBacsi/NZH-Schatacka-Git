#include "bemenet_es_fajlkezeles.h"
#include "SDL2/SDL.h"
#include "ablak_kezelo.h"
#include "debugmalloc.h"
#include <stdbool.h>



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

                    /* Szokoz */
                    case SDLK_SPACE:
                    // MENU
                        if (event->key.windowID == ablakIDk[0]) {
                            bill->menu_Szokoz = false;
                            break;
                        }
                    // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Szokoz = false;
                            break;
                        }
                    

                    /* Piros */                    
                    case SDLK_q:            // 'Q'
                        // MENU + TILTAS
                        if (event->key.windowID == ablakIDk[0]) {
                            bill->menu_Q = false;
                            bill->menu_tilt_Q = false;
                            break;
                        }
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Q = false;
                            break;
                        }
                    
                    case SDLK_2:            // '2'
                        // JATEK + TILTAS
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_2 = false;
                            bill->tilt_2 = false;
                            break;
                        }
                    
                    case SDLK_w:            // 'W'
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_W = false;
                            break;
                        }

                    /* Rozsa */                    
                    case SDLK_KP_DIVIDE:    // '/'
                        // MENU + TILTAS
                        if (event->key.windowID == ablakIDk[0]) {
                            bill->menu_Per = false;
                            bill->menu_tilt_Per = false;
                            break;
                        }
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Per = false;
                            break;
                        }
                    
                    case SDLK_KP_MULTIPLY:  // '*'
                        // JATEK + TILTAS
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Csillag = false;
                            bill->tilt_Csillag = false;
                            break;
                        }
                    
                    case SDLK_KP_MINUS:     // '-'
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Kotojel = false;
                            break;
                        }

                    /* Zold */
                    case SDLK_m:            // 'M'
                        // MENU + TILTAS
                        if (event->key.windowID == ablakIDk[0]) {
                            bill->menu_M = false;
                            bill->menu_tilt_M = false;
                            break;
                        }
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_M = false;
                            break;                               
                        }
                    
                    case SDLK_k:            // 'K'
                        // JATEK + TILTAS
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_K = false;
                            bill->tilt_K = false;
                            break;
                        }
                    
                    case SDLK_COMMA:        // ','
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Vesszo = false;
                            break;
                        }

                    /* Kek */
                    case SDLK_LEFT:         // <-
                        // MENU + TILTAS
                        if (event->key.windowID == ablakIDk[0]) {
                            bill->menu_Bal = false;
                            bill->menu_tilt_Bal = false;
                            break;
                        }
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Bal = false;
                            break;
                        }
                    
                    case SDLK_UP:           // Fel
                        // JATEK + TILTAS
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Fel = false;
                            bill->tilt_Fel = false;
                            break;
                        }
                    
                    case SDLK_RIGHT:        // ->
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Jobb = false;
                            break;
                        }
                }
                break;

            /* Lenyomott billentyuk */                
            case SDL_KEYDOWN:
                switch (event->key.keysym.sym) {
                    // Esc
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

                    /* Szokoz */
                    case SDLK_SPACE:
                    // MENU
                        if (event->key.windowID == ablakIDk[0]) {
                            bill->menu_Szokoz = true;
                            break;
                        }
                    // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Szokoz = true;
                            break;
                        }

                    
                    /* Piros */
                    case SDLK_q:            // 'Q'
                        // MENU
                        if (event->key.windowID == ablakIDk[0]) {
                            bill->menu_Q = true;
                            break;
                        }
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Q = true;
                            break;
                        }
                    case SDLK_2:            // '2'
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_2 = true;
                            break;
                        }
                    case SDLK_w:            // 'W'
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_W = true;
                            break;
                        }

                    /* Rozsa */
                    case SDLK_KP_DIVIDE:    // '/'
                        // MENU
                        if (event->key.windowID == ablakIDk[0]) {
                            bill->menu_Per = true;
                            break;
                        }
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Per = true;
                            break;
                        }
                    case SDLK_KP_MULTIPLY:  // '*'
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Csillag = true;
                            break;
                        }
                    case SDLK_KP_MINUS:     // '-'
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Kotojel = true;
                            break;
                        }

                    /* Zold */
                    case SDLK_m:            // 'M'
                        // MENU
                        if (event->key.windowID == ablakIDk[0]) {
                            bill->menu_M = true;
                            break;
                        }
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_M = true;
                            break;
                        }
                    case SDLK_k:            // 'K'
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_K = true;
                            break;
                        }
                    case SDLK_COMMA:        // ','
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Vesszo = true;
                            break;
                        }

                    /* Kek */
                    case SDLK_LEFT:         // <-
                        // MENU
                        if (event->key.windowID == ablakIDk[0]) {
                            bill->menu_Bal = true;
                            break;
                        }
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Bal = true;
                            break;
                        }
                    case SDLK_UP:           // Fel
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Fel = true;
                            break;
                        }
                    case SDLK_RIGHT:        // ->
                        // JATEK
                        if (event->key.windowID == ablakIDk[1]) {
                            bill->jatek_Jobb = true;
                            break;
                        }
                }
                break;
            
            case SDL_QUIT:
                /* Nem lehet bezarni az ablakot kiikszelessel (csak ha egy van nyitva)
                 * window, button, key, user stb. <- nem mukodnek :c*/
                bill->menu_Esc = true;
                break;
        }
}

void billentyuk_tiltasa(Billentyuk* bill) {
    /* Menu - kivalasztas */
    if (bill->menu_Q) bill->menu_tilt_Q = true;
    if (bill->menu_Per) bill->menu_tilt_Per = true;
    if (bill->menu_M) bill->menu_tilt_M = true;
    if (bill->menu_Bal) bill->menu_tilt_Bal = true;
    /* Jatek - loves */
    if (bill->jatek_2) bill->tilt_2 = true;
    if (bill->jatek_Csillag) bill->tilt_Csillag = true;
    if (bill->jatek_K) bill->tilt_K = true;
    if (bill->jatek_Fel) bill->tilt_Fel = true;
}