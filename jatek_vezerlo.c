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

#define PIROS_SDL (SDL_Color) {255, 0, 0}
#define ROZSA_SDL (SDL_Color) {255, 0, 255}
#define ZOLD_SDL (SDL_Color) {0, 255, 0}
#define KEK_SDL (SDL_Color) {0, 100, 255}
#define SZURKE_SDL (SDL_Color) {30, 30, 30}
#define FEHER_SDL (SDL_Color) {255, 255, 255}
#define FEKETE_SDL (SDL_Color) {0, 0, 0}

#define FALVASTAGSAG 1 //Ne noveld!!!!

SDL_Color SDL_Szin[6] = {PIROS_SDL, ROZSA_SDL, ZOLD_SDL, KEK_SDL, FEHER_SDL, FEKETE_SDL};

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

static void fal_pixel_db(Vezerles* vez) {
    // Ne legyen tulcsordulas
    vez->fal_db = 2 * FALVASTAGSAG * (vez->palya_meret.x + vez->palya_meret.y - 2 * FALVASTAGSAG * FALVASTAGSAG);
}

static void falak_letrehozasa(Vezerles* vez) {
    fal_pixel_db(vez);
    /* Memoria foglalas */
    vez->fal_vonal = (Fal*) malloc(vez->fal_db * sizeof(Fal));

    if (vez->fal_vonal == NULL) printf("Nem sikerult falvonal memoriat foglalni :c\n");
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


        /* Fal letrehozasa es memori lefoglalasa */
        if (vez->jt_mod != FAL_NELKULI && vez->fal_vonal == NULL) {
            falak_letrehozasa(vez);
        }
        
        if (vez->jt_mod == FAL_NELKULI && vez->fal_vonal != NULL) {
            for (int i=0; i < vez->fal_db; ++i)
                vez->fal_vonal[i].torolve = true;
        }
            

        /* Pixel koordinatak kiszamolasa */
        int index = 0; short x;
        for (short y=0; y < vez->palya_meret.y; ++y) {
            x = 0;
            while (x < vez->palya_meret.x) {
                // Kozepe -> ugras szelre
                if (FALVASTAGSAG <= y && y < vez->palya_meret.y-FALVASTAGSAG && FALVASTAGSAG <= x && x < vez->palya_meret.x-FALVASTAGSAG)
                    x = vez->palya_meret.x - FALVASTAGSAG;
                
                
                vez->fal_vonal[index++].pixel = (Pixel) {x, y};

                ++x;
            }
        }
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

static void jatek_hatteret_kirajzol(Ablak* jatek_ablak, Vezerles* vez) {
    boxRGBA(jatek_ablak->megjelenito, 0, 0, 1600, 900, 30, 30, 30, 255);
    // Hogy a vonalak jol nezzenek ki a kulonbozo jatekmodokban
    if (vez->jt_mod == FAL_NELKULI)
        boxRGBA(jatek_ablak->megjelenito, 1400, 0, 1401, 900, 255, 255, 255, 255);

    logot_rajzol(jatek_ablak, 1425, 10);
}

void uj_menet(Vezerles* vez, Jatekos* jatekosok) {
    vez->megallitva_jatek = true;
    vez->menet_vege = false;
    vez->menetido = 0.0;
    lovedekeket_torol(vez);
    for (int i=0; i < vez->jatekosszam; ++i) {
        *(jatekosok[i].tilt_lo) = false;
    }
    

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for (int i=0; i < vez->jatekosszam; ++i) {
        jatekosok[i].pontszam = 100;
    }

    // Fal
    if (vez->jt_mod != FAL_NELKULI && vez->fal_vonal != NULL) {
        for (int i=0; i < vez->fal_db; ++i)
            vez->fal_vonal[i].torolve = false;
    }

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
}

void jatek_kirajzolasa(Ablak* jatek_ablak, Vezerles* vez, Jatekos* jatekosok, Betutipusok* bt) {
    /* Fix elemek */
    jatek_hatteret_kirajzol(jatek_ablak, vez);
    /* Pontszamok */
    for (int i=0; i < vez->jatekosszam; ++i) {
        char pontszam[3+1];
        snprintf(pontszam, 3+1, "%d", jatekosok[i].pontszam);
        szoveget_kiir(pontszam, 1500, 100 + i*50, SDL_Szin[jatekosok[i].szin], SZURKE_SDL, bt->bold20, jatek_ablak->megjelenito, false);
    }

    /* Fal */
    for (int i=0; i < vez->fal_db; ++i) {
        if (vez->jt_mod != FAL_NELKULI && vez->fal_vonal[i].torolve == false) {
            pixelRGBA(jatek_ablak->megjelenito, vez->fal_vonal[i].pixel.x, vez->fal_vonal[i].pixel.y, 255, 255, 255, 255);
        }
    }


    /* Jatekosvonalak */
    
    /* Felveheto elemek */

    /* Lovedekek */
    Lovedek* mozgo;
    for (mozgo = vez->lovedekek; mozgo != NULL; mozgo = mozgo->kov) {
        switch (mozgo->szin) {
        case PIROS:
            circleRGBA(jatek_ablak->megjelenito, (Sint16)mozgo->kp.x, (Sint16)mozgo->kp.y, mozgo->sugar, 255, 0, 0, 255);
            break;
        case ROZSA:
            circleRGBA(jatek_ablak->megjelenito, (Sint16)mozgo->kp.x, (Sint16)mozgo->kp.y, mozgo->sugar, 255, 0, 255, 255);
            break;
        case ZOLD:
            circleRGBA(jatek_ablak->megjelenito, (Sint16)mozgo->kp.x, (Sint16)mozgo->kp.y, mozgo->sugar, 0, 255, 0, 255);
            break;
        case KEK:
            circleRGBA(jatek_ablak->megjelenito, (Sint16)mozgo->kp.x, (Sint16)mozgo->kp.y, mozgo->sugar, 0, 100, 255, 255);
            break;        
        default:
            printf("Kirajzolasnal hibas lovedek szin!\n");
            break;
        }
    }
    
    /* Jatekosok fejei */
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
            circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 2, 0, 100, 255, 255);
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

void lovedekek_mozditasa(Lovedek** lov, double elmozd) {
    Lovedek* mozgo;
    for (mozgo = *lov; mozgo != NULL; mozgo = mozgo->kov) {
        mozgo->kp.x += cos(mozgo->irany) * elmozd;
        mozgo->kp.y += sin(mozgo->irany) * elmozd;
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
        if (jatekosok[i].fej.x < 0 || jatekosok[i].fej.x >= 1400 || jatekosok[i].fej.y < 0 || jatekosok[i].fej.y >= 900) {
            jatekosok[i].eletben_van = false;
            // halalfej animaciot rajzol !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }

        /* Jatekos utkozik a fallal */ //jatekmod!
        if (vez->jt_mod != FAL_NELKULI && (jatekosok[i].fej.x < 2.0 || jatekosok[i].fej.x > 1398.0 || jatekosok[i].fej.y < 2.0 || jatekosok[i].fej.y > 898))
            jatekosok[i].eletben_van = false;

        /* Jatekos utkozik egy vonallal */ //pajzs

        /* Jatekos utkozik lovedekkel */ //pajzs


    }
    if (halottak >= vez->jatekosszam-1) {
        vez->menet_vege = true;
        vez->megallitva_jatek = true;
        // Szóközzel kell jatekot inditani szoveg!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
}

static void loves_hozzaadasa(double sugar, Lovedek** lov, Jatekos* jat) {
    Lovedek* uj_lov = (Lovedek*) malloc(sizeof(Lovedek));
    if (!uj_lov) printf("Nem sikerult memoriat fogalalni a lovedeknek! :c\n");

    Koordinata kp = (Koordinata) {jat->fej.x + 1.5 * sugar * cos(jat->irany), jat->fej.y + 1.5 * sugar * sin(jat->irany)};
    uj_lov->kp = kp;
    uj_lov->irany = jat->irany;
    uj_lov->sugar = sugar;
    uj_lov->szin = jat->szin;
    uj_lov->kov = *lov;
    *lov = uj_lov;
}

void loves_vizsgalata(Jatekos* jatekosok, Vezerles* vez) {
    for (int i=0; i < vez->jatekosszam; ++i) {
        if (jatekosok[i].eletben_van == false)
            continue;

        if (*(jatekosok[i].lo) && !(*(jatekosok[i].tilt_lo)) && jatekosok[i].pontszam >= 5){ // VAGY VAN SPECIALIS ELEME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            *(jatekosok[i].tilt_lo) = true;
            jatekosok[i].pontszam -= 5;

            // Loves tipusa???


            // Normal loves
            loves_hozzaadasa(20.0, &vez->lovedekek, &jatekosok[i]);            
            // Y-tengely meg van invertalva !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            

            // Nagyloves

            // Sokloves
        }
    }
}

void lovedekeket_torol(Vezerles* vez) {
    Lovedek* iter = vez->lovedekek;
    while (iter != NULL) {
        Lovedek* kov = iter->kov;
        free(iter);
        iter = kov;
    }
    vez->lovedekek = NULL;
}

/*
Lovedek* mozgo;
for (mozgo = *lov; mozgo != NULL; mozgo = mozgo->kov) {

}
*/