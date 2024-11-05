#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "bemenet_es_fajlkezeles.h"
#include "ablak_kezelo.h"
#include "jatek_vezerlo.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "debugmalloc.h"

static int aktiv_jatekosok_szama(bool* kiv_jatekosok) {
    int db = 0;
    for (int i=0; i<4; ++i) {
        if (kiv_jatekosok[i]) ++db;
    }
    return db;
}

static void szinek_billK_hozzarend_jatHoz(Jatekos* jatekosok, bool* kiv_jat, Billentyuk* bill, Vezerles* vez) {
    /* Szinek hozzarendelese */
    int idx_jat = 0;
    for (int idx_osszes_jatekos = 0; idx_osszes_jatekos < 4; ++idx_osszes_jatekos) {
        if (kiv_jat[idx_osszes_jatekos])
            jatekosok[idx_jat++].szin = idx_osszes_jatekos;
    }
    /* Billentyuk hozzarendelese */
    for (int idx_aktiv_jatekos = 0; idx_aktiv_jatekos < vez->jatekosszam; ++idx_aktiv_jatekos) {
        switch (jatekosok[idx_aktiv_jatekos].szin) {
            case PIROS:
                jatekosok[idx_aktiv_jatekos].bal = &bill->jatek_Q;
                jatekosok[idx_aktiv_jatekos].lo = &bill->jatek_2;
                jatekosok[idx_aktiv_jatekos].tilt_lo = &bill->tilt_2;
                jatekosok[idx_aktiv_jatekos].jobb = &bill->jatek_W;                
                break;
            case ROZSA:
                jatekosok[idx_aktiv_jatekos].bal = &bill->jatek_Per;
                jatekosok[idx_aktiv_jatekos].lo = &bill->jatek_Csillag;
                jatekosok[idx_aktiv_jatekos].tilt_lo = &bill->tilt_Csillag;
                jatekosok[idx_aktiv_jatekos].jobb = &bill->jatek_Kotojel; 
                break;

            case ZOLD:
                jatekosok[idx_aktiv_jatekos].bal = &bill->jatek_M;
                jatekosok[idx_aktiv_jatekos].lo = &bill->jatek_K;
                jatekosok[idx_aktiv_jatekos].tilt_lo = &bill->tilt_K;
                jatekosok[idx_aktiv_jatekos].jobb = &bill->jatek_Vesszo; 
                break;

            case KEK:
                jatekosok[idx_aktiv_jatekos].bal = &bill->jatek_Bal;
                jatekosok[idx_aktiv_jatekos].lo = &bill->jatek_Fel;
                jatekosok[idx_aktiv_jatekos].tilt_lo = &bill->tilt_Fel;
                jatekosok[idx_aktiv_jatekos].jobb = &bill->jatek_Jobb; 
                break;        
            default:
                printf("Hiba tortent a billentyuk jatekosokhoz valo hozzarendelesenel!\n");
                break;
        }
    }

}

static Pixel randPixel() {
    short x = (rand() % 1200) + 100;
    short y = (rand() % 700) + 100;
    return (Pixel) {x, y};
}

static Koordinata randKoord() {
    Pixel rand_pixel = randPixel();
    double x = (double) rand_pixel.x;
    double y = (double) rand_pixel.y;
    return (Koordinata) {x,y};
}

double tav(Koordinata k1, Koordinata k2) {
    return sqrt( pow(k1.x-k2.x, 2) + pow(k1.y-k2.y, 2));
}

static void randFej(Jatekos* jatekosok, Vezerles* vez) {
    for (int i=0; i < vez->jatekosszam; ++i) {
        bool jo_koordinata;
        do
        {
            jatekosok[i].fej = randKoord();
            if (i == 0) break;
            
            /* Megfelelo tavolsag biztositasa */
            for (int j=0; j<i; ++j) {
                jo_koordinata = true;
                if (tav(jatekosok[i].fej, jatekosok[j].fej) < 200.0) {
                    jo_koordinata = false;
                    break;
                }
            }
        } while (!jo_koordinata);

        /* Random irany */
        jatekosok[i].irany = (double) (rand() % 360);
    }
}

static void jatekosokat_fogalal(Vezerles* vez, Jatekos** cim_jat_abl) {
    Jatekos* mutato = (Jatekos*) malloc(vez->jatekosszam *  sizeof(Jatekos));
    if (!mutato) printf("Nem sikerult jatekosokat foglalni");

    *cim_jat_abl = mutato;
}

