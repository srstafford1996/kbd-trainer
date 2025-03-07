#include <Xinput.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>


#include "render.h"
#include "input.h"
#include "game.h"

const int FPS = 60;

int main()
{ 
    bool isRunning = true;
    bool showModeSelect = true;
    
    // Frame timing
    const Uint64 FRAME_DELAY = 1000 / FPS;
    Uint64 frameStart = 0;
    Uint64 frameTime = 0;    
    Uint64 prevFrame = 0;
    
    // SDL Window Management
    SDL_Event ev;    
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) < 0)
    {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        SDL_Delay(2000);
        return 1;
    }
    
    TTF_Init();

    if(InitController())
        printf("Controller found!\n");
    else
        printf("No compatible controller detected. Using keyboard inputs (WASD)\n");
    
    SDL_CreateWindowAndRenderer("KBD Trainer", INITIAL_VIEW_WIDTH, INITIAL_VIEW_HEIGHT, 0, &window, &renderer);
    if (window == NULL)
    {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Delay(2000);
        return 1;
    }
    
    InitGame();
    
    if (InitTextures(renderer))
        printf("Texture load success!\n");
    
    if (!InitModeSelect(renderer))
    {
        printf("Error initializing mode select: %s", SDL_GetError());
        return;
    }
    else 
        printf("Menu loaded. Let's go\nPress LEFT/RIGHT to navigate, UP to confirm selection\n");
    
    while (isRunning)
    {
        prevFrame = frameStart;
        frameStart = SDL_GetTicks();
        
        while (SDL_PollEvent(&ev) != 0)
        {
            if (ev.type == SDL_EVENT_QUIT)
                isRunning = false;
        }

        if (showModeSelect)
        {
            if (gamestate.start)
            {
                showModeSelect = false;
                DestroyModeSelect();
                printf("Starting mode: %s\n", gamestate.current_mode->mode_name);
                continue;
            }

            UpdateModeSelect( GetInput() );
            RenderModeSelect(renderer);
        }
        else 
        {
            Update( GetInput() );
            Render(renderer);
        }        

        // How long it took to execute the functions
        frameTime = SDL_GetTicks() - frameStart;

        //Wait out the remainder
        if (FRAME_DELAY > frameTime) 
            SDL_Delay(FRAME_DELAY - frameTime);
    }
    
    DestroyGame();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}