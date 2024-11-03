#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ablak_kezelo.h"
#include "jatek_vezerlo.h"
#include "bemenet_es_fajlkezeles.h"
#include "debugmalloc.h"

typedef struct Kivalasztas {
    int kiv_jt_mod;
    bool aktiv_jatekosok[4];
} Kivalasztas;

void inicializalas(Ablak* ablakok, Betutipusok* bt, SDL_Color* szinek);
void felszabadit(Ablak* ablakok, SDL_Color* szinek);
TTF_Font* betutipus_betoltese(char* nev, int meret);
void betutipusok_bezarasa(Betutipusok* bt);
SDL_Color* szinek_letrehozasa();
void menu_kivalasztas(Kivalasztas* kiv, Billentyuk* bill, Ablak* menu, Betutipusok* bt, SDL_Color* szinek);

int main(void) {
    Ablak* ablakok = NULL;

    ablakok = (Ablak*) malloc(4 * sizeof(Ablak)); if (!ablakok) printf("Nem sikerult az ablakoknak memoriat foglalni :c\n");
    
    SDL_Window *menu_ablak=NULL, *jatek_ablak=NULL, *sugo_ablak=NULL, *dics_lista_ablak=NULL;
    SDL_Renderer *menu_megj=NULL, *jatek_megj=NULL, *sugo_megj=NULL, *dics_lista_megj=NULL;
    SDL_Texture *menu_logo=NULL, *jatek_logo=NULL, *sugo_logo=NULL, *dics_lista_logo=NULL;
    
    ablakok[MENU] = (Ablak) { menu_ablak, menu_megj, 840, 360, "Schatacka - Menü", MENU, menu_logo, false};
    ablakok[JATEK] = (Ablak) { jatek_ablak, jatek_megj, 1600, 900, "Schatacka - Játék", JATEK, jatek_logo, false};
    ablakok[SUGO] = (Ablak) { sugo_ablak, sugo_megj, 800, 600, "Schatacka - Súgó", SUGO, sugo_logo, false};
    ablakok[DICS_LISTA] = (Ablak) { dics_lista_ablak, dics_lista_megj, 1000, 500, "Schatacka - Dicsőség Lista", DICS_LISTA, dics_lista_logo, false};

    Betutipusok bt = {NULL, NULL, NULL, NULL};
    SDL_Color* szinek = szinek_letrehozasa();
    Billentyuk bill = (Billentyuk) {false, false, false,false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
    Kivalasztas kiv = (Kivalasztas) { 0, {false, false, false, false} };

    inicializalas(ablakok, &bt, szinek);
    Vezerles vez;
    vez.megallitva_jatek = true; // uj_menet()-ben benne lesz (jatek altal megallitva -> nem renderel)



   while (!bill.menu_Esc)
    {
        /* Lekerdezesek a program mukodesehez */
        Uint32 ablakIDk[4] = { SDL_GetWindowID(ablakok[MENU].ablak), SDL_GetWindowID(ablakok[JATEK].ablak), SDL_GetWindowID(ablakok[SUGO].ablak), SDL_GetWindowID(ablakok[DICS_LISTA].ablak) };
        
        /* Billentyuk erzekelese */
        SDL_Event event;
        SDL_WaitEvent(&event);
        billentyuk_erzekelese(&event, &bill, ablakIDk);

        sugo_es_dics_lista_ablakok_kezelese(&bill, ablakok, &bt, szinek);
        menu_kivalasztas(&kiv, &bill, ablakok+MENU, &bt, szinek);

        jatek_ablak_kezelese(&bill, ablakok+JATEK, &vez, kiv.aktiv_jatekosok);
        if (ablakok[JATEK].nyitva) jatek_hatteret_kirajzol(ablakok+JATEK);
        

        
        billentyuk_tiltasa(&bill);
        
    }
    
    
    
    /* KILEPES */
    //texturak_torlese(ablakok);
    betutipusok_bezarasa(&bt);
    SDL_Quit();
    felszabadit(ablakok, szinek);

    return 0;
}

void inicializalas(Ablak* ablakok, Betutipusok* bt, SDL_Color* szinek) {
    /* SDL inicializalasa */
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    
    ablakot_letrehoz(ablakok+MENU);

    /* Betutipus betoltese, X pont magassaggal */
    TTF_Init();
    bt->med20 = betutipus_betoltese("OpenSans-Medium.ttf", 20);
    bt->reg15 = betutipus_betoltese("OpenSans-Regular.ttf", 15);
    bt->reg20 = betutipus_betoltese("OpenSans-Regular.ttf", 20);
    bt->bold20 = betutipus_betoltese("OpenSans-Bold.ttf", 20);

    fix_menut_kirajzol(ablakok+MENU, bt, szinek);
}

void felszabadit(Ablak* ablakok, SDL_Color* szinek) {
    free(ablakok);
    free(szinek);
}

void betutipusok_bezarasa(Betutipusok* bt) {
    TTF_CloseFont(bt->med20);
    TTF_CloseFont(bt->reg15);
    TTF_CloseFont(bt->reg20);
    TTF_CloseFont(bt->bold20);
}

TTF_Font* betutipus_betoltese(char* nev, int meret) {
    TTF_Font* betutipus = TTF_OpenFont(nev, meret);
    if (!betutipus) {
        SDL_Log("Nem sikerult megnyitni a betutipust! %s\n", TTF_GetError());
        exit(1);
    }
    return betutipus;
}

SDL_Color* szinek_letrehozasa() {
    SDL_Color* szinek = (SDL_Color*) malloc(7 * sizeof(SDL_Color));
    if (!szinek) printf("Nem sikerult a szineknek memoriat foglalni :c\n");

    szinek[FEHER] = (SDL_Color) {255, 255, 255};
    szinek[FEKETE] = (SDL_Color) {0, 0, 0};
    szinek[PIROS] = (SDL_Color) {255, 0, 0};
    szinek[ROZSA] = (SDL_Color) {255, 0, 255};
    szinek[ZOLD] = (SDL_Color) {0, 255, 0};
    szinek[KEK] = (SDL_Color) {0, 0, 255};
    szinek[SZURKE] = (SDL_Color) {30, 30, 30};

    return szinek;
}

void menu_kivalasztas(Kivalasztas* kiv, Billentyuk* bill, Ablak* menu, Betutipusok* bt, SDL_Color* szinek) {   
    SDL_RenderClear(menu->megjelenito);
    fix_menut_kirajzol(menu, bt, szinek);
    
    /* Jatekmodok kivalasztasa */
    if (bill->menu_F1) kiv->kiv_jt_mod = 0;
    else if (bill->menu_F2) kiv->kiv_jt_mod = 1;
    else if (bill->menu_F3) kiv->kiv_jt_mod = 2;

    // Normal
    if (kiv->kiv_jt_mod == 0) {        
        szoveget_kiir("Normál mód (F1)", 70, 130, szinek[FEHER], szinek[SZURKE], bt->bold20, menu->megjelenito, true);
        szoveget_kiir("Fal nélküli (F2)", 70, 190, szinek[FEHER], szinek[SZURKE], bt->reg20, menu->megjelenito, true);
        szoveget_kiir("Felvehető elemek tiltása (F3)", 70, 250, szinek[FEHER], szinek[SZURKE], bt->reg20, menu->megjelenito, true);
    }    
    // Fal nelkuli
    else if (kiv->kiv_jt_mod == 1) {        
        szoveget_kiir("Normál mód (F1)", 70, 130, szinek[FEHER], szinek[SZURKE], bt->reg20, menu->megjelenito, true);
        szoveget_kiir("Fal nélküli (F2)", 70, 190, szinek[FEHER], szinek[SZURKE], bt->bold20, menu->megjelenito, true);
        szoveget_kiir("Felvehető elemek tiltása (F3)", 70, 250, szinek[FEHER], szinek[SZURKE], bt->reg20, menu->megjelenito, true);
    }    
    // Felveheto elemek tiltasa
    else if (kiv->kiv_jt_mod) {        
        szoveget_kiir("Normál mód (F1)", 70, 130, szinek[FEHER], szinek[SZURKE], bt->reg20, menu->megjelenito, true);
        szoveget_kiir("Fal nélküli (F2)", 70, 190, szinek[FEHER], szinek[SZURKE], bt->reg20, menu->megjelenito, true);
        szoveget_kiir("Felvehető elemek tiltása (F3)", 70, 250, szinek[FEHER], szinek[SZURKE], bt->bold20, menu->megjelenito, true);
    }

    /* Jatekosok kivalasztasa */
    // Piros
    if (bill->menu_Q && !bill->menu_tilt_Q) kiv->aktiv_jatekosok[0] = !kiv->aktiv_jatekosok[0];
    if (kiv->aktiv_jatekosok[0] == true) szoveget_kiir("Kiválasztva", 670, 115, szinek[PIROS], szinek[SZURKE], bt->bold20, menu->megjelenito, true);
    // Rozsa
    if (bill->menu_Per && !bill->menu_tilt_Per) kiv->aktiv_jatekosok[1] = !kiv->aktiv_jatekosok[1];
    if (kiv->aktiv_jatekosok[1] == true) szoveget_kiir("Kiválasztva", 670, 165, szinek[ROZSA], szinek[SZURKE], bt->bold20, menu->megjelenito, true);
    // Zold
    if (bill->menu_M && !bill->menu_tilt_M) kiv->aktiv_jatekosok[2] = !kiv->aktiv_jatekosok[2];
    if (kiv->aktiv_jatekosok[2] == true) szoveget_kiir("Kiválasztva", 670, 215, szinek[ZOLD], szinek[SZURKE], bt->bold20, menu->megjelenito, true);
    // Kek
    if (bill->menu_Bal && !bill->menu_tilt_Bal) kiv->aktiv_jatekosok[3] = !kiv->aktiv_jatekosok[3];
    if (kiv->aktiv_jatekosok[3] == true) szoveget_kiir("Kiválasztva", 670, 265, szinek[KEK], szinek[SZURKE], bt->bold20, menu->megjelenito, true);
    
    SDL_RenderPresent(menu->megjelenito);
}