#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TOOLBAR_HEIGHT 60

typedef struct {
    int x, y, w, h;
    SDL_Color color;
} Button;

typedef enum {
    TOOL_NONE,
    TOOL_LINE,
    TOOL_RECTANGLE,
    TOOL_CIRCLE,
    TOOL_FILL,
    TOOL_UNDO
} Tool;

typedef struct {
    SDL_Surface *surface;
    SDL_Texture *texture;
} Canvas;

void save_image(SDL_Renderer *renderer, const char *filename) {
    SDL_Surface *saveSurface = NULL;
    SDL_Surface *infoSurface = SDL_GetWindowSurface(SDL_GetWindowFromID(1));
    if (infoSurface == NULL) {
        printf("Couldn't create surface from window: %s\n", SDL_GetError());
        return;
    }
    unsigned char *pixels = (unsigned char *)malloc(infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel);
    if (pixels == 0) {
        printf("Couldn't allocate memory for pixels: %s\n", SDL_GetError());
        return;
    }

    if (SDL_RenderReadPixels(renderer, NULL, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0) {
        printf("Couldn't read pixels from renderer: %s\n", SDL_GetError());
        free(pixels);
        return;
    }

    saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
    if (saveSurface == NULL) {
        printf("Couldn't create surface: %s\n", SDL_GetError());
        free(pixels);
        return;
    }

    if (SDL_SaveBMP(saveSurface, filename) != 0) {
        printf("Couldn't save BMP: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(saveSurface);
    free(pixels);
}

bool is_point_in_rect(int x, int y, SDL_Rect *rect) {
    return x >= rect->x && x <= (rect->x + rect->w) && y >= rect->y && y <= (rect->y + rect->h);
}

void draw_button(SDL_Renderer *renderer, Button *button) {
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, button->color.a);
    SDL_Rect rect = {button->x, button->y, button->w, button->h};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

void draw_circle(SDL_Renderer *renderer, int x0, int y0, int radius) {
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y) {
        SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
        SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

Canvas* create_canvas(SDL_Renderer *renderer, int width, int height) {
    Canvas *canvas = (Canvas *)malloc(sizeof(Canvas));
    canvas->surface = SDL_CreateRGBSurface(0, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    canvas->texture = SDL_CreateTextureFromSurface(renderer, canvas->surface);
    return canvas;
}

void update_canvas_texture(SDL_Renderer *renderer, Canvas *canvas) {
    SDL_DestroyTexture(canvas->texture);
    canvas->texture = SDL_CreateTextureFromSurface(renderer, canvas->surface);
}

void draw_canvas(SDL_Renderer *renderer, Canvas *canvas) {
    SDL_RenderCopy(renderer, canvas->texture, NULL, NULL);
}

void clear_canvas(Canvas *canvas, SDL_Color color) {
    SDL_FillRect(canvas->surface, NULL, SDL_MapRGBA(canvas->surface->format, color.r, color.g, color.b, color.a));
}

void render_preview(SDL_Renderer *renderer, Tool currentTool, int start_x, int start_y, int end_x, int end_y, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    switch (currentTool) {
        case TOOL_LINE:
            SDL_RenderDrawLine(renderer, start_x, start_y, end_x, end_y);
            break;
        case TOOL_RECTANGLE:
            SDL_RenderDrawRect(renderer, &(SDL_Rect){start_x, start_y, end_x - start_x, end_y - start_y});
            break;
        case TOOL_CIRCLE: {
            int radius = (int)sqrt((end_x - start_x) * (end_x - start_x) + (end_y - start_y) * (end_y - start_y));
            draw_circle(renderer, start_x, start_y, radius);
            break;
        }
        default:
            break;
    }
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("SDL Paint", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Canvas *canvas = create_canvas(ren, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_Color whiteColor = {255, 255, 255, 255};
    clear_canvas(canvas, whiteColor);
    update_canvas_texture(ren, canvas);

    Button redButton = {10, 10, 50, 40, {255, 0, 0, 255}};
    Button greenButton = {70, 10, 50, 40, {0, 255, 0, 255}};
    Button blueButton = {130, 10, 50, 40, {0, 0, 255, 255}};
    Button lineButton = {200, 10, 50, 40, {100, 100, 100, 255}};
    Button rectButton = {260, 10, 50, 40, {150, 150, 150, 255}};
    Button circleButton = {320, 10, 50, 40, {200, 200, 200, 255}};
    Button fillButton = {380, 10, 50, 40, {50, 50, 50, 255}};
    Button undoButton = {440, 10, 50, 40, {0, 0, 0, 255}};

    SDL_Color currentColor = {0, 0, 0, 255};
    Tool currentTool = TOOL_NONE;

    bool quit = false;
    bool drawing = false;
    bool preview = false;
    int start_x = 0, start_y = 0, last_x = 0, last_y = 0;

    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (is_point_in_rect(e.button.x, e.button.y, &(SDL_Rect){0, 0, WINDOW_WIDTH, TOOLBAR_HEIGHT})) {
                    if (is_point_in_rect(e.button.x, e.button.y, &(SDL_Rect){redButton.x, redButton.y, redButton.w, redButton.h})) {
                        currentColor = redButton.color;
                    } else if (is_point_in_rect(e.button.x, e.button.y, &(SDL_Rect){greenButton.x, greenButton.y, greenButton.w, greenButton.h})) {
                        currentColor = greenButton.color;
                    } else if (is_point_in_rect(e.button.x, e.button.y, &(SDL_Rect){blueButton.x, blueButton.y, blueButton.w, blueButton.h})) {
                        currentColor = blueButton.color;
                    } else if (is_point_in_rect(e.button.x, e.button.y, &(SDL_Rect){lineButton.x, lineButton.y, lineButton.w, lineButton.h})) {
                        currentTool = TOOL_LINE;
                    } else if (is_point_in_rect(e.button.x, e.button.y, &(SDL_Rect){rectButton.x, rectButton.y, rectButton.w, rectButton.h})) {
                        currentTool = TOOL_RECTANGLE;
                    } else if (is_point_in_rect(e.button.x, e.button.y, &(SDL_Rect){circleButton.x, circleButton.y, circleButton.w, circleButton.h})) {
                        currentTool = TOOL_CIRCLE;
                    }
                } else {
                    drawing = true;
                    preview = true;
                    start_x = last_x = e.button.x;
                    start_y = last_y = e.button.y;
                }
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                if (drawing) {
                    drawing = false;
                    preview = false;
                    switch (currentTool) {
                        case TOOL_LINE:
                            SDL_SetRenderDrawColor(ren, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
                            SDL_RenderDrawLine(ren, start_x, start_y, e.button.x, e.button.y);
                            break;
                        case TOOL_RECTANGLE:
                            SDL_SetRenderDrawColor(ren, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
                            SDL_RenderDrawRect(ren, &(SDL_Rect){start_x, start_y, e.button.x - start_x, e.button.y - start_y});
                            break;
                        case TOOL_CIRCLE: {
                            int radius = (int)sqrt((e.button.x - start_x) * (e.button.x - start_x) + (e.button.y - start_y) * (e.button.y - start_y));
                            SDL_SetRenderDrawColor(ren, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
                            draw_circle(ren, start_x, start_y, radius);
                            break;
                        }
                        default:
                            break;
                    }
                    update_canvas_texture(ren, canvas);
                }
            } else if (e.type == SDL_MOUSEMOTION) {
                if (drawing) {
                    last_x = e.motion.x;
                    last_y = e.motion.y;
                }
            }
        }

        SDL_RenderClear(ren);
        draw_canvas(ren, canvas);
        draw_button(ren, &redButton);
        draw_button(ren, &greenButton);
        draw_button(ren, &blueButton);
        draw_button(ren, &lineButton);
        draw_button(ren, &rectButton);
        draw_button(ren, &circleButton);

        if (preview && currentTool != TOOL_NONE) {
            render_preview(ren, currentTool, start_x, start_y, last_x, last_y, currentColor);
        }

        SDL_RenderPresent(ren);
    }

    SDL_DestroyTexture(canvas->texture);
    SDL_FreeSurface(canvas->surface);
    free(canvas);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
