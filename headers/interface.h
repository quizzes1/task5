#pragma once
typedef struct work_zone {
    SDL_Rect drect;
    SDL_Texture * texture;
} work_zone;

typedef enum tool_type {NOTHING, PALETTE, CIRCLE, BRUSH, ERASER, LINE, RECTANGLE, FILLING} tool_type;
void interface(SDL_Renderer * renderer, SDL_Window * window);