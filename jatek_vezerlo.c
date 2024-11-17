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
#include <time.h>
#include "debugmalloc.h"

#define PIROS_SDL (SDL_Color) {255, 0, 0}
#define ZOLD_SDL (SDL_Color) {0, 255, 0}
#define KEK_SDL (SDL_Color) {0, 100, 255}
#define ROZSA_SDL (SDL_Color) {255, 0, 255}
#define SZURKE_SDL (SDL_Color) {30, 30, 30}
#define FEHER_SDL (SDL_Color) {255, 255, 255}
#define FEKETE_SDL (SDL_Color) {0, 0, 0}
// Uj vonal lerakasank idokoze (*20ms)
#define GYAK 6
#define VON_TAV_HALAL 4.0
#define KEZDOPONTSZAM 50
#define RADIAN(fok) (fok * 3.1416 / 180.0)

SDL_Color SDL_Szin[6] = {PIROS_SDL, ZOLD_SDL, KEK_SDL, ROZSA_SDL, FEHER_SDL, FEKETE_SDL};

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
                jatekosok[idx_aktiv_jatekos].bal = &bill->jatek_A;
                jatekosok[idx_aktiv_jatekos].lo = &bill->jatek_W;
                jatekosok[idx_aktiv_jatekos].tilt_lo = &bill->tilt_W;
                jatekosok[idx_aktiv_jatekos].jobb = &bill->jatek_D;                
                break;

            case ZOLD:
                jatekosok[idx_aktiv_jatekos].bal = &bill->jatek_J;
                jatekosok[idx_aktiv_jatekos].lo = &bill->jatek_I;
                jatekosok[idx_aktiv_jatekos].tilt_lo = &bill->tilt_I;
                jatekosok[idx_aktiv_jatekos].jobb = &bill->jatek_L; 
                break;

            case KEK:
                jatekosok[idx_aktiv_jatekos].bal = &bill->jatek_Bal;
                jatekosok[idx_aktiv_jatekos].lo = &bill->jatek_Fel;
                jatekosok[idx_aktiv_jatekos].tilt_lo = &bill->tilt_Fel;
                jatekosok[idx_aktiv_jatekos].jobb = &bill->jatek_Jobb; 
                break;

            case ROZSA:
                jatekosok[idx_aktiv_jatekos].bal = &bill->jatek_7;
                jatekosok[idx_aktiv_jatekos].lo = &bill->jatek_Per;
                jatekosok[idx_aktiv_jatekos].tilt_lo = &bill->tilt_Per;
                jatekosok[idx_aktiv_jatekos].jobb = &bill->jatek_9; 
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

static double tav(Koordinata k1, Koordinata k2) {
    return sqrt( pow(k1.x-k2.x, 2) + pow(k1.y-k2.y, 2));
}

