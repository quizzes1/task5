all:
	gcc -o paint.exe menu.c buttons.c interface.c filework.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx
	.\paint