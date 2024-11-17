/** @file jatek_vezerlo.h
  * @brief A játék vezérléséért felelős, illetve az ahhoz szükséges adatokat tartalmazza
  *  
  * A játék vezérléséért felelős, illetve az ahhoz szükséges adatokat, adattípusokat,
  * függvényeket tartalmazza.
  * 
  * A játék bonyolultabb adattípusai a következő féle képpen épülnek fel:
  *   - A "jatekosok" egy pointer, ami egy dinamikusan foglalt tömbre mutat, ami a
  *     játékosokat tartalmazza. Elemszáma a kiválasztott játékosok száma.
  *   - Az egyes játékosokhoz tartozik egy-egy "vonal", amit maguk után húznak, ami egy
  *     láncolt lista.
  *   - A "vez" játékvezérlő struktúra rendelkezik egy "falak" nevű mutatóval, ami egy
  *     Falak típusú struktúra, ami tartalmazza a 4 falat, amik Fal struktúra típusú
  *     dinamikusan foglalt tömbök.
  *   - A "vez" játékvezérlő struktúra rendelkezik egy "lovedekek" nevű mutatóval, ami
  *     egy Lovedek típusú láncolt listára mutat.
  *   - Felvehető elemek
  *   - Animációk
  *
  * @author Bácsi Miklós
  * @date 2024-11-10
*/

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

/**
 * @brief A játékmód típusát jelöli
 * 
 * Ez az adattípus ahhoz szükséges, hogy a különböző játékmódok közötti funkciókat
 * megfelelően tudjuk implementálni.
 */
typedef enum Jatekmod {
    NORMAL,         ///< 'Normál játékmódot' jelöli
    FAL_NELKULI,    ///< 'Fal nélküli' játékmód
    FELV_E_T        ///< 'Felvehető elemet tiltása' játékmódot
} Jatekmod;


/**
 * @brief Ez az adattípus reprezentál egy pontos koordinátát a síkon
 */
typedef struct Koordinata {
    double x;   ///< koordináta abszcisszája (double)
    double y;   ///< koordináta ordinátája (double)
} Koordinata;


/**
 * @brief Ez az adattípus egy képpontot reprezentál a megjelenítőn
 */
typedef struct Pixel {
    short x;    ///< koordináta abszcisszája (short)
    short y;    ///< koordináta ordinátája (short)
} Pixel;


/**
 * @brief Ez az adattípus egy pixel falat reprezentál a pályán
 * 
 * Fallal való ütközéskor az adott játákos meghal, míg a lövedékek kitörlik a falakat
 * (a játékon belül).
 */
typedef struct Fal {
    Pixel pixel;    ///< koordináta abszcisszája (Pixel) @see Pixel
    bool torolve;   ///< bool változó, ami jelzi, hogy törölve van-e a fal (bool)
} Fal;


/**
 * @brief Ez az adattípus a 4 falra mutató pointert, illetve x és y hosszt tartalmazza
 */
typedef struct Falak {
    Fal* felso; ///< Felső falra (dinamikus tömb) mutató pointer (Fal*) @see Fal
    Fal* also;  ///< Alsó falra (dinamikus tömb) mutató pointer (Fal*) @see Fal
    Fal* bal;   ///< Bal falra (dinamikus tömb) mutató pointer (Fal*) @see Fal
    Fal* jobb;  ///< Jobb falra (dinamikus tömb) mutató pointer (Fal*) @see Fal
    short x_db; ///< felső és alsó falak darabszáma (short)
    short y_db; ///< bal és jobb falak darabszáma (short)
} Falak;


/**
 * @brief Ez az adattípus egy lövedéket reprezentál a pályán
 * 
 * Lövedékkel való ütközéskor az adott játákos meghal, míg a vonalakat és a falakat
 * törlésere kerülnek (a játékon belül).
 */
