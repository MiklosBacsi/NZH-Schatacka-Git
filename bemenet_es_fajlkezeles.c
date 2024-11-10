#include "bemenet_es_fajlkezeles.h"
#include "SDL2/SDL.h"
#include "debugmalloc.h"
#include <stdbool.h>
//#include "SDL2/SDL_keyboard.h"

#define MENU_ID 0
#define JATEK_ID 1
#define SUGO_ID 2
#define DICS_ID 3

void billentyuk_erzekelese(const SDL_Event* event, Billentyuk* bill, const Uint32* ablakIDk) {  
    /* Ahhoz kell, hogy allando kepfrissites legyen */
    if (event->type == SDL_USEREVENT)
        bill->van_bemenet = true;
    
    /* Felengedett billentyuk */
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
    // 'Q'
    if (event->key.keysym.sym == SDLK_q) {
        // Felenged
        if (event->type == SDL_KEYUP) {
            // MENU + TILTAS
            if (event->key.windowID == ablakIDk[MENU_ID]) {
                bill->menu_Q = false;
                bill->menu_tilt_Q = false;
            }
            // JATEK
            if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_Q = false;
        }
        // Lenyom
        else if (event->type == SDL_KEYDOWN) {
            // MENU
            if (event->key.windowID == ablakIDk[MENU_ID])
                bill->menu_Q = true;
            // JATEK
            if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_Q = true;
        }
    }

    // '2'
    if (event->key.keysym.sym == SDLK_2 && event->key.windowID == ablakIDk[JATEK_ID]) {
        // Felenged - TILTAS
        if (event->type == SDL_KEYUP) {
            bill->jatek_2 = false;
            bill->tilt_2 = false;
        }
        // Lenyom
        else if (event->type == SDL_KEYDOWN)
            bill->jatek_2 = true;
    }

    // 'W'
    if (event->key.keysym.sym == SDLK_w && event->key.windowID == ablakIDk[JATEK_ID]) {
        // Felenged
        if (event->type == SDL_KEYUP)
            bill->jatek_W = false;
        // Lenyom
        else if (event->type == SDL_KEYDOWN)
            bill->jatek_W = true;
    }


    /* Rozsa */ 
    // '/'
    if (event->key.keysym.sym == SDLK_KP_DIVIDE) {
        // Felenged
        if (event->type == SDL_KEYUP) {
            // MENU + TILTAS
            if (event->key.windowID == ablakIDk[MENU_ID]) {
                bill->menu_Per = false;
                bill->menu_tilt_Per = false;
            }
            // JATEK
            if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_Per = false;
        }
        // Lenyom
        else if (event->type == SDL_KEYDOWN) {
            // MENU
            if (event->key.windowID == ablakIDk[MENU_ID])
                bill->menu_Per = true;
            // JATEK
            if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_Per = true;
        }
    }

    // '*'
    if (event->key.keysym.sym == SDLK_KP_MULTIPLY && event->key.windowID == ablakIDk[JATEK_ID]) {
        // Felenged - TILTAS
        if (event->type == SDL_KEYUP) {
            bill->jatek_Csillag = false;
            bill->tilt_Csillag = false;
        }
        // Lenyom
        else if (event->type == SDL_KEYDOWN)
            bill->jatek_Csillag = true;
    }

    // '-'
    if (event->key.keysym.sym == SDLK_KP_MINUS && event->key.windowID == ablakIDk[JATEK_ID]) {
        // Felenged
        if (event->type == SDL_KEYUP)
            bill->jatek_Kotojel = false;
        // Lenyom
        else if (event->type == SDL_KEYDOWN)
            bill->jatek_Kotojel = true;
    }


    /* Zold */
    // 'M'
    if (event->key.keysym.sym == SDLK_m) {
        // Felenged
        if (event->type == SDL_KEYUP) {
            // MENU + TILTAS
            if (event->key.windowID == ablakIDk[MENU_ID]) {
                bill->menu_M = false;
                bill->menu_tilt_M = false;
            }
            // JATEK
            if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_M = false;
        }
        // Lenyom
        else if (event->type == SDL_KEYDOWN) {
            // MENU
            if (event->key.windowID == ablakIDk[MENU_ID])
                bill->menu_M = true;
            // JATEK
            if (event->key.windowID == ablakIDk[JATEK_ID])
                bill->jatek_M = true;
        }
    }

    // 'K'
    if (event->key.keysym.sym == SDLK_k && event->key.windowID == ablakIDk[JATEK_ID]) {
        // Felenged - TILTAS
        if (event->type == SDL_KEYUP) {
            bill->jatek_K = false;
            bill->tilt_K = false;
        }
        // Lenyom
        else if (event->type == SDL_KEYDOWN)
            bill->jatek_K = true;
    }

    // ','
    if (event->key.keysym.sym == SDLK_COMMA && event->key.windowID == ablakIDk[JATEK_ID]) {
        // Felenged
        if (event->type == SDL_KEYUP)
            bill->jatek_Vesszo = false;
        // Lenyom
        else if (event->type == SDL_KEYDOWN)
            bill->jatek_Vesszo = true;
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


    /* Kilepes */
    if (event->key.keysym.sym == SDL_QUIT)
    /* Nem lehet bezarni az ablakot kiikszelessel (csak ha egy van nyitva)
    * window, button, key, user stb. <- nem mukodnek :c*/
        bill->menu_Esc = true;
    // WINDOW_EVENT???????????????        
    
}

void billentyuk_tiltasa(Billentyuk* bill) {
    /* Menu - kivalasztas */
    if (bill->menu_Q) bill->menu_tilt_Q = true;
    if (bill->menu_Per) bill->menu_tilt_Per = true;
    if (bill->menu_M) bill->menu_tilt_M = true;
    if (bill->menu_Bal) bill->menu_tilt_Bal = true;
    
    /* Jatek - Esc */
    if (bill->jatek_Esc) bill->tilt_Esc = true;
}