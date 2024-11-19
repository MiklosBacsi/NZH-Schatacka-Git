/**  @file bemenet_es_fajlkezeles.h
  *  @brief Bemenet- és fájlkezelés
  *  
  *  Ez a fájl tartalmazza bool változókként a billenytűállapotot, illetve az
  *  azok rögzítéséhez szükséges függvényeket. Ezen kívül itt találhatók a
  *  fájlkezeléshez szükséges adatstruktúrák, függvények is. (MÉG NEM!!!)
  *
  *  @author Bácsi Miklós
  *  @date 2024-11-10
*/
#ifndef BEMENET_ES_FAJLKEZELES_H
#define BEMENET_ES_FAJLKEZELES_H

#include "SDL2/SDL.h"
#include "debugmalloc.h"
#include <stdbool.h>

/**
 * @brief billentyű válozók
 * 
 * A struktúra tartalmazza az egyes ablakokhoz tartozó billentyűk lenyomását jelző
 * bool változókat, illetve az azokhoz tartalmazó tiltó változókat is a szükséges
 * helyeken, amik a bizonyos gombok egyszeri lenyomásának jelzésére szolgál
 */
typedef struct Billentyuk {
    bool menu_Esc;      ///< Esc gomb a menüben (bool)
    bool jatek_Esc;     ///< Esc gomb a játékban (bool)
    bool sugo_Esc;      ///< Esc gomb a súgóban (bool)
    bool dics_Esc;      ///< Esc gomb a dicsőség listában (bool)

    bool menu_F1;       ///< F1 gomb a menüben (bool)
    bool menu_F2;       ///< F2 gomb a menüben (bool)
    bool menu_F3;       ///< F2 gomb a menüben (bool)
    bool menu_F10;      ///< F10 gomb a menüben (bool)
    bool menu_F11;      ///< F11 gomb a menüben (bool)

    bool jatek_A;       ///< 'A' gomb a játékban (bool)
    bool jatek_W;       ///< 'W' gomb a játékban (bool)
    bool jatek_D;       ///< 'D' gomb a játékban (bool)

    bool jatek_J;       ///< 'J' gomb a játékban (bool)
    bool jatek_I;       ///< 'I' gomb a játékban (bool)
    bool jatek_L;       ///< 'L' gomb a játékban (bool)
    
    bool jatek_Bal;     ///< Bal nyíl a játékban (bool)
    bool jatek_Fel;     ///< Fel nyíl a játékban (bool)
    bool jatek_Jobb;    ///< Jobb nyíl a játékban (bool)

    bool jatek_7;       ///< '7' gomb a játékban (NUMPAD) (bool)
    bool jatek_Per;     ///< '/' gomb a játékban (NUMPAD) (bool)
    bool jatek_9;       ///< '9' gomb a játékban (NUMPAD) (bool)
    bool menu_Szokoz;   ///< szóköz gomb a menüben (bool)
    bool jatek_Szokoz;  ///< szóköz gomb a játékban (bool)
    
    // A billentyuk egyszeri megnyomasahoz
    bool tilt_Esc;        ///< Esc gomb tiltása a játékban (egyszeri megnyomáshoz) (bool)
    bool tilt_W;          ///< 'W' gomb tiltása a játékban (egyszeri megnyomáshoz)
    bool tilt_I;          ///< 'I' gomb tiltása a játékban (egyszeri megnyomáshoz) (bool)
    bool tilt_Fel;        ///< Fel nyíl tiltása a játékban (egyszeri megnyomáshoz) (bool)
    bool tilt_Per;        ///< '/' gomb tiltása a játékban (egyszeri megnyomáshoz, NUMPAD) (bool)

    bool menu_A;        ///< 'A' gomb a menüben (bool)
    bool menu_J;        ///< 'J' gomb a menüben (bool)
    bool menu_Bal;      ///< bal nyíl gomb a menüben (bool)
    bool menu_7;        ///< '7' a menüben (NUMPAD) (bool)

    bool menu_tilt_A;   ///< 'A' gomb tiltása a menüben (egyszeri megnyomáshoz) (bool)
    bool menu_tilt_J;   ///< 'J' gomb tiltása a menüben (egyszeri megnyomáshoz) (bool)
    bool menu_tilt_Bal; ///< Bal nyíl gomb tiltása a menüben (egyszeri megnyomáshoz) (bool)
    bool menu_tilt_7;   ///< '7' gomb tiltása a menüben (egyszeri megnyomáshoz, NUMPAD) (bool)

    // Megfelelo kepfrissiteshez
    bool van_bemenet;   ///< jelzi, ha van bemenet (időzítő) (bool)
} Billentyuk;