static void randFej(Jatekos* jatekosok, Vezerles* vez) {
    // Random seed
    srand(time(NULL));

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

static void eletben_levo_jatekosok_pontjanak_novelese_halalkor(Jatekos* jatekosok, Vezerles* vez, int halottak_szama) {
    for (int i=0; i < vez->jatekosszam; ++i) {
        if (jatekosok[i].eletben_van == false)
            continue;
        jatekosok[i].pontszam += 3 + halottak_szama;
    }
}

static void jatekosokat_fogalal(Vezerles* vez, Jatekos** cim_jat_abl) {
    Jatekos* mutato = (Jatekos*) malloc(vez->jatekosszam *  sizeof(Jatekos));
    if (!mutato) printf("Nem sikerult jatekosokat foglalni");

    *cim_jat_abl = mutato;

    for (int i=0; i < vez->jatekosszam; ++i) {
        (*cim_jat_abl)[i].vonal = NULL;
    }
}

static void falak_letrehozasa(Vezerles* vez) {
    /* Memoria foglalas */
    Fal* felso = (Fal*) malloc(vez->falak.x_db * sizeof(Fal));
    Fal* also = (Fal*) malloc(vez->falak.x_db * sizeof(Fal));
    Fal* bal = (Fal*) malloc(vez->falak.y_db * sizeof(Fal));
    Fal* jobb = (Fal*) malloc(vez->falak.y_db * sizeof(Fal));   

    vez->falak.felso = felso;
    vez->falak.also = also;
    vez->falak.bal = bal;
    vez->falak.jobb = jobb;
    if (!felso || !also || !bal || !jobb)
        printf("Nem sikerult falvonal memoriat foglalni :c\n");

    /* Pixel koordinatak kiszamolasa */
    for (int i=0; i < vez->falak.x_db; ++i) {
        // Felso
        vez->falak.felso[i].pixel.x = i;
        vez->falak.felso[i].pixel.y = 0;
        // Also
        vez->falak.also[i].pixel.x = i;
        vez->falak.also[i].pixel.y = vez->palya_meret.y - 1;
    }
    for (int i=0; i < vez->falak.y_db; ++i) {
        // Bal
        vez->falak.bal[i].pixel.x = 0;
        vez->falak.bal[i].pixel.y = i;
        // Jobb
        vez->falak.jobb[i].pixel.x = vez->palya_meret.x - 1;
        vez->falak.jobb[i].pixel.y = i;
    }
}

static void animacio_hozzaadasa(AniTipus tipus, short x, short y, Vezerles* vez) {
    Animacio* uj_eleje = (Animacio*) malloc(sizeof(Animacio));
    if (!uj_eleje) {
        printf("Nem sikerult memoriat foglalni animacionak! :c\n"); fflush(stdout);
        return;
    }

    switch (tipus) {
        case HALALFEJ: uj_eleje->kep = vez->ani.halalfej; break;
        case PIROSPLUSZ1: uj_eleje->kep = vez->ani.pirosPluszEgy; break;
        case ZOLDPLUSZ1: uj_eleje->kep = vez->ani.zoldPluszEgy; break;
        case KEKPLUSZ1: uj_eleje->kep = vez->ani.kekPluszEgy; break;
        case ROZSAPLUSZ1: uj_eleje->kep = vez->ani.rozsaPluszEgy; break;
        default: printf("Hibas animacio tipus van megadva uj animacio hozzaadasanal!\n}"); fflush(stdout); break;
    }

    uj_eleje->poz.x = x;
    uj_eleje->poz.y = y;
    uj_eleje->elet_tartam = 0.5;
    
    uj_eleje->kov = vez->animaciok;
    vez->animaciok = uj_eleje;
}

static void animaciot_kirajzol(SDL_Texture* kep, short kpx, short kpy, SDL_Renderer* megjelenito) {
    SDL_Rect cel = { kpx-75/2, kpy-75/2, 75, 75 };
    /* Logo atmasolasa az ablakra */
    SDL_RenderCopy(megjelenito, kep, NULL, &cel);
}

void jatek_ablak_kezelese(Billentyuk* bill, Ablak* jatek_ablak, Vezerles* vez, Jatekos** cim_jatekosok, int jt_mod, bool* kiv_jat) {
    /* Ablak megnyitasa */
    if (bill->menu_Szokoz && !jatek_ablak->nyitva && aktiv_jatekosok_szama(kiv_jat) >= 2) {
        ablakot_letrehoz(jatek_ablak);
        vez->megallitva_felhasznalo = false;
        
        // Vezerles - adatok
        vez->jt_mod = jt_mod;
        vez->jatekosszam = aktiv_jatekosok_szama(kiv_jat);
        vez->max_elerheto_pontszam = (vez->jatekosszam - 1) * 40;
        
        /*** JATEKOSOK LETREHOZASA ***/
        jatekosokat_fogalal(vez, cim_jatekosok); // + Vonalak inicializalása NULL-ra
        szinek_billK_hozzarend_jatHoz(*cim_jatekosok, kiv_jat, bill, vez);


        /* Fal letrehozasa es memori lefoglalasa */
        if (vez->jt_mod != FAL_NELKULI && vez->falak.felso == NULL) {
            falak_letrehozasa(vez);
        }
        
        if (vez->jt_mod == FAL_NELKULI && vez->falak.felso != NULL) {
            for (int i=0; i < vez->falak.x_db; ++i) {
                vez->falak.felso[i].torolve = true;
                vez->falak.also[i].torolve = true;
            }
            for (int i=0; i < vez->falak.y_db; ++i) {
                vez->falak.bal[i].torolve = true;
                vez->falak.jobb[i].torolve = true;
            }
        }

        // Jatekosok pontszamanak inicializalasa
        for (int i=0; i < vez->jatekosszam; ++i) {
            (*cim_jatekosok)[i].pontszam = KEZDOPONTSZAM;
        }

        // Animaciok texturainak betoltese
        vez->ani.halalfej = textura_betoltese("Halalfej.png", jatek_ablak);
        vez->ani.pirosPluszEgy = textura_betoltese("PirosPluszEgy.png", jatek_ablak);
        vez->ani.zoldPluszEgy = textura_betoltese("ZoldPluszEgy.png", jatek_ablak);
        vez->ani.kekPluszEgy = textura_betoltese("KekPluszEgy.png", jatek_ablak);
        vez->ani.rozsaPluszEgy = textura_betoltese("RozsaPluszEgy.png", jatek_ablak);
        vez->ani.buborek = textura_betoltese("FelvehetoElem.png", jatek_ablak);
            
        uj_menet(vez, *cim_jatekosok);
    }
    /* Ablak bezarasa */
    else if (bill->jatek_Esc && !bill->tilt_Esc && vez->megallitva_felhasznalo && jatek_ablak->nyitva) {
        animacio_texturak_bezarasa(vez);
        SDL_DestroyRenderer(jatek_ablak->megjelenito);
        SDL_DestroyWindow(jatek_ablak->ablak);
        jatek_ablak->ablak = NULL;
        jatek_ablak->megjelenito = NULL;

        jatek_ablak->nyitva = false;
        bill->jatek_Esc = false;
        vez->megallitva_felhasznalo = false;
        vez->megallitva_jatek = true;
        vez->jatek_vege = false;
        
        vonalakat_torol(*cim_jatekosok, vez);
        lyukakat_torol(vez);
        animaciokat_torol(vez);

        free(*cim_jatekosok);
        *cim_jatekosok = NULL;
    }
    /* Megallitas - Felhasznalo */
    else if (bill->jatek_Esc && !vez->megallitva_felhasznalo && jatek_ablak->nyitva && !vez->jatek_vege) {
        vez->megallitva_felhasznalo = true;
    }
    /* Folytatas - Felhasznalo */
    else if (bill->jatek_Szokoz && (vez->megallitva_felhasznalo || vez->megallitva_jatek) && jatek_ablak->nyitva && !vez->jatek_vege) {
        vez->megallitva_felhasznalo = false;
        vez->megallitva_jatek = false;
    }
}

static void jatek_hatteret_kirajzol(Ablak* jatek_ablak, Vezerles* vez) {
    boxRGBA(jatek_ablak->megjelenito, 0, 0, 1600, 900, 30, 30, 30, 255);
}

void uj_menet(Vezerles* vez, Jatekos* jatekosok) {
    vez->megallitva_jatek = true;
    vez->megallitva_felhasznalo = true;
    vez->menet_vege = false;
    vez->menetido = 0;
    vez->vonal_szamlalo = 1;
    lovedekeket_torol(vez);
    lyukakat_torol(vez);
    vonalakat_torol(jatekosok, vez);
    animaciokat_torol(vez);
    felveheto_elemeket_torol(vez);

    for (int i=0; i < vez->jatekosszam; ++i) {
        *(jatekosok[i].tilt_lo) = false;
        jatekosok[i].lyuk_tilt = 0;
    }
    
    

    // Fal
    if (vez->jt_mod != FAL_NELKULI) {
        for (int i=0; i < vez->falak.x_db; ++i) {
            vez->falak.felso[i].torolve = false;
            vez->falak.also[i].torolve = false;
        }
        for (int i=0; i < vez->falak.y_db; ++i) {
            vez->falak.bal[i].torolve = false;
            vez->falak.jobb[i].torolve = false;
        }
    }

    // Jatekosok: random fej, irany
    randFej(jatekosok, vez);

    // Jatekosok: eletben_van = true + spec. elemek
    for (int i=0; i < vez->jatekosszam; ++i) {
        jatekosok[i].eletben_van = true;
        jatekosok[i].spec.tipus = NINCS;
        jatekosok[i].spec.elet_tartam = 0.0;
    }
}

void jatek_kirajzolasa(Ablak* jatek_ablak, Vezerles* vez, Jatekos* jatekosok, Betutipusok* bt) {
    /* Fix elemek */
    jatek_hatteret_kirajzol(jatek_ablak, vez);
    
    /* Fal */
    if (vez->jt_mod != FAL_NELKULI) {
        for (int i=0; i < vez->falak.x_db; ++i) {
            // Felso
            if (vez->falak.felso[i].torolve == false)
                pixelRGBA(jatek_ablak->megjelenito, vez->falak.felso[i].pixel.x, vez->falak.felso[i].pixel.y, 255, 255, 255, 255);
            // Also
            if (vez->falak.also[i].torolve == false)
                pixelRGBA(jatek_ablak->megjelenito, vez->falak.also[i].pixel.x, vez->falak.also[i].pixel.y, 255, 255, 255, 255);
        }
        for (int i=0; i < vez->falak.y_db; ++i) {
            // Bal
            if (vez->falak.bal[i].torolve == false)
                pixelRGBA(jatek_ablak->megjelenito, vez->falak.bal[i].pixel.x, vez->falak.bal[i].pixel.y, 255, 255, 255, 255);
            // Jobb
            if (vez->falak.jobb[i].torolve == false)
                pixelRGBA(jatek_ablak->megjelenito, vez->falak.jobb[i].pixel.x, vez->falak.jobb[i].pixel.y, 255, 255, 255, 255);
        }
    }

    /* Jatekosvonalak */
    Vonal* mozgoV;
    for (int i=0; i < vez->jatekosszam; ++i) {
        for (mozgoV = jatekosok[i].vonal; mozgoV != NULL; mozgoV = mozgoV->kov) {
            switch (mozgoV->szin) {
            case PIROS:
                circleRGBA(jatek_ablak->megjelenito, (Sint16)mozgoV->kord.x, (Sint16)mozgoV->kord.y, 2, 255, 0, 0, 255);
                break;            
            case ZOLD:
                circleRGBA(jatek_ablak->megjelenito, (Sint16)mozgoV->kord.x, (Sint16)mozgoV->kord.y, 2, 0, 255, 0, 255);
                break;
            case KEK:
                circleRGBA(jatek_ablak->megjelenito, (Sint16)mozgoV->kord.x, (Sint16)mozgoV->kord.y, 2, 0, 100, 255, 255);
                break;
            case ROZSA:
                circleRGBA(jatek_ablak->megjelenito, (Sint16)mozgoV->kord.x, (Sint16)mozgoV->kord.y, 2, 255, 0, 255, 255);
                break;     
            default:
                printf("Kirajzolasnal hibas lovedek szin!\n");
                printf("Szin: %d, mozgoV: %p, Köv: %p\n", mozgoV->szin, (void*) mozgoV, (void*) mozgoV->kov);
                break;
            }
        }
    } 
    
    /* Felveheto elemek (buborekok) */
    FelvehetoElem* mozgoB = vez->buborekok;
    while (mozgoB != NULL) {
        animaciot_kirajzol(vez->ani.buborek, mozgoB->kp.x, mozgoB->kp.y, jatek_ablak->megjelenito);
        mozgoB = mozgoB->kov;
    }

    /* Lovedekek */
    Lovedek* mozgoL;
    for (mozgoL = vez->lovedekek; mozgoL != NULL; mozgoL = mozgoL->kov) {
        switch (mozgoL->szin) {
        case PIROS:
            circleRGBA(jatek_ablak->megjelenito, (Sint16)mozgoL->kp.x, (Sint16)mozgoL->kp.y, mozgoL->sugar, 255, 0, 0, 255);
            break;
        case ZOLD:
            circleRGBA(jatek_ablak->megjelenito, (Sint16)mozgoL->kp.x, (Sint16)mozgoL->kp.y, mozgoL->sugar, 0, 255, 0, 255);
            break;
        case KEK:
            circleRGBA(jatek_ablak->megjelenito, (Sint16)mozgoL->kp.x, (Sint16)mozgoL->kp.y, mozgoL->sugar, 0, 100, 255, 255);
            break;
        case ROZSA:
            circleRGBA(jatek_ablak->megjelenito, (Sint16)mozgoL->kp.x, (Sint16)mozgoL->kp.y, mozgoL->sugar, 255, 0, 255, 255);
            break; 
        default:
            printf("Kirajzolasnal hibas lovedek szin!\n");
            break;
        }
    }
    
    /* Jatekosok fejei + PAJZS */
    for (int i=0; i < vez->jatekosszam; ++i) {
        if (!jatekosok[i].eletben_van) continue;

        switch (jatekosok[i].szin) {
        case PIROS:
            /* Fej */
            circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 2, 255, 0, 0, 255);
            /* Pajzs */
            if (jatekosok[i].spec.tipus == PAJZS) {
                // Ha mindjart lejar
                if (jatekosok[i].spec.elet_tartam < 3.0)
                    circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 10, 255, 0, 0, 150);
                else
                    circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 10, 255, 0, 0, 255);
            }
            break;
        case ZOLD:
            /* Fej */
            circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 2, 0, 255, 0, 255);
            /* Pajzs */
            if (jatekosok[i].spec.tipus == PAJZS) {
                // Ha mindjart lejar
                if (jatekosok[i].spec.elet_tartam < 3.0)
                    circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 10, 0, 255, 0, 150);
                else
                    circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 10, 0, 255, 0, 255);
            }
            break;
        case KEK:
            /* Fej */
            circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 2, 0, 100, 255, 255);
            /* Pajzs */
            if (jatekosok[i].spec.tipus == PAJZS) {
                // Ha mindjart lejar
                if (jatekosok[i].spec.elet_tartam < 3.0)
                    circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 10, 0, 100, 255, 150);
                else
                    circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 10, 0, 100, 255, 255);
            }
            break;
        case ROZSA:
            /* Fej */
            circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 2, 255, 0, 255, 255);
            /* Pajzs */
            if (jatekosok[i].spec.tipus == PAJZS) {
                // Ha mindjart lejar
                if (jatekosok[i].spec.elet_tartam < 3.0)
                    circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 10, 255, 0, 255, 150);
                else
                    circleRGBA(jatek_ablak->megjelenito, (Sint16)jatekosok[i].fej.x, (Sint16)jatekosok[i].fej.y, 10, 255, 0, 255, 255);
            }
            break;
        default:
            printf("Hiba a jatekosok fejenek/pajzsanak kirajzolasaval! Szin: %d\n", jatekosok[i].szin);
            break;
        }
    }

    // Jobb savot ujrarajzolom, hogy a lovedekek ne latszodjanak rajta
    boxRGBA(jatek_ablak->megjelenito, 1400, 0, 1600, 900, 30, 30, 30, 255);

    // Hogy fal nelkuli jatekmodban latszodjon a palya szele
    if (vez->jt_mod == FAL_NELKULI) {
        boxRGBA(jatek_ablak->megjelenito, 1400, 0, 1401, 900, 255, 255, 255, 255);
    }

    /* Animaciok */
    Animacio* mozgoA = vez->animaciok;
    while (mozgoA != NULL) {
        animaciot_kirajzol(mozgoA->kep, mozgoA->poz.x, mozgoA->poz.y, jatek_ablak->megjelenito);
        mozgoA = mozgoA->kov;
    }

    /* Pontszamok */
    for (int i=0; i < vez->jatekosszam; ++i) {
        char pontszam[3+1];
        snprintf(pontszam, 3+1, "%d", jatekosok[i].pontszam);
        szoveget_kiir(pontszam, 1500, 100 + i*50, SDL_Szin[jatekosok[i].szin], SZURKE_SDL, bt->bold20, jatek_ablak->megjelenito, false);
    }

    /* Uj menet inditasa felirat */
    if (vez->megallitva_jatek && vez->jatek_vege == false) {
        boxRGBA(jatek_ablak->megjelenito, 375, 400-20, 999, 549-20, 20, 20, 20, 200);
        szoveget_kiir("Új menet", 655, 420-20, FEHER_SDL, SZURKE_SDL, bt->bold20, jatek_ablak->megjelenito, false);
        szoveget_kiir("Új menet indításához nyomja meg a szóközt.", 495, 460-20, FEHER_SDL, SZURKE_SDL, bt->reg20, jatek_ablak->megjelenito, false);
        szoveget_kiir("A játékból való kilépéshez nyomja meg az Esc-et", 475, 490-20, FEHER_SDL, SZURKE_SDL, bt->reg20, jatek_ablak->megjelenito, false);
    }
    /* Jatek megallitva - Folytatas felirat */
    else if (vez->megallitva_felhasznalo && vez->jatek_vege == false) {
        boxRGBA(jatek_ablak->megjelenito, 375, 400-20, 999, 549-20, 20, 20, 20, 200);
        szoveget_kiir("Játék megállítva", 620, 420-20, FEHER_SDL, SZURKE_SDL, bt->bold20, jatek_ablak->megjelenito, false);
        szoveget_kiir("A játék folytatásához nyomja meg a szóközt", 495, 460-20, FEHER_SDL, SZURKE_SDL, bt->reg20, jatek_ablak->megjelenito, false);
        szoveget_kiir("A játékból való kilépéshez nyomja meg az Esc-et", 475, 490-20, FEHER_SDL, SZURKE_SDL, bt->reg20, jatek_ablak->megjelenito, false);
    }
    /* Jatek vege felirat */
    else if (vez->megallitva_jatek && vez->jatek_vege == true) {
        boxRGBA(jatek_ablak->megjelenito, 400, 400-20, 999, 549-20, 20, 20, 20, 200);
        szoveget_kiir("Játék vége", 655, 420-20, FEHER_SDL, SZURKE_SDL, bt->bold20, jatek_ablak->megjelenito, false);
        
        char max_pontszam_str[4+1];
        snprintf(max_pontszam_str, 4+1, "%d", vez->max_pontszam);
        szoveget_kiir("Nyertes pontszáma:", 590, 460-20, FEHER_SDL, SZURKE_SDL, bt->reg20, jatek_ablak->megjelenito, false);
        szoveget_kiir(max_pontszam_str, 790, 460-20, FEHER_SDL, SZURKE_SDL, bt->bold20, jatek_ablak->megjelenito, false);
        
        szoveget_kiir("A játékból való kilépéshez nyomja meg az Esc-et", 475, 490-20, FEHER_SDL, SZURKE_SDL, bt->reg20, jatek_ablak->megjelenito, false);
    }

    /*
    Lyuk* mozgoLyuk = vez->lyukak;
    while (mozgoLyuk != NULL) {
        circleRGBA(jatek_ablak->megjelenito, (Sint16)mozgoLyuk->eleje.x, (Sint16)mozgoLyuk->eleje.y, 3, 255, 255, 255, 255);
        circleRGBA(jatek_ablak->megjelenito, (Sint16)mozgoLyuk->vege.x, (Sint16)mozgoLyuk->vege.y, 3, 255, 255, 255, 255);

        mozgoLyuk = mozgoLyuk->kov;
    }
    */
    //lineRGBA(jatek_ablak->megjelenito, 700, 0, 700, 900, 255, 0, 0, 255);
    //lineRGBA(jatek_ablak->megjelenito, 0, 450, 1400, 450, 255, 0, 0, 255);

    //lineRGBA(jatek_ablak->megjelenito, 640, 10, 640, 800, 0, 255, 0, 255);
    //lineRGBA(jatek_ablak->megjelenito, 760, 10, 760, 800, 0, 255, 0, 255);

    //lineRGBA(jatek_ablak->megjelenito, 900, 10, 900, 800, 0, 255, 255, 255);
    //lineRGBA(jatek_ablak->megjelenito, 500, 10, 500, 800, 0, 255, 255, 255);


    // Logo
    logot_rajzol(jatek_ablak, 1425, 10);


    /*** MEGJELENITES ***/
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
        if (jatekosok[i].eletben_van == false)
            continue;

        // Elozo pozicio elmentese (vonalhuzasnal kelleni fog)
        jatekosok[i].elozo = jatekosok[i].fej;

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

