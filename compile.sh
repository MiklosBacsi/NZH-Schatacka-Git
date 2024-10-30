gcc -c main.c -o Build/main.o `sdl2-config --cflags --libs` -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lSDL2_mixer -Wall -Werror
gcc -c ablak_kezelo.c -o Build/ablak.o `sdl2-config --cflags --libs` -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lSDL2_mixer -Wall -Werror
gcc -c jatek_vezerlo.c -o Build/jatek.o `sdl2-config --cflags --libs` -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lSDL2_mixer -Wall -Werror
gcc Build/main.o Build/ablak.o Build/jatek.o -o Schatacka `sdl2-config --cflags --libs` -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lSDL2_mixer -Wall -Werror

./Schatacka