/**
 * @brief Dicsőséglista top 3 pontszáma bizonyos játékosszám esetén (Játék végekor)
 */
typedef struct Pontszamok {
  int elso;     ///< 1. helyezett pontszáma (int)
  int masodik;  ///< 2. helyezett pontszáma (int)
  int harmadik; ///< 3. helyezett pontszáma (int)
} Pontszamok;


/**
 * @brief Dicsőséglista top 3 topszámai az egyes játékosszámokhoz (Játék végekor)
 */
typedef struct OsszesPontszam {
  Pontszamok ketJatekos;    ///< 2 játékos esetén top 3 pontszám (Pontszamok) @see Pontszamok
  Pontszamok haromJatekos;  ///< 3 játékos esetén top 3 pontszám (Pontszamok) @see Pontszamok
  Pontszamok negyJatekos;   ///< 4 játékos esetén top 3 pontszám (Pontszamok) @see Pontszamok
} OsszesPontszam;


/**
 * @brief Billentyűk érzékelése
 *
 * Ez a függvény a bill struktúra bool változóiban eltárolja a jelenlegi billentyűállapotot
 * Ha az adott ablakon a billentyű le van nyomva, akkor az értéke igaz (true), különben
 * hamis (false).
 * Billentyű felengedésekor az adott változó értékét hamisra állítjuk be.
 * 
 * A bill struktúra van_bemenet változója az SDL könyvtár által biztosított időzítő
 * jelére igaz állapotba kerül.
 * 
 * A menüből való kilépés esetén a bill struktúra menu_Esc változóját igazra állítja,
 * ami a főprogram (main.c) while ciklusát terminálja.
 *  
 * @param[in] event interaktálás a programmal és időzítő (SDL_Event*)
 * @param[out] bill billentyűk lenyomásának állapotai (Billentyuk*) @see Billentyuk
 * @param[in] ablakIDk ablakok azonosítói (Uint32*)
 */
void billentyuk_erzekelese(SDL_Event* event, Billentyuk* bill, Uint32* ablakIDk);


/**
 * @brief Billentyűk tiltása egyszeri lenyomáshoz
 * 
 * Azoknak a billentyűknek, amelyek rendelkeznek tiltással (megnyomás esetén) a hozzájuk
 * rendelt tiló változókat igazra (true) állítja.
 * Az összes változó a bill struktúrában van tárolva.
 *  
 * @param[out] bill billentyuk lenyomásának állapotai (Billentyuk*) @see Billentyuk
 */
void billentyuk_tiltasa(Billentyuk* bill);


/**
 * @brief Fájlból beolvassa a régi pontszámokat, és visszatér egy azokat tartalmazó struktúrával
 *  
 * @return Összes pontszámot tartalmazó struktúra (OsszesPontszam) @see OsszesPontszam
 */
OsszesPontszam regi_pontszamokat_betolt();


/**
 * @brief Játék végekor frissíti a pontszámokat az nyertes pontszámával
 * 
 * A függvény felülírja a pontszámokat tartalmazó txt fájlt
 * 
 * @param[in] max_pontszam Nyertes pontszáma a játék végén (int)
 * @param[in] jatekos_szam Kiválasztott játékosok száma (int)
 * @param[in] pt Összes régi pontszámot tartalmazó struktúra (OsszesPontszam) @see OsszesPontszam
 */
void pontszamok_frissitese(int max_pontszam, int jatekos_szam, const OsszesPontszam* pt);

#endif