#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "headers/allheaders.h"
#include "headers/buttons.h"
#include "headers/interface.h"


int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("MENU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (NULL == window){
        printf("Couldn't create window!");
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL){
        printf("Couldn't create renderer!");
        return 1;
    }
    SDL_SetRenderDrawColor(renderer, 85, 85, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Event window_event;
    point coordinates_buttons;
    
    coordinates_buttons.x = SCREEN_WIDTH/2-150;
    coordinates_buttons.y = SCREEN_HEIGHT/2-200;

    button new_file_button = initialize_button(renderer, "NEW", coordinates_buttons);
    coordinates_buttons.y += 100;
    button open_file_button = initialize_button(renderer, "OPEN", coordinates_buttons);
    coordinates_buttons.y += 100;
    button save_file_button = initialize_button(renderer, "SAVE", coordinates_buttons);
    coordinates_buttons.y += 100;
    button exit_button = initialize_button(renderer, "EXIT", coordinates_buttons);

    while(true){
        SDL_Rect mouse_point;
        mouse_point.h = 1;
        mouse_point.w = 1;
        SDL_GetMouseState(&mouse_point.x, &mouse_point.y);
        new_file_button = update_button(renderer, new_file_button, mouse_point);
        open_file_button = update_button(renderer, open_file_button, mouse_point);
        save_file_button = update_button(renderer, save_file_button, mouse_point);
        exit_button = update_button(renderer, exit_button, mouse_point);

        draw_button(&new_file_button, renderer);
        draw_button(&open_file_button, renderer);
        draw_button(&save_file_button, renderer);
        draw_button(&exit_button, renderer);
        if ( SDL_PollEvent ( &window_event ) ){
            if ( SDL_QUIT == window_event.type ){
                break;
            } 
            if(SDL_MOUSEBUTTONUP == window_event.type){
                if(window_event.button.button == SDL_BUTTON_LEFT){
                    if(new_file_button.is_selected == true){
                        interface(renderer, window);
                    }
                    else if(exit_button.is_selected == true){
                        break;
                    }
                    else if(open_file_button.is_selected == true){
                        printf("open\n");
                    }
                    else if(save_file_button.is_selected == true){
                        printf("save\n");
                    }
                }
            }
        }
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}