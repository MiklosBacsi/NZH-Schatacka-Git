#include "bemenet_es_fajlkezeles.h"
#include "SDL2/SDL.h"
#include "debugmalloc.h"
#include <stdbool.h>
//#include "SDL2/SDL_keyboard.h"

#define MENU_ID 0
#define JATEK_ID 1
#define SUGO_ID 2
#define DICS_ID 3

void billentyuk_erzekelese(SDL_Event* event, Billentyuk* bill, Uint32* ablakIDk) {  
    /* Ahhoz kell, hogy allando kepfrissites legyen */
    if (event->type == SDL_USEREVENT)
        bill->van_bemenet = true;
    
    /****** Billentyuk ******/
    // Esc
    if (event->key.keysym.sym == SDLK_ESCAPE) {
        // Felenged
        if (event->type == SDL_KEYUP) {
            bill->menu_Esc = false;
            bill->sugo_Esc = false;
            bill->dics_Esc = false;
        
            // JATEK + TILTAS
            if (event->key.windowID == ablakIDk[JATEK_ID]) {
                bill->jatek_Esc = false;
                bill->tilt_Esc = false;
            }
        }

        // Lenyomas
        if (event->type == SDL_KEYDOWN) {
            if (event->key.windowID == ablakIDk[MENU_ID])
                bill->menu_Esc = true;
            else if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_Esc = true;
            else if (event->key.windowID == ablakIDk[SUGO_ID])
                bill->sugo_Esc = true;
            else if (event->key.windowID == ablakIDk[DICS_ID])
                bill->dics_Esc = true;
        }
    }

    // F1 - MENU
    if (event->key.keysym.sym == SDLK_F1 && event->key.windowID == ablakIDk[MENU_ID]) {
        if (event->type == SDL_KEYUP)
            bill->menu_F1 = false;
        else if (event->type == SDL_KEYDOWN)
            bill->menu_F1 = true;
    }

    // F2 - MENU
    if (event->key.keysym.sym == SDLK_F2 && event->key.windowID == ablakIDk[MENU_ID]) {
        if (event->type == SDL_KEYUP)
            bill->menu_F2 = false;
        else if (event->type == SDL_KEYDOWN)
            bill->menu_F2 = true;
    }

    // F3 - MENU
    if (event->key.keysym.sym == SDLK_F3 && event->key.windowID == ablakIDk[MENU_ID]) {
        if (event->type == SDL_KEYUP)
            bill->menu_F3 = false;
        else if (event->type == SDL_KEYDOWN)
            bill->menu_F3 = true;
    }

    // F10 - MENU
    if (event->key.keysym.sym == SDLK_F10 && event->key.windowID == ablakIDk[MENU_ID]) {
        if (event->type == SDL_KEYUP)
            bill->menu_F10 = false;
        else if (event->type == SDL_KEYDOWN)
            bill->menu_F10 = true;
    }

    // F11 - MENU
    if (event->key.keysym.sym == SDLK_F11 && event->key.windowID == ablakIDk[MENU_ID]) {
        if (event->type == SDL_KEYUP)
            bill->menu_F11 = false;
        else if (event->type == SDL_KEYDOWN)
            bill->menu_F11 = true;
    }

    /* Szokoz */
    if (event->key.keysym.sym == SDLK_SPACE) {
        // Felenged
        if (event->type == SDL_KEYUP) {
            // MENU
            if (event->key.windowID == ablakIDk[MENU_ID])
                bill->menu_Szokoz = false;
            // JATEK
            if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_Szokoz = false;
        }
        // Lenyom
        else if (event->type == SDL_KEYDOWN) {
            // MENU
            if (event->key.windowID == ablakIDk[MENU_ID])
                bill->menu_Szokoz = true;
            // JATEK
            if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_Szokoz = true;
        }
    }


    /* Piros */
    // 'A'
    if (event->key.keysym.sym == SDLK_a) {
        // Felenged
        if (event->type == SDL_KEYUP) {
            // MENU + TILTAS
            if (event->key.windowID == ablakIDk[MENU_ID]) {
                bill->menu_A = false;
                bill->menu_tilt_A = false;
            }
            // JATEK
            if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_A = false;
        }
        // Lenyom
        else if (event->type == SDL_KEYDOWN) {
            // MENU
            if (event->key.windowID == ablakIDk[MENU_ID])
                bill->menu_A = true;
            // JATEK
            if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_A = true;
        }
    }

    // 'W'
    if (event->key.keysym.sym == SDLK_w && event->key.windowID == ablakIDk[JATEK_ID]) {
        // Felenged - TILTAS
        if (event->type == SDL_KEYUP) {
            bill->jatek_W = false;
            bill->tilt_W = false;
        }
        // Lenyom
        else if (event->type == SDL_KEYDOWN)
            bill->jatek_W = true;
    }

    // 'D'
    if (event->key.keysym.sym == SDLK_d && event->key.windowID == ablakIDk[JATEK_ID]) {
        // Felenged
        if (event->type == SDL_KEYUP)
            bill->jatek_D = false;
        // Lenyom
        else if (event->type == SDL_KEYDOWN)
            bill->jatek_D = true;
    }


    /* Zold */
    // 'J'
    if (event->key.keysym.sym == SDLK_j) {
        // Felenged
        if (event->type == SDL_KEYUP) {
            // MENU + TILTAS
            if (event->key.windowID == ablakIDk[MENU_ID]) {
                bill->menu_J = false;
                bill->menu_tilt_J = false;
            }
            // JATEK
            if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_J = false;
        }
        // Lenyom
        else if (event->type == SDL_KEYDOWN) {
            // MENU
            if (event->key.windowID == ablakIDk[MENU_ID])
                bill->menu_J = true;
            // JATEK
            if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_J = true;
        }
    }

    // 'I'
    if (event->key.keysym.sym == SDLK_i && event->key.windowID == ablakIDk[JATEK_ID]) {
        // Felenged - TILTAS
        if (event->type == SDL_KEYUP) {
            bill->jatek_I = false;
            bill->tilt_I = false;
        }
        // Lenyom
        else if (event->type == SDL_KEYDOWN)
            bill->jatek_I = true;
    }

    // 'L'
    if (event->key.keysym.sym == SDLK_l && event->key.windowID == ablakIDk[JATEK_ID]) {
        // Felenged
        if (event->type == SDL_KEYUP)
            bill->jatek_L = false;
        // Lenyom
        else if (event->type == SDL_KEYDOWN)
            bill->jatek_L = true;
    }


    /* Kek */
    // <-
    if (event->key.keysym.sym == SDLK_LEFT) {
        // Felenged
        if (event->type == SDL_KEYUP) {
            // MENU + TILTAS
            if (event->key.windowID == ablakIDk[MENU_ID]) {
                bill->menu_Bal = false;
                bill->menu_tilt_Bal = false;
            }
            // JATEK
            if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_Bal = false;
        }
        // Lenyom
        else if (event->type == SDL_KEYDOWN) {
            // MENU
            if (event->key.windowID == ablakIDk[MENU_ID])
                bill->menu_Bal = true;
            // JATEK
            if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_Bal = true;
        }
    }

    // Fel
    if (event->key.keysym.sym == SDLK_UP && event->key.windowID == ablakIDk[JATEK_ID]) {
        // Felenged - TILTAS
        if (event->type == SDL_KEYUP) {
            bill->jatek_Fel = false;
            bill->tilt_Fel = false;
        }
        // Lenyom
        else if (event->type == SDL_KEYDOWN)
            bill->jatek_Fel = true;
    }

    // ->
    if (event->key.keysym.sym == SDLK_RIGHT && event->key.windowID == ablakIDk[JATEK_ID]) {
        // Felenged
        if (event->type == SDL_KEYUP)
            bill->jatek_Jobb = false;
        // Lenyom
        else if (event->type == SDL_KEYDOWN)
            bill->jatek_Jobb = true;
    }


    /* Rozsa */ 
    // '7'
    if (event->key.keysym.sym == SDLK_KP_7) {
        // Felenged
        if (event->type == SDL_KEYUP) {
            // MENU + TILTAS
            if (event->key.windowID == ablakIDk[MENU_ID]) {
                bill->menu_7 = false;
                bill->menu_tilt_7 = false;
            }
            // JATEK
            if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_7 = false;
        }
        // Lenyom
        else if (event->type == SDL_KEYDOWN) {
            // MENU
            if (event->key.windowID == ablakIDk[MENU_ID])
                bill->menu_7 = true;
            // JATEK
            if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_7 = true;
        }
    }

    // '/'
    if (event->key.keysym.sym == SDLK_KP_DIVIDE && event->key.windowID == ablakIDk[JATEK_ID]) {
        // Felenged - TILTAS
        if (event->type == SDL_KEYUP) {
            bill->jatek_Per = false;
            bill->tilt_Per = false;
        }
        // Lenyom
        else if (event->type == SDL_KEYDOWN)
            bill->jatek_Per = true;
    }

    // '9'
    if (event->key.keysym.sym == SDLK_KP_9 && event->key.windowID == ablakIDk[JATEK_ID]) {
        // Felenged
        if (event->type == SDL_KEYUP)
            bill->jatek_9 = false;
        // Lenyom
        else if (event->type == SDL_KEYDOWN)
            bill->jatek_9 = true;
    }
}

void billentyuk_tiltasa(Billentyuk* bill) {
    /* Menu - kivalasztas */
    if (bill->menu_A) bill->menu_tilt_A = true;
    if (bill->menu_J) bill->menu_tilt_J = true;
    if (bill->menu_Bal) bill->menu_tilt_Bal = true;
    if (bill->menu_7) bill->menu_tilt_7 = true;
    
    /* Jatek - Esc */
    if (bill->jatek_Esc) bill->tilt_Esc = true;
}