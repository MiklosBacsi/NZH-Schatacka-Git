#ifndef BEMENET_ES_FAJLKEZELES_H
#define BEMENET_ES_FAJLKEZELES_H

#include "SDL2/SDL.h"
#include "debugmalloc.h"
#include <stdbool.h>

typedef struct Billentyuk {
    bool menu_Esc; bool jatek_Esc; bool sugo_Esc; bool dics_Esc;
    bool menu_F1; bool menu_F2; bool menu_F3; bool menu_F10; bool menu_F11;
    bool jatek_Q; bool jatek_2; bool jatek_W;
    bool jatek_M; bool jatek_K; bool jatek_Vesszo;
    bool jatek_Bal; bool jatek_Fel; bool jatek_Jobb;
    bool jatek_Per; bool jatek_Csillag; bool jatek_Kotojel;
    bool menu_Szokoz; bool jatek_Szokoz;
    // A billentyuk egyszeri megnyomasahoz
    bool tilt_2; bool tilt_Csillag; bool tilt_K; bool tilt_Fel; bool tilt_Esc;
    bool menu_Q; bool menu_Per; bool menu_M; bool menu_Bal;
    bool menu_tilt_Q; bool menu_tilt_Per; bool menu_tilt_M; bool menu_tilt_Bal;
} Billentyuk;

void billentyuk_erzekelese(SDL_Event* event, Billentyuk* bill, Uint32* ablakIDk);

void billentyuk_tiltasa(Billentyuk* bill);

#endif