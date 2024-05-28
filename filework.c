#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "headers/allheaders.h"
#include "headers/filework.h"

void save_image(SDL_Renderer *renderer, const char *filename) {
    SDL_Surface *save_surface = NULL;
    SDL_Surface *infoSurface = SDL_GetWindowSurface(SDL_GetWindowFromID(1));
    SDL_Rect save_rect;
    save_rect.h = 620;
    infoSurface->h = 620;
    save_rect.w = SCREEN_WIDTH;
    save_rect.x = 0;
    save_rect.y = 100;
    if (infoSurface == NULL) {
        printf("Couldn't create surface from window: %s\n", SDL_GetError());
        return;
    }
    unsigned char *pixels = (unsigned char *)malloc(infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel);
    if (pixels == 0) {
        printf("Couldn't allocate memory for pixels: %s\n", SDL_GetError());
        return;
    }

    if (SDL_RenderReadPixels(renderer, &save_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0) {
        printf("Couldn't read pixels from renderer: %s\n", SDL_GetError());
        free(pixels);
        return;
    }

    save_surface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
    if (save_surface == NULL) {
        printf("Couldn't create surface: %s\n", SDL_GetError());
        free(pixels);
        return;
    }

    if (SDL_SaveBMP(save_surface, filename) != 0) {
        printf("Couldn't save BMP: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(save_surface);
    free(pixels);
}

SDL_Texture * open_image(SDL_Renderer * renderer){
    SDL_Texture * canvas_texture = IMG_LoadTexture(renderer, "saved.bmp");
    return canvas_texture;
}

// void open_image(SDL_Renderer *renderer, Canvas *canvas, const char *filename) {
//     SDL_Surface *loadedSurface = SDL_LoadBMP(filename);
//     if (loadedSurface == NULL) {
//         printf("Unable to load image %s! SDL Error: %s\n", filename, SDL_GetError());
//         return;
//     }
//     SDL_BlitSurface(loadedSurface, NULL, canvas->surface, NULL);
//     update_canvas_texture(renderer, canvas);
//     SDL_FreeSurface(loadedSurface);
// }
