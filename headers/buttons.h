#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "allheaders.h"

typedef struct button{
    TTF_Font* font;
    SDL_Texture* texture;
    SDL_Rect drect;
    SDL_Color main_col;
    SDL_Color highlight_col;
    bool is_selected;
    char name[100];
} button;

button initialize_button(SDL_Renderer * renderer, char *name, point coordinates);
button initialize_image_button(SDL_Renderer * renderer, char *source_image, point coordinates, int height, int width);
button initialize_button_pxls(SDL_Renderer * renderer, char *name, point coordinates);
void draw_image_button(SDL_Renderer * renderer, button * current_button);
void draw_button(button *current_button, SDL_Renderer * renderer);
button update_button(SDL_Renderer * renderer, button current_button, SDL_Rect mouse_point);
void update_image_button(SDL_Renderer * renderer, button * current_button, SDL_Rect *mouse_rect);