void lovedekek_mozditasa(Lovedek* lovedekek, double elmozd) {
    Lovedek* mozgo;
    for (mozgo = lovedekek; mozgo != NULL; mozgo = mozgo->kov) {
        mozgo->kp.x += cos(mozgo->irany) * elmozd;
        mozgo->kp.y += sin(mozgo->irany) * elmozd;
    }
}

static bool van_fal_atmenetkor_X(short fejY, Vezerles* vez) {
    if (fejY < 1) fejY = 1;
    else if (fejY > vez->palya_meret.y-2) fejY = vez->palya_meret.y - 2;

    if (!vez->falak.bal[fejY-1].torolve || !vez->falak.bal[fejY].torolve || !vez->falak.bal[fejY+1].torolve)
        return true;
    if (!vez->falak.jobb[fejY-1].torolve || !vez->falak.jobb[fejY].torolve || !vez->falak.jobb[fejY+1].torolve)
        return true;
    return false;    
}

static bool van_fal_atmenetkor_Y(short fejX, Vezerles* vez) {
    if (fejX < 1) fejX = 1;
    else if (fejX > vez->palya_meret.x-2) fejX = vez->palya_meret.x - 2;

    if (!vez->falak.felso[fejX-1].torolve || !vez->falak.felso[fejX].torolve || !vez->falak.felso[fejX+1].torolve)
        return true;
    if (!vez->falak.also[fejX-1].torolve || !vez->falak.also[fejX].torolve || !vez->falak.also[fejX+1].torolve)
        return true;
    return false;    
}

