#ifndef JATEK_VEZERLO_H
#define JATEK_VEZERLO_H

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "bemenet_es_fajlkezeles.h"
#include "ablak_kezelo.h"
#include <stdbool.h>
#include "debugmalloc.h"

typedef struct Vezerles
{
    bool megallitva_jatek; bool megallitva_felhasznalo;
} Vezerles;


void jatek_ablak_kezelese(Billentyuk* bill, Ablak* ablak, Vezerles* vez, bool* kiv_jat);

int aktiv_jatekosok_szama(bool* kiv_jatekosok);



#endif