typedef struct Lovedek {
    Koordinata kp;      ///< lövedék középpontja (Koordinata) @see Koordinata
    double irany;       ///< lövedék haladási iránya fokban megadva (double)
    double sugar;       ///< lövedék sugara (double)
    Szin szin;          ///< lövedék színe (Szin) @see Szin
    struct Lovedek* kov;///< lövedék középpontja (Lovedek*) @see Lovedek
} Lovedek;


/**
 * @brief Ez az adattípus egy (játékos által húzott) vonalat reprezentál a pályán
 * 
 * Vonallal való ütközéskor az adott játákos meghal, míg a vonalakat kitörlik
 * a lövedékek (a játékon belül).
 */
typedef struct Vonal {
    Koordinata kord;    ///< vonal középpontja (Koordinata) @see Koordinata
    Szin szin;          ///< vonal színe (Szin) @see Szin
    int16_t id;         ///< vonalszegmensek azonosítója (pozitív: vonal közepe, negatív: vonal végpontja) (int16_t)
    struct Vonal* kov;  ///< vonal középpontja (Vonal*) @see Vonal
} Vonal;


/**
 * @brief Ez az adattípus egy lyukat (egy végpontpárt) reprezentál, amin való áthaladásért jutalmat kapnak a játékosok
 */
typedef struct Lyuk {
    Koordinata eleje;   ///< a lyuk elejének koordinátája (Koordinata) @see Koordinata
    Koordinata vege;    ///< a lyuk végének koordinátája (Koordinata) @see Koordinata
    struct Lyuk* kov;    ///< láncolt lista következő eleme (Lyuk*) @see Lyuk
} Lyuk;


/**
 * @brief Menün belül kiválasztott játékmódot, illetve játékosokat tárolja
 * 
 * Ennek segítségével tudjuk új játék indításánál kiválasztani a játékmódot,
 * illetve ezzel tudunk megfelelő mennyiségű játékost foglalni.
 */
typedef struct Kivalasztas {
    int kiv_jt_mod;         ///< kiválasztott játékmódot jelöli (int)
    bool aktiv_jatekosok[4];///< 4 elemű bool tömb (true: játékos kiválasztva, false: nem)  (bool*)
} Kivalasztas;


/**
 * @brief Speciális elemek típusát jelöli
 */
typedef enum SpecialisElemTipus {
    NINCS = 0,  ///< Játékos nem rendelkezik speciális elemmel
    NAGY_LOV,   ///< Játékos nagy lövéssel rendelkezik
    SOK_LOV,    ///< Játékos sok lövéssel rendelkezik
    PAJZS       ///< Játékos pajzzsal rendelkezik
} SpecialisElemTipus;


/**
 * @brief Egy speciális elemet jelöl, amivel egy játékos rendelkezik
 */
typedef struct SpecialisElem {
    SpecialisElemTipus tipus;   ///< Játékos speciális elemének típusa (SpecialisElemTipus) @see SpecialisElemTipus
    double elet_tartam;         ///< Játékos pajzsának élettartama (double)
} SpecialisElem;


/**
 * @brief Felvehető elemet (buborékot) jelöl, ami a kijelzőn is meg fog jelenni
 */
typedef struct FelvehetoElem {
    Pixel kp;                   ///< Felvehető elem (buborék) középpontja (Pixel) @see Pixel
    double elet_tartam;         ///< Felvehető elem (buborék) élettartama mielőtt eltűnik (double)
    struct FelvehetoElem* kov;  ///< Felvehető elemek láncolt listájának következő eleme (FelvehetoElem*) @see FelvehetoElem
} FelvehetoElem;


/**
 * @brief Animációk típusát (+1, halálfej) jelöli
 */
typedef enum AniTipus {
    HALALFEJ = 0,   ///< Halálfej animációt jelöli
    PIROSPLUSZ1,    ///< Piros '+1' animációt jelöli
    ZOLDPLUSZ1,     ///< Zöld '+1' animációt jelöli
    KEKPLUSZ1,      ///< Kék '+1' animációt jelöli
    ROZSAPLUSZ1     ///< Rózsaszín '+1' animációt jelöli
} AniTipus;