void halal_vizsgalata(Jatekos* jatekosok, Vezerles* vez) {    
    int halottak = 0;
    for (int i=0; i < vez->jatekosszam; ++i) {
        if (jatekosok[i].eletben_van == false) {
            ++halottak;
            continue;
        }
        /* Jatekos kimegy a palyarol - kijon a masik oldalon */
        short tulcs_x_min = 0 - (short) jatekosok[i].fej.x;
        short tulcs_x_max = (short) jatekosok[i].fej.x - vez->palya_meret.x;
        short tulcs_y_min = 0 - (short) jatekosok[i].fej.y;
        short tulcs_y_max = (short) jatekosok[i].fej.y - vez->palya_meret.y;
        
        // Jobb --> Bal
        if (tulcs_x_max > 0 && tulcs_x_max >= tulcs_y_min && tulcs_x_max >= tulcs_y_max) {
            if (vez->jt_mod != FAL_NELKULI && van_fal_atmenetkor_X((short)jatekosok[i].fej.y, vez)) {
                jatekosok[i].eletben_van = false;
                animacio_hozzaadasa(HALALFEJ, (short) jatekosok[i].fej.x, (short) jatekosok[i].fej.y, vez);
                ++halottak;
                eletben_levo_jatekosok_pontjanak_novelese_halalkor(jatekosok, vez, halottak);
            }
            else
                jatekosok[i].fej.x = 2.5;
        }
        // Bal --> Jobb
        else if (tulcs_x_min > 0 && tulcs_x_min >= tulcs_y_min && tulcs_x_min >= tulcs_y_max) {
            if (vez->jt_mod != FAL_NELKULI && van_fal_atmenetkor_X((short)jatekosok[i].fej.y, vez)) {
                jatekosok[i].eletben_van = false;
                animacio_hozzaadasa(HALALFEJ, (short) jatekosok[i].fej.x, (short) jatekosok[i].fej.y, vez);
                ++halottak;
                eletben_levo_jatekosok_pontjanak_novelese_halalkor(jatekosok, vez, halottak);
            }
            else
                jatekosok[i].fej.x =  vez->palya_meret.x - 3.5;
        }
        // Fent --> Lent
        else if (tulcs_y_min > 0 && tulcs_y_min >= tulcs_x_min && tulcs_y_min >= tulcs_x_max) {
            if (vez->jt_mod != FAL_NELKULI && van_fal_atmenetkor_Y((short)jatekosok[i].fej.x, vez)) {
                jatekosok[i].eletben_van = false;
                animacio_hozzaadasa(HALALFEJ, (short) jatekosok[i].fej.x, (short) jatekosok[i].fej.y, vez);
                ++halottak;
                eletben_levo_jatekosok_pontjanak_novelese_halalkor(jatekosok, vez, halottak);
            }
            else
                jatekosok[i].fej.y = vez->palya_meret.y - 3.5;
        }
        // Lent--> Fent
        else if (tulcs_y_max > 0 && tulcs_y_max >= tulcs_x_min && tulcs_y_max >= tulcs_x_max) {
            if (vez->jt_mod != FAL_NELKULI && van_fal_atmenetkor_Y((short)jatekosok[i].fej.x, vez)) {
                jatekosok[i].eletben_van = false;
                animacio_hozzaadasa(HALALFEJ, (short) jatekosok[i].fej.x, (short) jatekosok[i].fej.y, vez);
                ++halottak;
                eletben_levo_jatekosok_pontjanak_novelese_halalkor(jatekosok, vez, halottak);
            }
            else
                jatekosok[i].fej.y = 2.5;
        }
    }

    /* Jatekos utkozik egy vonallal + vonal torlese */ //pajzs!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for (int i=0; i < vez->jatekosszam; ++i) {
        if (jatekosok[i].vonal == NULL)
            break;

        Vonal* lemaradoV = NULL;
        Vonal* mozgoV = jatekosok[i].vonal;
        while (mozgoV != NULL) {
            /*** Jatekos halalanak vizsgalata ***/
            for (int i=0; i < vez->jatekosszam; ++i) {
                if (mozgoV == jatekosok[i].vonal)
                    continue;

                if (tav(jatekosok[i].fej, mozgoV->kord) < VON_TAV_HALAL && jatekosok[i].eletben_van && jatekosok[i].spec.tipus != PAJZS) {
                    jatekosok[i].eletben_van = false;
                    animacio_hozzaadasa(HALALFEJ, (short) jatekosok[i].fej.x, (short) jatekosok[i].fej.y, vez);
                    ++halottak;
                    eletben_levo_jatekosok_pontjanak_novelese_halalkor(jatekosok, vez, halottak);
                }
            }

            /*** Vonal torlesenek vizsgalata ***/
            Lovedek* mozgoLov = vez->lovedekek;
            // Megall ha van torlendo vonal vagy ha a lovedekek vegere ert
            while (mozgoLov != NULL) {
                if (tav(mozgoLov->kp, mozgoV->kord) < mozgoLov->sugar)
                    break;
                mozgoLov = mozgoLov->kov;
            }
                
            // Nem torlok, mert nincs torlendo lovedek
            if (mozgoLov == NULL) {
                // Vonal leptetese
                lemaradoV = mozgoV;
                mozgoV = mozgoV->kov;
            }
            // Elso vonalat torlom
            else if (lemaradoV == NULL) {
                Vonal* torlendo = mozgoV;
                mozgoV = mozgoV->kov;
                jatekosok[i].vonal = mozgoV;
                free(torlendo);
            }
            // Kozeperol vagy vegerol torlok vonalat
            else {                    
                lemaradoV->kov = mozgoV->kov;
                free(mozgoV);
                mozgoV = lemaradoV->kov;
            }
        }
    }


    /* Jatekos utkozik lovedekkel */ //pajzs
    for (int i=0; i < vez->jatekosszam; ++i) {
        if (jatekosok[i].eletben_van == false || jatekosok[i].spec.tipus == PAJZS) {
            continue;
        }
        Lovedek* mozgo = vez->lovedekek;
        while (mozgo != NULL) {
            if (tav(jatekosok[i].fej, mozgo->kp) <= mozgo->sugar) {
                jatekosok[i].eletben_van = false;
                animacio_hozzaadasa(HALALFEJ, (short) jatekosok[i].fej.x, (short) jatekosok[i].fej.y, vez);
                ++halottak;
                jatekosok[mozgo->szin].pontszam += 3;
                eletben_levo_jatekosok_pontjanak_novelese_halalkor(jatekosok, vez, halottak);
                break;
            }
            mozgo = mozgo->kov;
        }
    }

    /* Pazsj megol egy masik jatekost */
    for (int i=0; i < vez->jatekosszam; ++i) {
        if (jatekosok[i].spec.tipus != PAJZS || jatekosok[i].eletben_van == false)
            continue;
        for (int j=0; j < vez->jatekosszam; ++j) {
            if (i == j || jatekosok[j].eletben_van == false)
                continue;
            // Ha j. jatekos meghal
            if (tav(jatekosok[i].fej, jatekosok[j].fej) < 10.0) {
                jatekosok[j].eletben_van = false;
                animacio_hozzaadasa(HALALFEJ, (short) jatekosok[j].fej.x, (short) jatekosok[j].fej.y, vez);
                ++halottak;
                eletben_levo_jatekosok_pontjanak_novelese_halalkor(jatekosok, vez, halottak);
            }
        }
    }


    /* Menet vege */
    if (halottak >= vez->jatekosszam-1) {
        vez->menet_vege = true;
        vez->megallitva_jatek = true;
    }
    /* Jatek vege */
    for (int i=0; i < vez->jatekosszam; ++i) {
        if (jatekosok[i].pontszam >= vez->max_elerheto_pontszam) {
            vez->jatek_vege = true;
            vez->megallitva_jatek = true;
            vez->megallitva_felhasznalo = true;

            // Maxpontszam szamitasa
            int max = 0;
            int sum = 0;
            for (int i=0; i < vez->jatekosszam; ++i) {
                if (jatekosok[i].pontszam > max)
                    max = jatekosok[i].pontszam;
                sum += jatekosok[i].pontszam;
            }
            double atlag = (sum-max) / (vez->jatekosszam-1);
            double alap = max - atlag;
            if (alap < 1.0) alap = 1.0;
            double kitevo = 1.0 + 2.0 / (double) vez->jatekosszam;
            double m_pt_szam = pow(alap, kitevo);
            
            //Ellenorzes
            if (m_pt_szam < 1) m_pt_szam = 1;
            else if (m_pt_szam > 9999) m_pt_szam = 9999;

            vez->max_pontszam = (int)m_pt_szam;
        }
    }
}

