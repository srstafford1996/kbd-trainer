#include <Xinput.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

#include "input.h"


int main() {

    SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *img = NULL;
    int w, h;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initializing SDL\n");
        return 1;
    }

    win = SDL_CreateWindow("Image Loading", 100, 100, 800, 600, 0);
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    img = IMG_LoadTexture(renderer, "assets\\n.bmp");
    SDL_QueryTexture(img, NULL, NULL, &w, &h);

    SDL_Rect icon;
    icon.x = 0;
    icon.y = 0;
    icon.w = 200;
    icon.h = 200;

    while (true) {

        SDL_Event e;
        if ( SDL_PollEvent(&e) ) {
            if (e.type == SDL_QUIT) {
                break;
            } else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
                break;
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, img, NULL, &icon);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(img);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    
    return 0;
}