/**
 * @brief Animációkat (+1, halálfej) tartalmazó láncolt lista
 */
typedef struct Animacio {
    SDL_Texture* kep;       ///< betöltött textúrára mutató pointer (SDL_Texture*)
    Pixel poz;              ///< animáció középpontjának koordinátái (Pixel) @see Pixel
    double elet_tartam;     ///< animáció élettartama másodpercben megadva (double)
    struct Animacio* kov;   ///< láncolt lista következő eleme (Animacio*) @see Animacio
} Animacio;


/**
 * @brief Animációk (+1, halálfej) textúráit tartalmazó struktúra
 */
typedef struct AnimacioTexturak {
    SDL_Texture* halalfej;      ///< Halálfej textúrára mutató pointer (SDL_Texture*)
    SDL_Texture* buborek;       ///< Felvehető elem (buborék) (SDL_Texture*)
    // '+1'
    SDL_Texture* pirosPluszEgy; ///< Piros +1 textúrára mutató pointer (SDL_Texture*)
    SDL_Texture* zoldPluszEgy;  ///< Zöld +1 textúrára mutató pointer (SDL_Texture*)
    SDL_Texture* kekPluszEgy;   ///< Kék +1 textúrára mutató pointer (SDL_Texture*)
    SDL_Texture* rozsaPluszEgy; ///< Rózsaszín +1 textúrára mutató pointer (SDL_Texture*)
    // Normal loves piktogram
    SDL_Texture* pirosNormalLov;///< Piros normál lövésre textúrára mutató pointer (SDL_Texture*)
    SDL_Texture* zoldNormalLov; ///< Zöld normál lövésre textúrára mutató pointer (SDL_Texture*)
    SDL_Texture* kekNormalLov;  ///< Kék normál lövésre textúrára mutató pointer (SDL_Texture*)
    SDL_Texture* rozsaNormalLov;///< Rózsaszín normál lövésre textúrára mutató pointer (SDL_Texture*)
    // Nagy loves piktogram
    SDL_Texture* pirosNagyLov;  ///< Piros nagy lövésre textúrára mutató pointer (SDL_Texture*)
    SDL_Texture* zoldNagyLov;   ///< Zöld nagy lövésre textúrára mutató pointer (SDL_Texture*)
    SDL_Texture* kekNagyLov;    ///< Kék nagy lövésre textúrára mutató pointer (SDL_Texture*)
    SDL_Texture* rozsaNagyLov;  ///< Rózsaszín nagy lövésre textúrára mutató pointer (SDL_Texture*)
    // Sok loves piktogram
    SDL_Texture* pirosSokLov;   ///< Piros sok lövésre textúrára mutató pointer (SDL_Texture*)
    SDL_Texture* zoldSokLov;    ///< Zöld sok lövésre textúrára mutató pointer (SDL_Texture*)
    SDL_Texture* kekSokLov;     ///< Kék sok lövésre textúrára mutató pointer (SDL_Texture*)
    SDL_Texture* rozsaSokLov;   ///< Rózsaszín sok lövésre textúrára mutató pointer (SDL_Texture*)
} AnimacioTexturak;


/**
 * @brief Ez a struktúra egy játékost reprezentál
 * 
 * Ezt a struktúra tartalmazza egy játékos adatait, illetve egy pointert, ami a
 * hozzá tartozó vonalra (láncolt lista) mutat.
 */