static void loves_hozzaadasa(double sugar, Vezerles* vez, Jatekos* jat, double irany) {
    Lovedek* uj_eleje = (Lovedek*) malloc(sizeof(Lovedek));
    if (!uj_eleje) {
        printf("Nem sikerult memoriat fogalalni a lovedeknek! :c\n");
        return;
    }

    Koordinata kp = (Koordinata) {jat->fej.x + 1.5 * sugar * cos(jat->irany), jat->fej.y + 1.5 * sugar * sin(jat->irany)};
    uj_eleje->kp = kp;
    uj_eleje->irany = irany;
    uj_eleje->sugar = sugar;
    uj_eleje->szin = jat->szin;

    uj_eleje->kov = vez->lovedekek;
    vez->lovedekek = uj_eleje;
}

void loves_vizsgalata(Jatekos* jatekosok, Vezerles* vez) {
    for (int i=0; i < vez->jatekosszam; ++i) {
        if (jatekosok[i].eletben_van == false)
            continue;

        if (*(jatekosok[i].lo) && !(*(jatekosok[i].tilt_lo)) && ((jatekosok[i].pontszam >= 6) || jatekosok[i].spec.tipus == NAGY_LOV || jatekosok[i].spec.tipus == SOK_LOV)){
            *(jatekosok[i].tilt_lo) = true;
            /* Ha van specialis lovese */
            // Nagy loves
            if (jatekosok[i].spec.tipus == NAGY_LOV) {
                loves_hozzaadasa(30.0, vez, &jatekosok[i], jatekosok[i].irany);
                jatekosok[i].spec.tipus = NINCS;
            }
            // Sok loves
            else if (jatekosok[i].spec.tipus == SOK_LOV) {
                // Kozep - 60 fok
                loves_hozzaadasa(15.0, vez, &jatekosok[i], jatekosok[i].irany - RADIAN(60.0));
                // Kozep - 45 fok
                loves_hozzaadasa(15.0, vez, &jatekosok[i], jatekosok[i].irany - RADIAN(45.0));
                // Kozep - 30 fok
                loves_hozzaadasa(15.0, vez, &jatekosok[i], jatekosok[i].irany - RADIAN(30.0));
                // Kozep - 15 fok
                loves_hozzaadasa(15.0, vez, &jatekosok[i], jatekosok[i].irany - RADIAN(15.0));
                // Kozep
                loves_hozzaadasa(15.0, vez, &jatekosok[i], jatekosok[i].irany);
                // Kozep + 15 fok
                loves_hozzaadasa(15.0, vez, &jatekosok[i], jatekosok[i].irany + RADIAN(15.0));
                // Kozep + 30 fok
                loves_hozzaadasa(15.0, vez, &jatekosok[i], jatekosok[i].irany + RADIAN(30.0));
                // Kozep + 45 fok
                loves_hozzaadasa(15.0, vez, &jatekosok[i], jatekosok[i].irany + RADIAN(45.0));
                // Kozep + 60 fok
                loves_hozzaadasa(15.0, vez, &jatekosok[i], jatekosok[i].irany + RADIAN(60.0));

                jatekosok[i].spec.tipus = NINCS;
            }

            /* Normal loves */
            else {
                jatekosok[i].pontszam -= 6;
                loves_hozzaadasa(15.0, vez, &jatekosok[i], jatekosok[i].irany);
            }
        }
    }
}

