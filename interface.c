#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "headers/allheaders.h"
#include "headers/buttons.h"
#include "headers/interface.h"

void initialize_working_zone(SDL_Renderer * renderer, work_zone * initializing_working_zone){
    initializing_working_zone->drect.x = 0;
    initializing_working_zone->drect.y = 100;
    initializing_working_zone->drect.w = SCREEN_WIDTH;
    initializing_working_zone->drect.h = SCREEN_HEIGHT;

    SDL_Surface *surface = SDL_CreateRGBSurface(0, 200, 150, 32, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));
    initializing_working_zone->texture = SDL_CreateTextureFromSurface(renderer, surface);
}



void interface(SDL_Renderer * renderer, SDL_Window * window){
    SDL_Event window_event;
    point button_coordinates;
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
    button_coordinates.y = brush_button.drect.y+50;
    button button_brush_1px = initialize_button_pxls(renderer, "1px", button_coordinates);
    button_coordinates.x += 55;
    button button_brush_2px = initialize_button_pxls(renderer, "2px", button_coordinates);
    button_coordinates.x += 55;
    button button_brush_3px = initialize_button_pxls(renderer, "3px", button_coordinates);

    button_coordinates.x = eraser_button.drect.x;
    button_coordinates.y = brush_button.drect.y+50;
    button button_eraser_1px = initialize_button_pxls(renderer, "1px", button_coordinates);
    button_coordinates.x += 55;
    button button_eraser_2px = initialize_button_pxls(renderer, "2px", button_coordinates);
    button_coordinates.x += 55;
    button button_eraser_3px = initialize_button_pxls(renderer, "3px", button_coordinates);

    button_coordinates.x = palette_button.drect.x;
    button_coordinates.y = palette_button.drect.y+50;

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
    initialize_working_zone(renderer, &working_zone);

    tool_type tool_menu_selected = NOTHING;
    tool_type tool_selected = BRUSH;

    bool is_drawing = false; 

    while(true){   
        SDL_Rect mouse_point;
        mouse_point.h = 1;
        mouse_point.w = 1;
        SDL_GetMouseState(&mouse_point.x, &mouse_point.y);

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

        if( tool_menu_selected == BRUSH){
            button_brush_1px = update_button(renderer, button_brush_1px, mouse_point);
            button_brush_2px = update_button(renderer, button_brush_2px, mouse_point);
            button_brush_3px = update_button(renderer, button_brush_3px, mouse_point);

            draw_button(&button_brush_1px, renderer);
            draw_button(&button_brush_2px, renderer);
            draw_button(&button_brush_3px, renderer);
        }
        if( tool_menu_selected == ERASER ){
            button_eraser_1px = update_button(renderer, button_eraser_1px, mouse_point);
            button_eraser_2px = update_button(renderer, button_eraser_2px, mouse_point);
            button_eraser_3px = update_button(renderer, button_eraser_3px, mouse_point);

            draw_button(&button_eraser_1px, renderer);
            draw_button(&button_eraser_2px, renderer);
            draw_button(&button_eraser_3px, renderer);
        }
        if( tool_menu_selected == PALETTE){
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

        if(SDL_PollEvent(&window_event)){
            if(window_event.type == SDL_QUIT){
                break;
            }
            if(window_event.type == SDL_MOUSEBUTTONDOWN){
                is_drawing = true;
            }
            if(window_event.type == SDL_MOUSEBUTTONUP){
                is_drawing = false;
                if(window_event.button.button == SDL_BUTTON_LEFT && brush_button.is_selected == true){
                
                    if (tool_menu_selected == BRUSH) {
                        tool_menu_selected = NOTHING;
                    }
                    else{
                        tool_menu_selected = BRUSH;
                    }
                    
                }
                else if(window_event.button.button == SDL_BUTTON_LEFT && eraser_button.is_selected == true){
                    if(tool_menu_selected == ERASER){
                        tool_menu_selected = NOTHING;
                    }
                    else{
                        tool_menu_selected = ERASER;
                    }
                }
                else if(window_event.button.button == SDL_BUTTON_LEFT && circle_button.is_selected == true){
                    printf("circle\n");
                }
                else if(window_event.button.button == SDL_BUTTON_LEFT && filling_button.is_selected == true){
                    printf("filling\n");
                }
                else if(window_event.button.button == SDL_BUTTON_LEFT && palette_button.is_selected == true){
                    if(tool_menu_selected == PALETTE){
                        tool_menu_selected = NOTHING;
                    }
                    else {
                        tool_menu_selected = PALETTE;
                    }
                }
                else if(window_event.button.button == SDL_BUTTON_LEFT && rectangle_button.is_selected == true){
                    printf("rectangle\n");
                }
                else if(window_event.button.button == SDL_BUTTON_LEFT && line_button.is_selected == true){
                    printf("line\n");
                }
                else if(window_event.button.button == SDL_BUTTON_LEFT && button_brush_1px.is_selected == true){
                    printf("1px\n");
                }
                else if(window_event.button.button == SDL_BUTTON_LEFT && button_brush_2px.is_selected == true){
                    printf("2px\n");
                }
                else if(window_event.button.button == SDL_BUTTON_LEFT && button_brush_3px.is_selected == true){
                    printf("3px\n");
                }

            }
            if (window_event.type == SDL_MOUSEMOTION){
                if(is_drawing){
                    // filledCircleRGBA(renderer, mouse_point.x, mouse_point.y, 1, 0, 0, 0, SDL_ALPHA_OPAQUE);
                }
            }
            
        }
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }

}