typedef struct Jatekos {
    Szin szin;          ///< játékos színe (Szin) @see Szin
    Koordinata fej;     ///< játékos feje (Koordinata) @see Koordinata
    Koordinata elozo;   ///< játékos előző pozíciója, új vonal koordinátája (Koordinata) @see Koordinata
    double irany;       ///< játékos haladási iárnya fokban (double)
    SpecialisElem spec; ///< speciális elem (pajzs, nagy/sok lövés) (SpecialisElem) @see SpecialisElem
    bool* bal;          ///< mutató a játékos bal gombjára (bool*) @see Billentyuk
    bool* lo;           ///< mutató a játékos lövés gombjára (bool*) @see Billentyuk
    bool* tilt_lo;      ///< mutató a játékos lövésének tiltására (bool*) @see Billentyuk
    bool* jobb;         ///< mutató a játékos jobb gombjára (bool*) @see Billentyuk
    Vonal* vonal;       ///< mutató a játékoshoz tartozó vonalra (Vonal*) @see Vonal
    bool eletben_van;   ///< jelzi életben van-e a játékos (bool)
    int pontszam;       ///< játékos pontszáma (int)
    int16_t lyuk_tilt;  ///< lyukon való áthaladás tiltása egy rövid ideig (hogy pontosan 1 pontot kapjon)
} Jatekos;


/**
 * @brief Ez a struktúra egy a játék vezérléséhez szükséges adatokat, adatszerkezeteket tartalmazza
 */
typedef struct Vezerles {
    bool megallitva_jatek;      ///< jelzi a játék meg van-e állítva a játékvezérlés által (bool)
    bool megallitva_felhasznalo;///< jelzi a játék meg van-e állítva a felhasználó által (bool)
    Uint16 menetido;            ///< számolja a menetidőt (több mint 20 perc után csordulna túl, de lehetetlen addig játszani egy menetet) (Uint16)
    int16_t vonal_szamlalo;     ///< játékosonkénti lerakott vonal (pontok) száma (Uint16)
    bool menet_vege;            ///< jelzi a menet végetért-e (bool)
    bool jatek_vege;            ///< jelzi a játék végét (bool)
    int jatekosszam;            ///< tartalmazza az aktív játékosok számát (int)
    int max_elerheto_pontszam;  ///< tartalmazza az adott játékosszámhoz tartozó elégséges pont, mely elérése esetén a játék véget ér (int)
    int max_pontszam;           ///< játékosok által szerzett maximális pontszám (int)
    Jatekmod jt_mod;            ///< tartalmazza a játékmódot (Jatekmod) @see Jatekmod
    double elmozd_jat;          ///< tartalmazza azt az hosszt, amellyel a játékosok képfrissítésenként elmozgulnak (double)
    double elmozd_lov;          ///< tartalmazza azt az hosszt, amellyel a lövedékek képfrissítésenként elmozgulnak (double)
    double fordulas;            ///< tartalmazza azt az fokot, amellyel a játékosok képfrissítésenként elfordulnak (double)
    Falak falak;                ///< struktúra, mely tartalmaz pointereket, melyek a 4 falra (dinamikus tömb) mutatnak @see Falak
    Pixel palya_meret;          ///< pixel típusú adattípus, mely tartalmazza a pálya szélességét és hosszát (Pixel) @see Pixel
    Lovedek* lovedekek;         ///< pointer, mely a lövedékekre (láncolt lista) mutat (Lovedek*) @see Lovedek
    Lyuk* lyukak;               ///< lyukakat tartalmazó láncolt lista, amiken való áthaladásért a játékosok jutalomban részesülnak (Lyuk*)
    FelvehetoElem* buborekok;   ///< felvehető elemeket (buborékokat) tartalmazó láncolt lista (FelvehetoElem*) @see FelvehetoElem
    AnimacioTexturak ani;       ///> animációk textúráit tartalmazó struktúra (AnimacioTexturak) @see AnimacioTexturak
    Animacio* animaciok;        ///< animációkat (+1, halálfej) tartalmazó láncolt lista (Animacio*) @see Animacio
} Vezerles;