static void felveheto_elem_hozzaadasa(Vezerles* vez, Jatekos* jatekosok) {
    FelvehetoElem* uj_eleje = (FelvehetoElem*) malloc(sizeof(FelvehetoElem));
    if (!uj_eleje) {
        printf("Nem sikerult memoriat foglalni felveheto elemnek! :c\n");
        return;
    }

    /* Kozpeppont szamitasa */
    // Random seed
    srand(time(NULL));
    Pixel kord;
    bool jo_koordinata;
    do
    {
        jo_koordinata = true;
        kord = randPixel();

        /* Megfelelo tavolsag biztositasa */
        // Jatekosoktol
        for (int i=0; i < vez->jatekosszam; ++i) {
            if (tav(jatekosok[i].fej, (Koordinata) {(double)kord.x, (double)kord.y}) < 200.0) {
                jo_koordinata = false;
                break;
            }
        }
        // Tobbi buborektol
        FelvehetoElem* mozgo = vez->buborekok;
        while (mozgo != NULL) {
            if (tav((Koordinata) {(double)mozgo->kp.x, (double)mozgo->kp.y}, (Koordinata) {(double)kord.x, (double)kord.y}) < 200.0) {
                jo_koordinata = false;
                break;
            }

            mozgo = mozgo->kov;
        }

    } while (!jo_koordinata);
    
    uj_eleje->kp = kord;
    uj_eleje->elet_tartam = 15.0;
    
    uj_eleje->kov = vez->buborekok;
    vez->buborekok = uj_eleje;
}

void felveheto_elemek_kezelese(Vezerles* vez, Jatekos* jatekosok) {
    if (vez->jt_mod == FELV_E_T)
        return;
    
    /* Lejart buborekok torlese */
    FelvehetoElem* mozgo = vez->buborekok;
    FelvehetoElem* lemarado = NULL;
    while (mozgo != NULL) {
        // Torolni kell
        if (mozgo->elet_tartam < 0) {
            // Elso elemet
            if (lemarado == NULL) {
                FelvehetoElem* torlendo = mozgo;
                mozgo = mozgo->kov;
                vez->buborekok = mozgo;
                free(torlendo);
            }
            // Kozeperol/vegerol torlok
            else {
                FelvehetoElem* torlendo = mozgo;
                mozgo = mozgo->kov;
                lemarado->kov = mozgo;
                free(torlendo);
            }
        }
        // Nem torlok
        else {
            lemarado = mozgo;
            mozgo = mozgo->kov;
        }
    }

    /* Jatekos felveszi az elemet */
    for (int i=0; i < vez->jatekosszam; ++i) {
        mozgo = vez->buborekok;
        lemarado = NULL;
        while(mozgo != NULL) {
            // Buborekon belül van
            if (tav(jatekosok[i].fej, (Koordinata){(double)mozgo->kp.x, (double)mozgo->kp.y}) < 37.5) {
                /* Kitorlom a buborekot */
                // Elso elemet torlom
                if (lemarado == NULL) {
                    FelvehetoElem* torlendo = mozgo;
                    mozgo = mozgo->kov;
                    vez->buborekok = mozgo;
                    free(torlendo);
                }
                // Kozeperol/vegerol torlok
                else {
                    FelvehetoElem* torlendo = mozgo;
                    mozgo = mozgo->kov;
                    lemarado->kov = mozgo;
                    free(torlendo);
                }

                /* Elem hozzaadasa */
                srand(time(NULL));
                jatekosok[i].spec.tipus = (rand() % 3) + 1;
                
                // Töröld ki!!! @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@v
                //jatekosok[i].spec.tipus = PAJZS;
                switch (jatekosok[i].spec.tipus) {
                    case NAGY_LOV: printf("Nagy lövés\n"); break;
                    case SOK_LOV: printf("Sok lövés\n"); break;
                    case PAJZS: printf("Pajzs\n"); break;
                    default: printf("Hibás típus\n"); break;
                }
                //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

                if (jatekosok[i].spec.tipus == PAJZS)
                    jatekosok[i].spec.elet_tartam = 15.0;

                break;
            }
            // Buborekon kivul van
            else {
                lemarado = mozgo;
                mozgo = mozgo->kov;
            }
        }


    }

    /* Lejart pajzs eltavolitasa es meglevo elettartamanak csokkentese*/
    for (int i=0; i < vez->jatekosszam; ++i) {
        if (jatekosok[i].spec.tipus == PAJZS) {
            jatekosok[i].spec.elet_tartam -= 0.02;

            if (jatekosok[i].spec.elet_tartam < 0.0)
                jatekosok[i].spec.tipus = NINCS;
        }
    }

    /* Elettartam csokkentese */
    mozgo = vez->buborekok;
    while (mozgo != NULL) {
        mozgo->elet_tartam -= 0.02;
        mozgo = mozgo->kov;
    }
    
    /* Felveheto elem (buborek) hozzaadasa */
    if (vez->menetido % 500 == 200) {
        felveheto_elem_hozzaadasa(vez, jatekosok);
    }
}

