#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "headers/allheaders.h"
#include "headers/buttons.h"
#include "headers/interface.h"
#include "headers/filework.h"

int colours_list[9][3] = {
    {0, 0, 0},
    {0, 0, 255},
    {0, 255, 0},
    {255, 165, 0},
    {255, 20, 147},
    {148, 0, 211},
    {255, 0, 0},
    {255, 255, 255},
    {255, 255, 0}};

void initialize_working_zone(SDL_Renderer *renderer, work_zone *initializing_working_zone, int flag)
{
    initializing_working_zone->drect.x = 0;
    initializing_working_zone->drect.y = 100;
    initializing_working_zone->drect.w = SCREEN_WIDTH;
    initializing_working_zone->drect.h = SCREEN_HEIGHT - 100;
    if(flag == 0){  
        SDL_Surface *surface = SDL_CreateRGBSurface(0, initializing_working_zone->drect.w, initializing_working_zone->drect.h, 32, 0, 0, 0, 0);
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));
        initializing_working_zone->texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface); 
    }
    else {
        initializing_working_zone->texture = open_image(renderer);
    }
}

void interface(SDL_Renderer *renderer, SDL_Window *window, int flag)
{
    

    SDL_Event window_event;
    point button_coordinates;
    button_coordinates.x = 1100;
    button_coordinates.y = 30;
    button save_button = initialize_button_pxls(renderer, "SAVE", button_coordinates);
    button_coordinates.x = 10;
    button_coordinates.y = 10;
    button brush_button = initialize_image_button(renderer, "fonts_and_images/brush.png", button_coordinates, 48, 48);
    button_coordinates.x += 70;
    button eraser_button = initialize_image_button(renderer, "fonts_and_images/eraser.png", button_coordinates, 48, 48);
    button_coordinates.x += 70;
    button filling_button = initialize_image_button(renderer, "fonts_and_images/filling.png", button_coordinates, 48, 48);
    button_coordinates.x += 70;
    button palette_button = initialize_image_button(renderer, "fonts_and_images/palette.png", button_coordinates, 48, 48);
    button_coordinates.x += 70;
    button line_button = initialize_image_button(renderer, "fonts_and_images/line.png", button_coordinates, 48, 48);
    button_coordinates.x += 70;
    button circle_button = initialize_image_button(renderer, "fonts_and_images/circle.png", button_coordinates, 48, 48);
    button_coordinates.x += 70;
    button rectangle_button = initialize_image_button(renderer, "fonts_and_images/rectangle.png", button_coordinates, 48, 48);

    button_coordinates.x = brush_button.drect.x;
    button_coordinates.y = brush_button.drect.y + 50;
    button button_brush_1px = initialize_button_pxls(renderer, "1px", button_coordinates);
    button_coordinates.x += 55;
    button button_brush_2px = initialize_button_pxls(renderer, "2px", button_coordinates);
    button_coordinates.x += 55;
    button button_brush_3px = initialize_button_pxls(renderer, "3px", button_coordinates);

    button_coordinates.x = eraser_button.drect.x;
    button_coordinates.y = brush_button.drect.y + 50;
    button button_eraser_1px = initialize_button_pxls(renderer, "1px", button_coordinates);
    button_coordinates.x += 55;
    button button_eraser_2px = initialize_button_pxls(renderer, "2px", button_coordinates);
    button_coordinates.x += 55;
    button button_eraser_3px = initialize_button_pxls(renderer, "3px", button_coordinates);

    button_coordinates.x = palette_button.drect.x;
    button_coordinates.y = palette_button.drect.y + 50;

    button button_palette_black = initialize_image_button(renderer, "fonts_and_images/black.png", button_coordinates, 32, 32);
    button_coordinates.x += 45;
    button button_palette_blue = initialize_image_button(renderer, "fonts_and_images/blue.png", button_coordinates, 32, 32);
    button_coordinates.x += 45;
    button button_palette_green = initialize_image_button(renderer, "fonts_and_images/green.png", button_coordinates, 32, 32);
    button_coordinates.x += 45;
    button button_palette_orange = initialize_image_button(renderer, "fonts_and_images/orange.png", button_coordinates, 32, 32);
    button_coordinates.x += 45;
    button button_palette_pink = initialize_image_button(renderer, "fonts_and_images/pink.png", button_coordinates, 32, 32);
    button_coordinates.x += 45;
    button button_palette_purple = initialize_image_button(renderer, "fonts_and_images/purple.png", button_coordinates, 32, 32);
    button_coordinates.x += 45;
    button button_palette_red = initialize_image_button(renderer, "fonts_and_images/red.png", button_coordinates, 32, 32);
    button_coordinates.x += 45;
    button button_palette_white = initialize_image_button(renderer, "fonts_and_images/white.png", button_coordinates, 32, 32);
    button_coordinates.x += 45;
    button button_palette_yellow = initialize_image_button(renderer, "fonts_and_images/yellow.png", button_coordinates, 32, 32);

    work_zone working_zone;
    initialize_working_zone(renderer, &working_zone, flag);

    colour_type colour_selected = BLACK;
    tool_type tool_selected = BRUSH;
    tool_type tool_menu_selected = NOTHING;
    int brush_size = 1;

    bool is_drawing = false;
    bool is_up = false;
    
    // SDL_Texture *drawing_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, working_zone.drect.w, working_zone.drect.h);
    SDL_Texture *drawing_texture = open_image(renderer);
    SDL_Texture *tmp_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, working_zone.drect.w, working_zone.drect.h);
    SDL_SetRenderTarget(renderer, drawing_texture);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, tmp_texture);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetTextureBlendMode(drawing_texture, SDL_BLENDMODE_BLEND);


    Uint32 last_button_up_time = 0;

    int mouse_start_x = 0, mouse_start_y = 0;
    int radius = 0;

    while (true)
    {
        SDL_SetTextureBlendMode(drawing_texture, SDL_BLENDMODE_MUL);
        SDL_Rect mouse_point;
        mouse_point.h = 1;
        mouse_point.w = 1;
        SDL_GetMouseState(&mouse_point.x, &mouse_point.y);

        save_button = update_button(renderer, save_button, mouse_point);
        draw_button(&save_button, renderer);

        update_image_button(renderer, &brush_button, &mouse_point);
        update_image_button(renderer, &eraser_button, &mouse_point);
        update_image_button(renderer, &palette_button, &mouse_point);
        update_image_button(renderer, &filling_button, &mouse_point);
        update_image_button(renderer, &circle_button, &mouse_point);
        update_image_button(renderer, &rectangle_button, &mouse_point);
        update_image_button(renderer, &line_button, &mouse_point);

        draw_image_button(renderer, &brush_button);
        draw_image_button(renderer, &eraser_button);
        draw_image_button(renderer, &palette_button);
        draw_image_button(renderer, &filling_button);
        draw_image_button(renderer, &circle_button);
        draw_image_button(renderer, &rectangle_button);
        draw_image_button(renderer, &line_button);

        SDL_RenderCopy(renderer, working_zone.texture, NULL, &working_zone.drect);
        SDL_RenderCopy(renderer, tmp_texture, NULL, &working_zone.drect);
        SDL_RenderCopy(renderer, drawing_texture, NULL, &working_zone.drect);

        if (tool_menu_selected == BRUSH)
        {
            button_brush_1px = update_button(renderer, button_brush_1px, mouse_point);
            button_brush_2px = update_button(renderer, button_brush_2px, mouse_point);
            button_brush_3px = update_button(renderer, button_brush_3px, mouse_point);

            draw_button(&button_brush_1px, renderer);
            draw_button(&button_brush_2px, renderer);
            draw_button(&button_brush_3px, renderer);
        }
        if (tool_menu_selected == ERASER)
        {
            button_eraser_1px = update_button(renderer, button_eraser_1px, mouse_point);
            button_eraser_2px = update_button(renderer, button_eraser_2px, mouse_point);
            button_eraser_3px = update_button(renderer, button_eraser_3px, mouse_point);

            draw_button(&button_eraser_1px, renderer);
            draw_button(&button_eraser_2px, renderer);
            draw_button(&button_eraser_3px, renderer);
        }
        if (tool_menu_selected == PALETTE)
        {
            update_image_button(renderer, &button_palette_black, &mouse_point);
            update_image_button(renderer, &button_palette_blue, &mouse_point);
            update_image_button(renderer, &button_palette_green, &mouse_point);
            update_image_button(renderer, &button_palette_orange, &mouse_point);
            update_image_button(renderer, &button_palette_pink, &mouse_point);
            update_image_button(renderer, &button_palette_purple, &mouse_point);
            update_image_button(renderer, &button_palette_red, &mouse_point);
            update_image_button(renderer, &button_palette_white, &mouse_point);
            update_image_button(renderer, &button_palette_yellow, &mouse_point);

            draw_image_button(renderer, &button_palette_black);
            draw_image_button(renderer, &button_palette_blue);
            draw_image_button(renderer, &button_palette_green);
            draw_image_button(renderer, &button_palette_orange);
            draw_image_button(renderer, &button_palette_pink);
            draw_image_button(renderer, &button_palette_purple);
            draw_image_button(renderer, &button_palette_red);
            draw_image_button(renderer, &button_palette_white);
            draw_image_button(renderer, &button_palette_yellow);
        }

        if (SDL_PollEvent(&window_event))
        {
            if (window_event.type == SDL_QUIT)
            {
                break;
            }

            if (window_event.type == SDL_MOUSEBUTTONDOWN)
            {
                last_button_up_time = 0;
                is_up = false;
                is_drawing = true;
                mouse_start_x = mouse_point.x;
                mouse_start_y = mouse_point.y;

                if(tool_selected == BRUSH){
                    SDL_SetRenderTarget(renderer, drawing_texture);
                    filledCircleRGBA(renderer, mouse_point.x - working_zone.drect.x, mouse_point.y - working_zone.drect.y, brush_size, colours_list[colour_selected][0], colours_list[colour_selected][1], colours_list[colour_selected][3], SDL_ALPHA_OPAQUE);
                    SDL_SetRenderTarget(renderer, NULL);
                }
            }

            if (window_event.type == SDL_MOUSEBUTTONUP)
            {
                is_up = true;

                if(window_event.button.button == SDL_BUTTON_LEFT && save_button.is_selected == true){
                    save_image(renderer, "saved.bmp");
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && brush_button.is_selected == true)
                {
                    if (tool_menu_selected == BRUSH)
                    {
                        tool_menu_selected = NOTHING;
                    }
                    else
                    {
                        tool_menu_selected = BRUSH;
                        tool_selected = BRUSH;
                    }
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && eraser_button.is_selected == true)
                {
                    if (tool_menu_selected == ERASER)
                    {
                        tool_menu_selected = NOTHING;
                    }
                    else
                    {
                        tool_menu_selected = ERASER;
                        tool_selected = ERASER;
                    }
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && circle_button.is_selected == true)
                {
                    tool_selected = CIRCLE;
                    tool_menu_selected = NOTHING;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && filling_button.is_selected == true)
                {
                    printf("feeling\n");
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && palette_button.is_selected == true)
                {
                    if (tool_menu_selected == PALETTE)
                    {
                        tool_menu_selected = NOTHING;
                    }
                    else
                    {
                        tool_menu_selected = PALETTE;
                    }
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && rectangle_button.is_selected == true)
                {
                    tool_selected = RECTANGLE;
                    tool_menu_selected = NOTHING;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && line_button.is_selected == true)
                {
                    tool_selected = LINE;
                    tool_menu_selected = NOTHING;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && button_brush_1px.is_selected == true)
                {
                    brush_size = 1;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && button_brush_2px.is_selected == true)
                {
                    brush_size = 2;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && button_brush_3px.is_selected == true)
                {
                    brush_size = 3;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && button_eraser_1px.is_selected == true)
                {
                    colour_selected = WHITE;
                    brush_size = 1;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && button_eraser_2px.is_selected == true)
                {
                    colour_selected = WHITE;
                    brush_size = 2;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && button_eraser_3px.is_selected == true)
                {
                    colour_selected = WHITE;
                    brush_size = 3;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && button_palette_black.is_selected == true)
                {
                    colour_selected = BLACK;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && button_palette_blue.is_selected == true)
                {
                    colour_selected = BLUE;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && button_palette_green.is_selected == true)
                {
                    colour_selected = GREEN;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && button_palette_red.is_selected == true)
                {
                    colour_selected = RED;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && button_palette_white.is_selected == true)
                {
                    colour_selected = WHITE;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && button_palette_yellow.is_selected == true)
                {
                    colour_selected = YELLOW;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && button_palette_orange.is_selected == true)
                {
                    colour_selected = ORANGE;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && button_palette_pink.is_selected == true)
                {
                    colour_selected = PINK;
                }
                else if (window_event.button.button == SDL_BUTTON_LEFT && button_palette_purple.is_selected == true)
                {
                    colour_selected = PURPLE;
                }
            }
            if (window_event.type == SDL_MOUSEMOTION)
            {
                if (is_drawing)
                {
                    SDL_SetRenderTarget(renderer, drawing_texture);

                    if (tool_selected == CIRCLE)
                    {
                        SDL_SetRenderTarget(renderer, tmp_texture);
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderClear(renderer);

                        radius = SDL_sqrt(SDL_pow(mouse_start_x - mouse_point.x, 2.0f) + SDL_pow(mouse_start_y - mouse_point.y, 2.0f));
                        circleRGBA(renderer, mouse_point.x - working_zone.drect.x, mouse_point.y - working_zone.drect.y, radius, colours_list[colour_selected][0], colours_list[colour_selected][1], colours_list[colour_selected][2], SDL_ALPHA_OPAQUE);
                    }

                    if (tool_selected == RECTANGLE)
                    {
                        SDL_SetRenderTarget(renderer, tmp_texture);
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderClear(renderer);

                        rectangleRGBA(renderer, mouse_start_x - working_zone.drect.x, mouse_start_y- working_zone.drect.y, mouse_point.x - working_zone.drect.x, mouse_point.y- working_zone.drect.y, colours_list[colour_selected][0], colours_list[colour_selected][1], colours_list[colour_selected][2], SDL_ALPHA_OPAQUE);
                    }

                    if (tool_selected == LINE)
                    {
                        SDL_SetRenderTarget(renderer, tmp_texture);
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderClear(renderer);

                        lineRGBA(renderer, mouse_start_x - working_zone.drect.x, mouse_start_y - working_zone.drect.y, mouse_point.x - working_zone.drect.x, mouse_point.y - working_zone.drect.y, colours_list[colour_selected][0], colours_list[colour_selected][1], colours_list[colour_selected][2], SDL_ALPHA_OPAQUE);
                    }

                    if (tool_selected == BRUSH || tool_selected == ERASER)
                    {
                        filledCircleRGBA(renderer, mouse_point.x - working_zone.drect.x, mouse_point.y - working_zone.drect.y, brush_size * 2, colours_list[colour_selected][0], colours_list[colour_selected][1], colours_list[colour_selected][2], SDL_ALPHA_OPAQUE);
                    }

                    SDL_SetRenderTarget(renderer, NULL);
                }
            }
        }

        if (is_drawing && is_up)
        {
            if (last_button_up_time == 0)
            {
                last_button_up_time = SDL_GetTicks();
            }

            if (SDL_GetTicks() - last_button_up_time > 50)
            {
                is_drawing = false;
                is_up = false;
                last_button_up_time = 0;

                if (tool_selected == CIRCLE)
                {
                    SDL_SetRenderTarget(renderer, tmp_texture);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderClear(renderer);

                    SDL_SetRenderTarget(renderer, drawing_texture);

                    radius = SDL_sqrt(SDL_pow(mouse_start_x - mouse_point.x, 2.0f) + SDL_pow(mouse_start_y - mouse_point.y, 2.0f));
                    circleRGBA(renderer, mouse_point.x - working_zone.drect.x, mouse_point.y - working_zone.drect.y, radius, colours_list[colour_selected][0], colours_list[colour_selected][1], colours_list[colour_selected][2], SDL_ALPHA_OPAQUE);

                }

                if (tool_selected == RECTANGLE)
                {
                    SDL_SetRenderTarget(renderer, tmp_texture);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderClear(renderer);

                    SDL_SetRenderTarget(renderer, drawing_texture);
                    rectangleRGBA(renderer, mouse_start_x - working_zone.drect.x, mouse_start_y - working_zone.drect.y, mouse_point.x - working_zone.drect.x, mouse_point.y - working_zone.drect.y, colours_list[colour_selected][0], colours_list[colour_selected][1], colours_list[colour_selected][2], SDL_ALPHA_OPAQUE);
                }

                if (tool_selected == LINE)
                {
                    SDL_SetRenderTarget(renderer, tmp_texture);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderClear(renderer);

                    SDL_SetRenderTarget(renderer, drawing_texture);
                    lineRGBA(renderer, mouse_start_x - working_zone.drect.x, mouse_start_y - working_zone.drect.y, mouse_point.x - working_zone.drect.x, mouse_point.y - working_zone.drect.y, colours_list[colour_selected][0], colours_list[colour_selected][1], colours_list[colour_selected][2], SDL_ALPHA_OPAQUE);
                }

                SDL_SetRenderTarget(renderer, NULL);
            }
        }

        SDL_SetRenderDrawColor(renderer, 85, 85, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderPresent(renderer);

        if (!is_drawing)
        {
            SDL_RenderClear(renderer);
        }
    }

    SDL_DestroyTexture(drawing_texture);
}
