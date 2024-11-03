#ifndef JATEK_VEZERLO_H
#define JATEK_VEZERLO_H

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "bemenet_es_fajlkezeles.h"
#include "ablak_kezelo.h"
#include <stdbool.h>
#include <math.h>
#include "debugmalloc.h"

typedef enum Jatekmod {NORMAL, FAL_NELKULI, FELV_E_T} Jatekmod;

typedef struct Koordinata {
    double x;
    double y;
} Koordinata;

typedef struct Pixel {
    short x; short y;
} Pixel;

typedef struct Lovedek {
    Koordinata kp;
    double irany;
    double sugar;
} Lovedek;

//typedef enum SpecialisElem {NINCS = 0, NAGY_L, SOK_L, PAJZS} SpecialisElem;



/*
typedef struct Animacio {
    SDL_Texture* kep;
    Koordinata kp;
    double elet_hossz;
} Animacio;
*/

typedef struct Jatekos {
    Szin szin;
    Koordinata fej;
    double irany;
    //SpecialisElem spec_elem;
    bool* bal; bool* lo; bool* tilt_lo; bool* jobb;
    Pixel* vonal;
    bool eletben_van;
    int pontszam;
} Jatekos;


typedef struct Vezerles
{
    bool megallitva_jatek; bool megallitva_felhasznalo;
    double menetido;
    int jatekosszam; int max_pontszam; Jatekmod jt_mod;
    Pixel* palya;
    Lovedek* lovedekek;
    //FelvehetoElemek felv_e_K;
    //Animacio* animaciok;

} Vezerles;


void jatek_ablak_kezelese(Billentyuk* bill, Ablak* ablak, Vezerles* vez, bool* kiv_jat);

int aktiv_jatekosok_szama(bool* kiv_jatekosok);

void jatek_hatteret_kirajzol(Ablak* jatek_ablak);

#endif