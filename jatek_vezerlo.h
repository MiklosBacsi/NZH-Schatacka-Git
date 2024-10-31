#ifndef JATEK_VEZERLO_H
#define JATEK_VEZERLO_H

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "debugmalloc.h"

typedef enum Irany {BAL = 0, LO, JOBB} Irany;

typedef struct Jatekos {
    Szin szin;
    bool aktiv;
} Jatekos;



#endif