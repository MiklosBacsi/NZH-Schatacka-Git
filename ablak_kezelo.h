#ifndef ABLAK_KEZELO_H
#define ALBLAK_KEZELO_H

enum Ablak_tipus {MENU, JATEK, SUGO, DICS_LISTA};

typedef struct Ablak {
    //SDL_Window* ablak;
    //SDL_Renderer* megjelenito;
    int szelesseg, magassag;
    Ablak_tipus abl_tip;
} Ablak;

void menu_init();



#endif