void vonalat_hozzaad(Jatekos* jatekosok, Vezerles* vez) {
    if (vez->menetido % GYAK == 0) {
        int vonal_ido = vez->menetido % (GYAK * 20);
        bool novelve = false;

        for (int i=0; i < vez->jatekosszam; ++i) {
            if (jatekosok[i].eletben_van == false)
                continue;

            // Vonal eleje es vege (vegpontok)
            if (vonal_ido == 0 || vonal_ido == GYAK * (20 - 3)) {
                novelve = true;

                Vonal* uj_eleje = (Vonal*) malloc(sizeof(Vonal));
                if (!uj_eleje) printf("Nem sikerult memoriat fogalalni a vonalnak! :c\n");

                uj_eleje->kord = jatekosok[i].elozo;
                uj_eleje->szin = jatekosok[i].szin;
                // Negativ jeloli a vonal vegpontjat
                uj_eleje->id = (-1) * vez->vonal_szamlalo;

                uj_eleje->kov = jatekosok[i].vonal;
                jatekosok[i].vonal = uj_eleje;

                // Lyuk hozzaadasa
                if (vonal_ido == 0 && jatekosok[i].vonal != NULL && jatekosok[i].vonal->id != -1) {
                    if (jatekosok[i].vonal->id + 1 == jatekosok[i].vonal->kov->id) {
                        Lyuk* uj_lyuk = (Lyuk*) malloc(sizeof(Lyuk));
                        if (!uj_lyuk) printf("Nem sikerult memoriat fogalalni a lyuknak! :c\n");

                        uj_lyuk->eleje = jatekosok[i].vonal->kord;
                        uj_lyuk->vege = jatekosok[i].vonal->kov->kord;

                        uj_lyuk->kov = vez->lyukak;
                        vez->lyukak = uj_lyuk;
                    }
                }
            }
            // Vonal kozepe
            else if (vonal_ido < GYAK * (20 - 3)) {
                novelve = true;

                Vonal* uj_eleje = (Vonal*) malloc(sizeof(Vonal));
                if (!uj_eleje) printf("Nem sikerult memoriat fogalalni a vonalnak! :c\n");

                uj_eleje->kord = jatekosok[i].elozo;
                uj_eleje->szin = jatekosok[i].szin;
                // Pozitiv jeloli a vonal kozepet
                uj_eleje->id = vez->vonal_szamlalo;

                uj_eleje->kov = jatekosok[i].vonal;
                jatekosok[i].vonal = uj_eleje;
            }
            // Ket szuntet hagyunk a lyuknak
        }
        if (novelve)
            ++vez->vonal_szamlalo;
    }
}

