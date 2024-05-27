#pragma once
typedef struct work_zone {
    SDL_Rect drect;
    SDL_Texture * texture;
} work_zone;

typedef enum tool_type {NOTHING, PALETTE, CIRCLE, BRUSH, ERASER, LINE, RECTANGLE, FILLING} tool_type;
typedef enum colour_type {BLACK, BLUE, GREEN, ORANGE, PINK, PURPLE, RED, WHITE, YELLOW} colour_type;
void interface(SDL_Renderer * renderer, SDL_Window * window);