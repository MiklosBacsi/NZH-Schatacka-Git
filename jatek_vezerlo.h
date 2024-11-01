#ifndef JATEK_VEZERLO_H
#define JATEK_VEZERLO_H

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include <stdbool.h>
#include "debugmalloc.h"

typedef enum Irany {BAL = 0, LO, JOBB} Irany;

typedef struct Jatekos {
    Szin szin;
    bool aktiv;
} Jatekos;

typedef struct Billentyuk {
    bool menu_Esc; bool jatek_Esc; bool sugo_Esc; bool dics_Esc;
    bool menu_F1; bool menu_F2; bool menu_F3; bool menu_F10; bool menu_F11;
    bool jatek_Q; bool jatek_2; bool jatek_W;
    bool jatek_M; bool jatek_K; bool jatek_Vesszo;
    bool jatek_Bal; bool jatek_Fel; bool jatek_Jobb;
    bool jatek_Per; bool jatek_Csillag; bool jatek_Kotojel;
} Billentyuk;




#endif