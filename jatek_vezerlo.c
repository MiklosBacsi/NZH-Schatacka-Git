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

    // Jobb savot ujrarajzolom, hogy a lovedekek ne latszodjanak rajta
    boxRGBA(jatek_ablak->megjelenito, 1400, 0, 1600, 900, 30, 30, 30, 255);

    // Hogy fal nelkuli jatekmodban latszodjon a palya szele
    if (vez->jt_mod == FAL_NELKULI) {
        boxRGBA(jatek_ablak->megjelenito, 1400, 0, 1401, 900, 255, 255, 255, 255);
    }

    /* Pontszamok */
    for (int i=0; i < vez->jatekosszam; ++i) {
        char pontszam[3+1];
        snprintf(pontszam, 3+1, "%d", jatekosok[i].pontszam);
        szoveget_kiir(pontszam, 1500, 100 + i*50, SDL_Szin[jatekosok[i].szin], SZURKE_SDL, bt->bold20, jatek_ablak->megjelenito, false);
    }

    // Logo
    logot_rajzol(jatek_ablak, 1425, 10);

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
        /* Jatekos kimegy a palyarol - kijon a masik oldalon */
        short tulcs_x_min = 0 - (short) jatekosok[i].fej.x;
        short tulcs_x_max = (short) jatekosok[i].fej.x - vez->palya_meret.x;
        short tulcs_y_min = 0 - (short) jatekosok[i].fej.y;
        short tulcs_y_max = (short) jatekosok[i].fej.y - vez->palya_meret.y;
        
        // Jobb --> Bal
        if (tulcs_x_max > 0 && tulcs_x_max >= tulcs_y_min && tulcs_x_max >= tulcs_y_max) {
            jatekosok[i].fej.x = 0.5;
        }
        // Bal --> Jobb
        else if (tulcs_x_min > 0 && tulcs_x_min >= tulcs_y_min && tulcs_x_min >= tulcs_y_max) {
            jatekosok[i].fej.x =  vez->palya_meret.x - 1.5;
        }
        // Fent --> Lent
        else if (tulcs_y_min > 0 && tulcs_y_min >= tulcs_x_min && tulcs_y_min >= tulcs_x_max) {
            jatekosok[i].fej.y = vez->palya_meret.y - 1.5;
        }
        // Lent--> Fent
        else if (tulcs_y_max > 0 && tulcs_y_max >= tulcs_x_min && tulcs_y_max >= tulcs_x_max) {
            jatekosok[i].fej.y = 0.5;
        }

        
        //if (jatekosok[i].fej.x < 0 || jatekosok[i].fej.x >= 1400 || jatekosok[i].fej.y < 0 || jatekosok[i].fej.y >= 900) {
            
        //jatekosok[i].eletben_van = false;
        // halalfej animaciot rajzol !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        /* Jatekos utkozik a fallal */ //jatekmod!
        /*
        JÓ:
        if (vez->jt_mod != FAL_NELKULI && (jatekosok[i].fej.x < 2.0 || jatekosok[i].fej.x > 1398.0 || jatekosok[i].fej.y < 2.0 || jatekosok[i].fej.y > 898))
            jatekosok[i].eletben_van = false;
        */

        /* Jatekos utkozik egy vonallal */ //pajzs

        /* Jatekos utkozik lovedekkel */ //pajzs


    }
    if (halottak >= vez->jatekosszam-1) {
        vez->menet_vege = true;
        vez->megallitva_jatek = true;
        // Szóközzel kell jatekot inditani szoveg!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
}

static Lovedek* loves_hozzaadasa(double sugar, Lovedek* lov_eleje, Jatekos* jat) {
    Lovedek* uj_eleje = (Lovedek*) malloc(sizeof(Lovedek));
    if (!uj_eleje) printf("Nem sikerult memoriat fogalalni a lovedeknek! :c\n");

    Koordinata kp = (Koordinata) {jat->fej.x + 1.5 * sugar * cos(jat->irany), jat->fej.y + 1.5 * sugar * sin(jat->irany)};
    uj_eleje->kp = kp;
    uj_eleje->irany = jat->irany;
    uj_eleje->sugar = sugar;
    uj_eleje->szin = jat->szin;

    uj_eleje->kov = lov_eleje;

    return uj_eleje;
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
            vez->lovedekek = loves_hozzaadasa(20.0, vez->lovedekek, &jatekosok[i]);
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