/**
 * @brief Játék ablak kezelése
 * 
 * A kapott paraméterek segítsével kezeli a játékablak megnyitását, bezárását, navigálását
 * új menet indítását, játék megállítását/folytatását stb.
 * 
 * Továbbá létrehozza a játékosokat (dinamikus tömb) a kiv_jat kapott paraméter segítségével.
 * A játékosokhoz hozzárendeli a bal, jobb, lő, lövés tiltás gombokat.
 * 
 * Megfelelő játékmód esetén létrehozza a falakat (dinamikus tömb) (ha még nem voltak létrehozva).
 * 
 * Ezen kívül új menetet indít az "uj_menet" függvény meghívásával.
 * 
 * @param bill billentyűk lenyomásának állapotai (Billentyuk*) @see Billentyuk
 * @param[out] jatek_ablak játékablak megjelenítéshez (Ablak*) @see Ablak
 * @param[out] vez játékvezérléshez szükséges adatokat tartalmazó struktúra (Vezerles*) @see Vezerles
 * @param[out] cim_jatekosok játékosok címét tartalmazza (Jatekos**) @see Jatekos
 * @param jt_mod játékmódot tartalmazza (int)
 * @param kiv_jat 4 elemű bool-okat tartalmazó lista, játkosok foglalásához szükséges (bool*)
 */
void jatek_ablak_kezelese(Billentyuk* bill, Ablak* jatek_ablak, Vezerles* vez, Jatekos** cim_jatekosok, int jt_mod, bool* kiv_jat);


/**
 * @brief Új menet indítása
 * 
 * Értékeket inicializál, felszabadítja a lefoglalt játékelemeket (pl. lövedékek, vonalak),
 * játékosok véletlenszerű pozícióját jelöli ki stb.
 * 
 * @param[out] vez játékvezérléshez szükséges adatokat tartalmazó struktúra (Vezerles*) @see Vezerles
 * @param[out] jatekosok játékosokat tartalmazó dinamikus tömböt tartalmazza (Jatekos*) @see Jatekos
 */
void uj_menet(Vezerles* vez, Jatekos* jatekosok);


/**
 * @brief A megkapott paraméterek segítségével kirajzolja a teljes játékot a képernyőre
 * 
 * @param[out] jatek_ablak játékablak megjelenítéshez (Ablak*) @see Ablak
 * @param vez játékvezérléshez szükséges adatokat tartalmazó struktúra (Vezerles*) @see Vezerles
 * @param jatekosok játékosokat tartalmazó dinamikus tömböt tartalmazza (Jatekos*) @see Jatekos
 * @param bt betűtípus, szükséges a szöveg kiírásához (Betutipusok*) @see Betutipusok
 */
void jatek_kirajzolasa(Ablak* jatek_ablak, Vezerles* vez, Jatekos* jatekosok, Betutipusok* bt);


/**
 * @brief Az SDL időzítőjének hozzáadásához szükséges
 * 
 * Ez a függvény paraméterként fogad egy Uint32 típusú ms változót, ami ms-ban jelöli
 * az az időközt, amilyen gyakorisággal az SDL könyvtár időzítője jelet generál. Ez a
 * parancs a függvény hasában található: SDL_PushEvent.
 * 
 * A függvény argumentumként használjuk egy másik függvényben (SDL_AddTimer), ami egy
 * időzítő létrehozásához szükséges (SDL_TimerID).
 * 
 * @param ms időköz ms-ban amikor az SDL időzítője jelet generál (Uint32)
 * @param param általában NULL értéket adunk neki (void*)
 * @return időköz ms-ban amikor az SDL időzítője jelet generál (Uint32)
 */
Uint32 idozit(Uint32 ms, void *param);


/**
 * @brief A lenyomott gombok alapján elforgatja, majd eltolja az életben lévő játékosokat
 * 
 * Az egyes játékosokhoz hozzá vannak rendelve a gombok, amikkel irányítani tudnak (bool*).
 * Ezeket és a paraméterként fogadott "vez" Vezerlo típusú struktúrában definiált képfrissítéenkénti
 * elfordulási és elmozdulási értékeket használja fel a transzformációk végrehajtásához.
 * 
 * @param[out] jatekosok játékosokat tartalmazó dinamikus tömböt tartalmazza (Jatekos*) @see Jatekos
 * @param vez játékvezérléshez szükséges adatokat tartalmazó struktúra (Vezerles*) @see Vezerles
 */
