/** @file ablak_kezelo.h
  * @brief SDL ablakokat kezeli (megnyitás, bezárás, kirajzolás stb.)
  *  
  * Ez a fájl tartalmazza a grafikus megjelenítéshez szükséges adattípusokat és függvényeket.
  *
  * @author Bácsi Miklós
  * @date 2024-11-10
*/
#ifndef ABLAK_KEZELO_H
#define ABLAK_KEZELO_H

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "bemenet_es_fajlkezeles.h"
#include <stdlib.h>
#include <stdbool.h>
#include "debugmalloc.h"

/**
 * @brief Ablak típusát jelöli
 * 
 * Ez az adattípus ahhoz szükséges, hogy a lenyomott billenytűről megállíthassuk, hogy
 * melyik ablakban volt lenyomva, mert eltérő funkcionalitással rendelkeznek.
 */
typedef enum Ablak_tipus {
    MENU = 0,   ///< Menü ablak jelölése
    JATEK,      ///< Játék ablak
    SUGO,       ///< Súgó ablak
    DICS_LISTA  ///< Dicsőséglista ablak
} Ablak_tipus;


/**
 * @brief Eltárolja az ablakok paramétereit, illetve más fontos adatot is
 * 
 * A struktúra tartalmazza az egyes ablakokhoz tartozó fontos paramétereket, illetve
 * egy, a játék logójának kirajzolásához szükséges mutatót.
 */
typedef struct Ablak {
    SDL_Window* ablak;          ///< Ablak struktúra ablaka (ami megnyílik) (SDL_Window*)
    SDL_Renderer* megjelenito;  ///< Albak struktúra megjelenítője (amire rajzolunk) (SDL_Renderer*)
    int szelesseg;              ///< az ablak szélességét tárolja (int)
    int magassag;               ///< az ablak magasságát tárolja (int)
    char* cim;                  ///< az ablak címét tarolja (char*)
    Ablak_tipus abl_tip;        ///< az ablak típusa (Ablak_tipus) @see Ablak_tipus
    SDL_Texture* logo;          ///< logók az egyes ablakokra való kirajzolásához (SDL_Texture*)
    bool nyitva;                ///< ablak státuszát jelzi nyitva/bezárva (bool)
} Ablak;


/**
 * @brief Betűtípusok
 * 
 * A struktúra tartalmazza az egyes betűtípusokra mutató pointereket. Ezek segítségével
 * tudunk szöveget megjelíteni a kijelzőn.
 */
typedef struct Betutipusok{
    TTF_Font* med20;    ///< OpenSans Medium betűtípus, 20 pont (TTF_Font*)
    TTF_Font* reg15;    ///< OpenSans Regular betűtípus, 15 pont (TTF_Font*)
    TTF_Font* reg20;    ///< OpenSans Regular betűtípus, 20 pont (TTF_Font*)
    TTF_Font* bold20;   ///< OpenSans Bold betűtípus, 20 pont (TTF_Font*)
} Betutipusok;


/**
 * @brief Színek jelölése
 * 
 * A struktúra tartalmazza a különböző színeket, amelyekkel a játékosokat azonosítjuk,
 * illettve a kirajzolásnál játszik még fontos szerepet.
 */
typedef enum Szin {
    PIROS = 0,  ///< Piros szín jelölése
    ROZSA,      ///< Rózsaszín szín jelölése
    ZOLD,       ///< Zöld szín jelölése
    KEK,        ///< Kék szín jelölése
    SZURKE,     ///< Szürke szín jelölése
    FEHER,      ///< Fehér szín jelölése
    FEKETE      ///< Fekete szín jelölése
} Szin;


/**
 * @brief Megnyitja a paraméterként kapott ablakot
 *
 * Megnyitja a paraméterként kapott ablakot, és az ablak nyitva bool változáját igazra
 * állítja.
 *  
 * @param[out] ablak  (Ablak*) @see Ablak
 */
void ablakot_letrehoz (Ablak* ablak);