void felveheto_elemeket_torol(Vezerles* vez) {
    FelvehetoElem* iter = vez->buborekok;
    while (iter != NULL) {
        FelvehetoElem* torlendo = iter;
        iter = iter->kov;
        free(torlendo);
    }
    vez->buborekok = NULL;
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

void vonalakat_torol(Jatekos* jatekosok, Vezerles* vez) {
    if (jatekosok != NULL && jatekosok->vonal != NULL) {
        for (int i=0; i < vez->jatekosszam; ++i) {
            Vonal* iter = jatekosok[i].vonal;
            while (iter != NULL) {
                Vonal* kov = iter->kov;
                free(iter);
                iter = kov;
            }
            jatekosok[i].vonal = NULL;
        }
    }
}

void lyukakat_torol(Vezerles* vez) {
    Lyuk* iter = vez->lyukak;
    while (iter != NULL) {
        Lyuk* kov = iter->kov;
        free(iter);
        iter = kov;
    }
    vez->lyukak = NULL;
}

static bool kint_van(Koordinata kord, double sugar, Vezerles* vez) {
    return 0 > kord.x+sugar || kord.x-sugar >=vez->palya_meret.x || 0 > kord.y+sugar || kord.y-sugar >=vez->palya_meret.y;
}

void falak_es_lovekek_torlesenek_vizsgalata(Vezerles* vez) {
    Lovedek* lemarado = NULL;
    Lovedek* mozgo = vez->lovedekek;
    // Megall a torlendo elemnel
    while (mozgo != NULL && !kint_van(mozgo->kp, mozgo->sugar, vez)) {
        lemarado = mozgo;
        mozgo = mozgo->kov;
    }    
        
    if (mozgo == NULL) {
        // Nem torlok, mert nincs torlendo lovedek
    } else if (lemarado == NULL) {
        // Elso elemet torlom
        Lovedek* ujeleje = mozgo->kov;
        free(mozgo);
        vez->lovedekek = ujeleje;
    } else {
        // Kozeperol vagy vegerol torlok
        lemarado->kov = mozgo->kov;
        free(mozgo);
    }
    
    /* Fal pixeinek tolese lovedekkel valo utkozeskor */
    if (vez->jt_mod == FAL_NELKULI) 
        return;
    for (mozgo = vez->lovedekek; mozgo != NULL; mozgo = mozgo->kov) {
        // Felso
        if (mozgo->kp.y - mozgo->sugar < 2.0) {
            short min = (short) (mozgo->kp.x - mozgo->sugar);
            short max = (short) (mozgo->kp.x + mozgo->sugar);
            if (min < 0) min = 0;
            if (max >= vez->palya_meret.x) max = vez->palya_meret.x - 1;

            for (short i=min; i<max ; ++i) {
                if (tav(mozgo->kp, (Koordinata) {(double)vez->falak.felso[i].pixel.x, (double)vez->falak.felso[i].pixel.y}) <= mozgo->sugar) {
                    vez->falak.felso[i].torolve = true;
                }
            }
        }

        // Also
        if (mozgo->kp.y + mozgo->sugar > vez->palya_meret.y - 2.0) {
            short min = (short) (mozgo->kp.x - mozgo->sugar);
            short max = (short) (mozgo->kp.x + mozgo->sugar);
            if (min < 0) min = 0;
            if (max >= vez->palya_meret.x) max = vez->palya_meret.x - 1;

            for (short i=min; i<max ; ++i) {
                if (tav(mozgo->kp, (Koordinata) {(double)vez->falak.also[i].pixel.x, (double)vez->falak.also[i].pixel.y}) <= mozgo->sugar) {
                    vez->falak.also[i].torolve = true;
                }
            }
        }

        // Bal
        if (mozgo->kp.x - mozgo->sugar < 2.0) {
            short min = (short) (mozgo->kp.y - mozgo->sugar);
            short max = (short) (mozgo->kp.y + mozgo->sugar);
            if (min < 0) min = 0;
            if (max >= vez->palya_meret.y) max = vez->palya_meret.y - 1;

            for (short i=min; i<max ; ++i) {
                if (tav(mozgo->kp, (Koordinata) {(double)vez->falak.bal[i].pixel.x, (double)vez->falak.bal[i].pixel.y}) <= mozgo->sugar) {
                    vez->falak.bal[i].torolve = true;
                }
            }
        }

        // Jobb
        if (mozgo->kp.x + mozgo->sugar > vez->palya_meret.x - 2.0) {
            short min = (short) (mozgo->kp.y - mozgo->sugar);
            short max = (short) (mozgo->kp.y + mozgo->sugar);
            if (min < 0) min = 0;
            if (max >= vez->palya_meret.y) max = vez->palya_meret.y - 1;

            for (short i=min; i<max ; ++i) {
                if (tav(mozgo->kp, (Koordinata) {(double)vez->falak.jobb[i].pixel.x, (double)vez->falak.jobb[i].pixel.y}) <= mozgo->sugar) {
                    vez->falak.jobb[i].torolve = true;
                }
            }
        }
    }
}
/*
static bool athaladt_a_lyukon (Jatekos* jatekos, Vezerles* vez) {

}
*/
void lyuk_vizsgalata(Jatekos* jatekosok, Vezerles* vez) {
    /* Lyuk lovedek altali torlese */
    Lovedek* mozgoLov = vez->lovedekek;
    while (mozgoLov != NULL) {
        Lyuk* mozgoLyuk = vez->lyukak;
        Lyuk* lemaradoLyuk = NULL;
        
        while (mozgoLyuk != NULL) {
            // Kell torolni
            if (tav(mozgoLyuk->eleje, mozgoLov->kp) < mozgoLov->sugar || tav(mozgoLyuk->vege, mozgoLov->kp) < mozgoLov->sugar) {
                // Elejerol torlok
                if (lemaradoLyuk == NULL) {
                    Lyuk* torlendo = mozgoLyuk;
                    mozgoLyuk = mozgoLyuk->kov;
                    vez->lyukak = mozgoLyuk;
                    free(torlendo);
                }
                // Kozeperol vagy vegerol torlunk
                else {
                    Lyuk* torlendo = mozgoLyuk;
                    mozgoLyuk = mozgoLyuk->kov;
                    lemaradoLyuk->kov = mozgoLyuk;
                    free(torlendo);
                }
            }
            // Nem torlok, leptetes
            else {
                lemaradoLyuk = mozgoLyuk;
                mozgoLyuk = mozgoLyuk->kov;
            }
        }
        mozgoLov = mozgoLov->kov;
    }
    
    /* Lyukon valo athaladas vizsgalata */
    for (int i=0; i < vez->jatekosszam; ++i) {
        Lyuk* mozgo = vez->lyukak;
        while (mozgo != NULL) {
            if (tav(mozgo->eleje, jatekosok[i].fej) < 15.0 && tav(mozgo->vege, jatekosok[i].fej) < 15.0 && jatekosok[i].lyuk_tilt < vez->menetido && jatekosok[i].eletben_van && jatekosok[i].spec.tipus != PAJZS) {
                jatekosok[i].pontszam += 1;
                jatekosok[i].lyuk_tilt = vez->menetido + 20;
                
                // Animacio
                switch (jatekosok[i].szin) {
                    case PIROS: animacio_hozzaadasa(PIROSPLUSZ1, (short) jatekosok[i].fej.x, (short) jatekosok[i].fej.y, vez); break;
                    case ZOLD: animacio_hozzaadasa(ZOLDPLUSZ1, (short) jatekosok[i].fej.x, (short) jatekosok[i].fej.y, vez); break;
                    case KEK: animacio_hozzaadasa(KEKPLUSZ1, (short) jatekosok[i].fej.x, (short) jatekosok[i].fej.y, vez); break;
                    case ROZSA: animacio_hozzaadasa(ROZSAPLUSZ1, (short) jatekosok[i].fej.x, (short) jatekosok[i].fej.y, vez); break;
                    default: printf("Hibas szin +1 animacio hozzaadasanak (lyuk)!\n");
                }

                break;
            }
            mozgo = mozgo->kov;
        }
    }
}

SDL_Texture* textura_betoltese(char* nev, Ablak* jatekablak) {
    SDL_Texture* textura = IMG_LoadTexture(jatekablak->megjelenito, nev);
    if (!textura) {
        SDL_Log("Nem sikerult megnyitni az animacio texturajat! %s\n", TTF_GetError());
        exit(1);
    }
    return textura;
}

void animacio_texturak_bezarasa(Vezerles* vez) {
    // Halalfej
    if (vez->ani.halalfej != NULL)
        SDL_DestroyTexture(vez->ani.halalfej);
    // Piros '+1'
    if (vez->ani.pirosPluszEgy != NULL)
        SDL_DestroyTexture(vez->ani.pirosPluszEgy);
    // Zold '+1'
    if (vez->ani.zoldPluszEgy != NULL)
        SDL_DestroyTexture(vez->ani.zoldPluszEgy);
    // Kek '+1'
    if (vez->ani.kekPluszEgy != NULL)
        SDL_DestroyTexture(vez->ani.kekPluszEgy);
    // Rozsaszin '+1'
    if (vez->ani.rozsaPluszEgy != NULL)
        SDL_DestroyTexture(vez->ani.rozsaPluszEgy);
    // Felveheto elem (buborek)
    if (vez->ani.buborek != NULL)
        SDL_DestroyTexture(vez->ani.buborek);
}

void animaciok_kezelese(Vezerles* vez) {
    /* Lejart animaciok torlese */
    Animacio* mozgo = vez->animaciok;
    Animacio* lemarado = NULL;
    while (mozgo != NULL) {
        // Torolni kell
        if (mozgo->elet_tartam < 0) {
            // Elso elemet
            if (lemarado == NULL) {
                Animacio* torlendo = mozgo;
                mozgo = mozgo->kov;
                vez->animaciok = mozgo;
                free(torlendo);
            }
            // Kozeperol/vegerol torlok
            else {
                Animacio* torlendo = mozgo;
                mozgo = mozgo->kov;
                lemarado->kov = mozgo;
                free(torlendo);
            }
        }
        // Nem torlok
        else {
            lemarado = mozgo;
            mozgo = mozgo->kov;
        }
    }

    /* Animaciok transzformalasa */
    mozgo = vez->animaciok;
    while (mozgo != NULL) {
        mozgo->elet_tartam -= 0.02;
        mozgo->poz.y -= 1;

        mozgo = mozgo->kov;
    }
}

void animaciokat_torol(Vezerles* vez) {
    Animacio* iter = vez->animaciok;
    while (iter != NULL) {
        Animacio* torlendo = iter;
        iter = iter->kov;
        free(torlendo);
    }
    vez->animaciok = NULL;
}
// Vege ********************************************************