void jatekosok_mozditasa(Jatekos* jatekosok, Vezerles* vez);


/**
 * @brief A lövedékek láncolt listán iterál és az iránya alapján eltolja a megfelelő irányba
 * 
 * @param[out] lovedekek lövedékek láncolt lista (Lovedek*) @see Lovedek
 * @param elmozd ekkora lesz a lövedék elmozdulása (double)
 */
void lovedekek_mozditasa(Lovedek* lovedekek, double elmozd);


/**
 * @brief A játékosok halálát vizsgálja
 * 
 * Paraméterként fogadott játékosok dinamikus tömbön iterál és különböző esetekre vizsgál,
 * amikor a játékosok meghalhatnak.
 * Halál esetén egy halálfej animációt játszik le.
 * Továbbá, vizsgálja a játék végét (maxpontszám elérése).
 * 
 * @param[out] jatekosok játékosokat tartalmazó dinamikus tömböt tartalmazza (Jatekos*) @see Jatekos
 * @param[out] vez játékvezérléshez szükséges adatokat tartalmazó struktúra (Vezerles*) @see Vezerles
 */
void halal_vizsgalata(Jatekos* jatekosok, Vezerles* vez);


/**
 * @brief A lövés lehetőségét vizsálja
 * 
 * Lövés esetén meghívja a "loves_hozzaadasa" függvényt.
 * Ha a játékosok lőnek, akkor csökkenti a pontjukat is.
 * 
 * @param[out] jatekosok játékosokat tartalmazó dinamikus tömböt tartalmazza (Jatekos*) @see Jatekos
 * @param[out] vez játékvezérléshez szükséges adatokat tartalmazó struktúra (Vezerles*) @see Vezerles
 */
void loves_vizsgalata(Jatekos* jatekosok, Vezerles* vez);


/**
 * @brief Egyesével iterál a lövedékeken (láncolt lista) majd felszabadítja azokat
 * 
 * A paraméterként fogadott vez Vezerles struktúrára mutató pointer tartalmazza a lövedékeket.
 * 
 * @param[out] vez játékvezérléshez szükséges adatokat tartalmazó struktúra (Vezerles*) @see Vezerles
 */
void lovedekeket_torol(Vezerles* vez);


/**
 * @brief Egyesével iterál a lyukakon (láncolt lista) majd felszabadítja azokat
 * 
 * A paraméterként fogadott vez Vezerles struktúrára mutató pointer tartalmazza a lyukakat.
 * 
 * @param[out] vez játékvezérléshez szükséges adatokat tartalmazó struktúra (Vezerles*) @see Vezerles
 */
void lyukakat_torol(Vezerles* vez);


/**
 * @brief Különböző esetekben vizsgálja és törli a falakat és lövedékeket
 * 
 * A paraméterként fogadott vez Vezerles struktúrára mutató pointer tartalmazza a
 * lövedékeket (láncolt lista) és a falakat (dinamikus tömb).
 * 
 * @param[out] vez játékvezérléshez szükséges adatokat tartalmazó struktúra (Vezerles*) @see Vezerles
 */
void falak_es_lovekek_torlesenek_vizsgalata(Vezerles* vez);


/**
 * @brief Adott gyakorisággal az életben lévő játékosok után lerak vonalat
 * 
 * A paraméterként fogadott jatekosok Jatekos típusú dinamikus tömb egyes elemei
 * rendelkeznek egy Vonal láncolt listára mutató pointerrel, amit változtatunk.
 * 
 * Mindig a lista elejére szúrunk be, és az eleje a játékos fejével együtt mozog,
 * míg a lista vége a kezdőpozíció marad (kivéve ha kitöröljük, mert akkor változik).
 * 
 * @param[out] jatekosok játékosokat tartalmazó dinamikus tömböt tartalmazza (Jatekos*) @see Jatekos
 * @param vez játékvezérléshez szükséges adatokat tartalmazó struktúra (Vezerles*) @see Vezerles
 */