void jatek_ablak_kezelese(Billentyuk* bill, Ablak* jatek_ablak, Vezerles* vez, Jatekos** cim_jatekosok, int jt_mod, bool* kiv_jat) {
    /* Ablak megnyitasa */
    if (bill->menu_Szokoz && !jatek_ablak->nyitva && aktiv_jatekosok_szama(kiv_jat) >= 2) {
        ablakot_letrehoz(jatek_ablak);
        vez->megallitva_felhasznalo = false;
        
        // Vezerles - adatok
        vez->jt_mod = jt_mod;
        vez->jatekosszam = aktiv_jatekosok_szama(kiv_jat);
        vez->max_pontszam = (vez->jatekosszam - 1) * 40;
        
        /*** JATEKOSOK LETREHOZASA ***/
        jatekosokat_fogalal(vez, cim_jatekosok);
        szinek_billK_hozzarend_jatHoz(*cim_jatekosok, kiv_jat, bill, vez);

        uj_menet(vez, *cim_jatekosok);
    }
    /* Ablak bezarasa */
    else if (bill->jatek_Esc && !bill->tilt_Esc && vez->megallitva_felhasznalo && jatek_ablak->nyitva) {
        SDL_DestroyRenderer(jatek_ablak->megjelenito);
        SDL_DestroyWindow(jatek_ablak->ablak);
        jatek_ablak->ablak = NULL;
        jatek_ablak->megjelenito = NULL;

        jatek_ablak->nyitva = false;
        bill->jatek_Esc = false;
        vez->megallitva_felhasznalo = false;
        vez->megallitva_jatek = true;
        
        free(*cim_jatekosok);
        *cim_jatekosok = NULL;
    }
    /* Megallitas - Felhasznalo */
    else if (bill->jatek_Esc && !vez->megallitva_felhasznalo && jatek_ablak->nyitva) {
        vez->megallitva_felhasznalo = true;
    }
    /* Folytatas - Felhasznalo */
    else if (bill->jatek_Szokoz && (vez->megallitva_felhasznalo || vez->megallitva_jatek) && jatek_ablak->nyitva) {
        vez->megallitva_felhasznalo = false;
        vez->megallitva_jatek = false;
    }
}

void jatek_hatteret_kirajzol(Ablak* jatek_ablak) {
    boxRGBA(jatek_ablak->megjelenito, 0, 0, 1600, 900, 30, 30, 30, 255);
    boxRGBA(jatek_ablak->megjelenito, 1401, 0, 1402, 900, 255, 255, 255, 255);

    logot_rajzol(jatek_ablak, 1425, 10);
}

void uj_menet(Vezerles* vez, Jatekos* jatekosok) {
    vez->megallitva_jatek = true;
    vez->menet_vege = false;
    vez->menetido = 0.0;

    /* Megjelenites */
    // Kiir: A menet inditasahoz nyomja meg a Szokozt
    
    // Jatekosok vonalanak torlese (din. tomb)
    // Lovedekek torlese (din. tomb)
    // Felveheto elemek torlese (din. tomb)
    // Animaciok torlese (din. tomb)

    

    // Jatekosok: random fej, irany
    randFej(jatekosok, vez);

    // Jatekosok: eletben_van = true
    for (int i=0; i < vez->jatekosszam; ++i) {
        jatekosok[i].eletben_van = true;
    }
    // Falak letrehozasa!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void jatek_kirajzolasa(Ablak* jatek_ablak, Vezerles* vez, Jatekos* jatekosok, Betutipusok* bt) {
    /* Fix elemek */
    jatek_hatteret_kirajzol(jatek_ablak);
    /* Pontszamok */

    /* Palya vonalak */
    
    /* Jatekosvonalak */
    
    /* Felveheto elemek */

    /* Lovedekek */
    
    /* Jatekosok */
    for (int i=0; i < vez->jatekosszam; ++i) {
        if (!jatekosok[i].eletben_van) continue;

        switch (jatekosok[i].szin) {
        case PIROS:
            circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 2, 255, 0, 0, 255);
            break;
        case ROZSA:
            circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 2, 255, 0, 255, 255);
            break;
        case ZOLD:
            circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 2, 0, 255, 0, 255);
            break;
        case KEK:
            circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 2, 0, 0, 255, 255);
            break;
        default:
            printf("Hiba a jatekosok fejenek kirajzolasaval! Szin: %d\n", jatekosok[i].szin);
            break;
        }
    }
    SDL_RenderPresent(jatek_ablak->megjelenito);
}

Uint32 idozit(Uint32 ms, void *param) {
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;   /* ujabb varakozas */
}

void jatekosok_mozditasa(Jatekos* jatekosok, Vezerles* vez) {
    for (int i=0; i < vez->jatekosszam; ++i) {
        if (!jatekosok[i].eletben_van) continue;

        /* Fok allitasa */
        // Balra
        if (*(jatekosok[i].bal) && !(*(jatekosok[i].jobb)))
            jatekosok[i].irany -= vez->fordulas;
        // Jobbra
        if (*(jatekosok[i].jobb) && !(*(jatekosok[i].bal)))
            jatekosok[i].irany += vez->fordulas;
        
        /* Jatekosok elmozditasa */
        jatekosok[i].fej.x += cos(jatekosok[i].irany) * vez->elmozd_jat;
        jatekosok[i].fej.y += sin(jatekosok[i].irany) * vez->elmozd_jat;
    }
}

void halal_vizsgalata(Jatekos* jatekosok, Vezerles* vez) {    
    int halottak = 0;
    for (int i=0; i < vez->jatekosszam; ++i) {
        if (!jatekosok[i].eletben_van) {
            ++halottak;
            continue;
        }
        /* Jatekos kimegy a palyarol */
        if (jatekosok[i].fej.x < 0 || jatekosok[i].fej.x > 1400 || jatekosok[i].fej.y < 0 || jatekosok[i].fej.y > 900) {
            jatekosok[i].eletben_van = false;
            // halalfej animaciot rajzol !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }

        /* Jatekos utkozik a fallal */ //jatekmod!

        /* Jatekos utkozik egy vonallal */ //pajzs

        /* Jatekos utkozik lovedekkel */ //pajzs


    }
    if (halottak >= vez->jatekosszam-1) {
        vez->menet_vege = true;
        vez->megallitva_jatek = true;
        // Szóközzel kell jatekot inditani szoveg!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
}