#ifndef JATEK_VEZERLO_H
#define JATEK_VEZERLO_H

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "bemenet_es_fajlkezeles.h"
#include "ablak_kezelo.h"
#include <stdlib.h>
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

typedef struct Fal {
    Pixel pixel;
    bool torolve;
} Fal;


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
    double elet_tartam; //      = 3.0; koronkent: -= 0.02;
} Animacio;
*/

typedef struct Jatekos {
    Szin szin;
    Koordinata fej;
    double irany;
    //SpecialisElem spec_elem;
    bool* bal; bool* lo; bool* tilt_lo; bool* jobb;
    //Vonal* vonal;
    //Lemarado pointer a lyukakhoz
    bool eletben_van;
    int pontszam;
} Jatekos;


typedef struct Vezerles
{
    bool megallitva_jatek; bool megallitva_felhasznalo;
    double menetido; bool menet_vege;
    int jatekosszam; int max_pontszam; Jatekmod jt_mod;
    double elmozd_jat; double elmozd_lov; double fordulas;
    Fal* fal_vonal; int fal_db;
    Pixel palya_meret;
    Lovedek* lovedekek;
    //FelvehetoElemek felv_e_K;
    //Animacio* animaciok;

} Vezerles;


void jatek_ablak_kezelese(Billentyuk* bill, Ablak* jatek_ablak, Vezerles* vez, Jatekos** cim_jatekosok, int jt_mod, bool* kiv_jat);

void uj_menet(Vezerles* vez, Jatekos* jatekosok);

double tav(Koordinata k1, Koordinata k2);

void jatek_kirajzolasa(Ablak* jatek_ablak, Vezerles* vez, Jatekos* jatekosok, Betutipusok* bt);

Uint32 idozit(Uint32 ms, void *param);

void jatekosok_mozditasa(Jatekos* jatekosok, Vezerles* vez);

void halal_vizsgalata(Jatekos* jatekosok, Vezerles* vez);

#endif