void vonalat_hozzaad(Jatekos* jatekosok, Vezerles* vez);


/**
 * @brief Kitörli a játékosok által húzott összes vonalat
 * 
 * Új menet hívásakor, és a játékból való kilépéskor lesz meghívva.
 * 
 * @param[out] jatekosok játékosokat tartalmazó dinamikus tömböt tartalmazza (Jatekos*) @see Jatekos
 * @param vez játékvezérléshez szükséges adatokat tartalmazó struktúra (Vezerles*) @see Vezerles
 */
void vonalakat_torol(Jatekos* jatekosok, Vezerles* vez);


/**
 * @brief Vizsgálja a játékosok áthaladását a lyukon illetve, hogy azokat törli-e lövedék
 * 
 * @param[out] jatekosok játékosokat tartalmazó dinamikus tömböt tartalmazza (Jatekos*) @see Jatekos
 * @param[out] vez játékvezérléshez szükséges adatokat tartalmazó struktúra (Vezerles*) @see Vezerles
 */
void lyuk_vizsgalata(Jatekos* jatekosok, Vezerles* vez);


/**
 * @brief Betölt egy textúrát egy mejelenítőre az SDL könyvtár segítségével
 * 
 * @param[in] nev betöltendő kép neve (char*)
 * @param[out] jatekablak ennek az ablaknak a megjelenítőjére tölti majd be a textrúrát (Ablak*) @see Ablak
 * @return betöltött textúra címe (SDL_Texture*)
 */
SDL_Texture* textura_betoltese(char* nev, Ablak* jatekablak);


/**
 * @brief Bezárja az animáció textrúráit
 * 
 * @param[out] vez struktúra, mely tartalmazza a beolvasott textúrák címeit (Vezerles*) @see Vezerles
 */
void animacio_texturak_bezarasa(Vezerles* vez);


/**
 * @brief Bezárja az animáció textrúráit
 * 
 * A paraméterként fogadott vez struktúra tartalmazza a betöltött textúrákat, illetve az animációk láncolt listát is.
 * 
 * @param[out] vez struktúra, mely tartalmazza az 'animaciok' láncolt listát, illetve a beolvasott textúrák címeit (Vezerles*) @see Vezerles
 */
void animaciok_kezelese(Vezerles* vez);


/**
 * @brief Egyesével iterál az animációkon (láncolt lista) majd felszabadítja azokat
 * 
 * A paraméterként fogadott vez Vezerles struktúrára mutató pointer tartalmazza az animációkat
 * 
 * @param[out] vez játékvezérléshez szükséges adatokat tartalmazó struktúra (Vezerles*) @see Vezerles
 */
void animaciokat_torol(Vezerles* vez);


/**
 * @brief Felvehető elemeket (buborékokat), illetve a speciális elemeket (a már felvett elemeket) kezeli
 * 
 * A paraméterként fogadott vez struktúra tartalmazza a buborékok (felvehető elemek) láncolt listát.
 * 
 * @param[out] vez játékvezérléshez szükséges adatokat tartalmazó struktúra (Vezerles*) @see Vezerles
 * @param[out] jatekosok játékosokat tartalmazó dinamikus tömböt tartalmazza (Jatekos*) @see Jatekos
 */
void felveheto_elemek_kezelese(Vezerles* vez, Jatekos* jatekosok);


/**
 * @brief Kitörli a felvehető elemeket (buborékokat)
 * 
 * @param[out] vez játékvezérléshez szükséges adatokat tartalmazó struktúra (Vezerles*) @see Vezerles
 */
void felveheto_elemeket_torol(Vezerles* vez);

#endif