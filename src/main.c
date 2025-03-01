#include <Xinput.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <SDL_image.h>


#include "render.h"
#include "input.h"


int main() {
    
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        SDL_Delay(2000);
        return 1;
    }

    /*
    if( !IMG_Init(0) ) {
        printf("Error initializing SDL Image: %s\n", IMG_GetError());
        SDL_Delay(2000);
        return 1;
    }
        */
    window = SDL_CreateWindow("KBD Trainer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, INITIAL_VIEW_WIDTH, INITIAL_VIEW_HEIGHT, SDL_WINDOW_SHOWN);
    if( window == NULL ) {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Delay(2000);
        return 1;
    }

    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    if( renderer == NULL ) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_Delay(2000);
        return 1;
    }
    
    if (Init_Textures(renderer)) {
        printf("Texture load success!\n");
    }

    SDL_RenderClear(renderer);
    Render_KBD_Sequence(renderer);
    SDL_RenderPresent(renderer);

    
    SDL_Delay(2000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}