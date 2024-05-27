#include <stdio.h>
#include <string.h>
#include <SDL2/SDL_image.h>
#include "headers/allheaders.h"
#include "headers/buttons.h"


button initialize_button(SDL_Renderer * renderer, char *name, point coordinates){
    button initializing_button;
    TTF_Init();
    initializing_button.font = TTF_OpenFont("fonts_and_images/font.ttf", 150);
    if (!initializing_button.font){
        printf("Cannot download font on button!");
        exit(0);
    }

    initializing_button.main_col.a = SDL_ALPHA_OPAQUE;
    initializing_button.main_col.r = 255;
    initializing_button.main_col.g = 255;
    initializing_button.main_col.b = 255;

    initializing_button.highlight_col.a = SDL_ALPHA_OPAQUE;
    initializing_button.highlight_col.r = 255;
    initializing_button.highlight_col.g = 0;
    initializing_button.highlight_col.b = 0;

    strcpy(initializing_button.name, name);

    SDL_Surface *stext = TTF_RenderText_Blended_Wrapped(initializing_button.font, initializing_button.name, initializing_button.main_col, 600);
    SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, stext);

    initializing_button.texture = text;
    initializing_button.drect.h = 100;
    initializing_button.drect.w = 300;
    initializing_button.drect.x = coordinates.x;
    initializing_button.drect.y = coordinates.y;
    
    return initializing_button;
}

button initialize_button_pxls(SDL_Renderer * renderer, char *name, point coordinates){
    button initializing_button;
    TTF_Init();
    initializing_button.font = TTF_OpenFont("fonts_and_images/font.ttf", 100);
    if (!initializing_button.font){
        printf("Cannot download font on button!");
        exit(0);
    }
    initializing_button.main_col.a = SDL_ALPHA_OPAQUE;
    initializing_button.main_col.r = 255;
    initializing_button.main_col.g = 255;
    initializing_button.main_col.b = 255;

    initializing_button.highlight_col.a = SDL_ALPHA_OPAQUE;
    initializing_button.highlight_col.r = 255;
    initializing_button.highlight_col.g = 0;
    initializing_button.highlight_col.b = 0;

    strcpy(initializing_button.name, name);

    SDL_Surface *stext = TTF_RenderText_Blended_Wrapped(initializing_button.font, initializing_button.name, initializing_button.main_col, 600);
    SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, stext);

    initializing_button.texture = text;
    initializing_button.drect.h = 30;
    initializing_button.drect.w = 50;
    initializing_button.drect.x = coordinates.x;
    initializing_button.drect.y = coordinates.y;
    
    return initializing_button;
}

button initialize_image_button(SDL_Renderer * renderer, char *source_image, point coordinates, int height, int width){
    button initializing_button;

    initializing_button.texture = IMG_LoadTexture(renderer, source_image);
    initializing_button.is_selected = false;
    initializing_button.drect.h = height;
    initializing_button.drect.w = width;
    initializing_button.drect.x = coordinates.x;
    initializing_button.drect.y = coordinates.y;
    
    return initializing_button;
}

void update_image_button(SDL_Renderer * renderer, button * current_button, SDL_Rect *mouse_rect){
    if(SDL_HasIntersection(&current_button->drect, mouse_rect) && current_button->is_selected == false){
        current_button->drect.x += 10;
        current_button->drect.y -= 10;
        current_button->is_selected = true;
    }
    else if (!SDL_HasIntersection(&current_button->drect, mouse_rect) && current_button->is_selected == true){
        current_button->drect.x -= 10;
        current_button->drect.y += 10;
        current_button->is_selected = false;
    }
}

void draw_image_button(SDL_Renderer * renderer, button * current_button){
    SDL_RenderCopy(renderer, current_button->texture, NULL, &current_button->drect);
}

button update_button(SDL_Renderer * renderer, button current_button, SDL_Rect mouse_point){
    if (SDL_HasIntersection(&current_button.drect, &mouse_point)){
        current_button.is_selected = true;
        SDL_Color col = {current_button.highlight_col.r, current_button.highlight_col.g, current_button.highlight_col.b, 0};
        SDL_Surface *stext = TTF_RenderText_Blended_Wrapped(current_button.font,current_button.name, col, 600);
        SDL_Texture *main_text = SDL_CreateTextureFromSurface(renderer, stext);
        current_button.texture = main_text;
        // SDL_DestroyTexture(main_text);
        SDL_FreeSurface(stext);
    }
    else{
        current_button.is_selected = false;
        SDL_Color col = {current_button.main_col.r, current_button.main_col.g, current_button.main_col.b, 0};
        SDL_Surface *stext = TTF_RenderText_Blended_Wrapped(current_button.font, current_button.name, col, 600);
        SDL_Texture *main_text = SDL_CreateTextureFromSurface(renderer, stext);
        current_button.texture = main_text;
        SDL_FreeSurface(stext);
    }
    return current_button;
}

void draw_button(button *current_button, SDL_Renderer * renderer){
    SDL_RenderCopy(renderer, current_button->texture, NULL, &current_button->drect);
    SDL_DestroyTexture(current_button->texture);
}