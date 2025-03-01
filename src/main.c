#include <Xinput.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <SDL_image.h>


#include "render.h"
#include "input.h"

const int FPS = 60;

int main()
{ 
    bool isRunning = true;
    
    // Frame timing
    const Uint64 FRAME_DELAY = 1000 / FPS;
    Uint64 frameStart = 0;
    Uint64 frameTime = 0;    
    Uint64 prevFrame = 0;
    
    // SDL Window Management
    SDL_Event ev;    
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        SDL_Delay(2000);
        return 1;
    }
    
    IMG_Init(0);
    
    window = SDL_CreateWindow("KBD Trainer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, INITIAL_VIEW_WIDTH, INITIAL_VIEW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Delay(2000);
        return 1;
    }
    
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_Delay(2000);
        return 1;
    }
    
    if (Init_Textures(renderer))
        printf("Texture load success!\n");
    
    
    while (isRunning)
    {
        prevFrame = frameStart;
        frameStart = SDL_GetTicks64();
        
        while (SDL_PollEvent(&ev) != 0)
        {
            if (ev.type == SDL_QUIT)
                isRunning = false;
        }

        SDL_RenderClear(renderer);
        Render_KBD_Sequence(renderer);
        SDL_RenderPresent(renderer);

        // How long it took to execute the functions
        frameTime = SDL_GetTicks64() - frameStart;

        //Wait out the remainder
        if (FRAME_DELAY > frameTime) 
            SDL_Delay(FRAME_DELAY - frameTime);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}