/**
 * @brief Logót rajzol egy megjelenítőre
 *
 * A paraméterként fogadott ablak megjelenítőjére rajzol logót, amelyre mutató pointert
 * szintén eltárol ugyan annak a paraméterként fogadott ablakban.
 * 
 * @param[out] ablak ennek az ablaknak a megjelenítőjere rajzol logót (*Ablak) @see Ablak
 * @param x kirajzolandó logó bal felső sarkának X koordinátája (int)
 * @param y kirajzolandó logó bal felső sarkának Y koordinátája (int)
 */
void logot_rajzol(Ablak* ablak, int x, int y);


/**
 * @brief Szöveget ír a megjelenítőre
 *
 * Ez a függvény lehetővé teszi, hogy szöveget írjunk ki megjelenítőre a megadott
 * paraméterek segítségével.
 *  
 * @param[in] szoveg kiírandó szöveg (char*)
 * @param x a kíírandó szöveg szövegdobozának bal felső sarkának X koordinátája (int)
 * @param y a kíírandó szöveg szövegdobozának bal felső sarkának Y koordinátája (int)
 * @param betu_szin kíírandó szöveg színe (SDL_Color)
 * @param hatter_szin szöveg hátterszíne, engedélyezhető az utolsó paraméterrel (SDL_Color)
 * @param[in] font kiírandó szöveg betűtípusára mutató pointer (TTF_Font*)
 * @param[out] megjelenito erre rajzoljuk rá a szöveget (SDL_Renderer*)
 * @param hatterrel ezzel lehet engedélyezni, hogy legyen háttér a szöveghez (bool)
 */
void szoveget_kiir(const char* szoveg, int x, int y, SDL_Color betu_szin, SDL_Color hatter_szin, const TTF_Font* font, SDL_Renderer* megjelenito, bool hatterrel);


/**
 * @brief Kirajzolja a menü ablak fix (nem változó) elemeit
 *  
 * @param[out] ablak ennek a megjelenítőjére rajzoljuk ki menüt (Ablak*) @see Ablak
 * @param[in] bt betűtípus, szükséges a szöveg kiírásához (Betutipusok*) @see Betutipusok
 */
void fix_menut_kirajzol(Ablak* ablak, const Betutipusok* bt);


/**
 * @brief Súgó és dicsőséglista ablakok kezelése
 * 
 * A paraméterként megkapott billentyűállapot és a szintén paraméterként kapott ablakok
 * segítségével az ablakok megnyitását, bezárását, navgiálását, és kirajzolását kezeli.
 * A legutóbbihoz a betűtípusok paraméter is szükséges.
 * 
 * A súgó kirajzolás a "sugot_kirajzol", míg a dicsőséglista kirajzolása a
 * "dics_listat_kirajzol" függvénnyel történik.
 * 
 * @param[in] bill billentyűk lenyomásának állapotai (Billentyuk*) @see Billentyuk
 * @param[out] ablakok ablakokat tartalmazó dinamikus tömb (Ablak*) @see Ablak
 * @param[in] bt betűtípus, szükséges a szöveg kiírásához (Betutipusok*) @see Betutipusok
 */
void sugo_es_dics_lista_ablakok_kezelese(const Billentyuk* bill, Ablak* ablakok, const Betutipusok* bt);


/**
 * @brief Kirajzolja a súgót
 *
 * Ez a függvény kirajzolja a súgót, amiben hasznos információk lelhetők a felhasználó
 * számára.
 *  
 * @param[out] ablakok ablakokat tartalmazó dinamikus tömb (Ablak*) @see Ablak
 * @param[in] bt betűtípus, szükséges a szöveg kiírásához (Betutipusok*) @see Betutipusok
 */
void sugot_kirajzol(Ablak* ablakok, const Betutipusok* bt);


/**
 * @brief Kirajzolja a dicsőséglistát
 *
 * Ez a függvény kirajzolja a dicsőséglistát, amiben a 2, 3, illetve 4 játkosokhoz
 * tartalmazó legjobb három pontszámok találhatók.
 *  
 * @param[out] ablak egy Ablak struktúrára mutató pointer, ami tárolja a megjelenítőt, amire a kirajzolás történik (Ablak*) @see Ablak
 * @param[in] bt betűtípus, szükséges a szöveg kiírásához (Betutipusok*) @see Betutipusok
 */
void dics_listat_kirajzol(Ablak* ablak, const Betutipusok* bt);

#endif