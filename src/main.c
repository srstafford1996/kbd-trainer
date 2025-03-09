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

int main()
{ 
    bool isRunning = true;
    bool showGameView = false;
    
    // Frame timing
    const Uint64 FRAME_DELAY = 16666666;
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
    
    if (!InitMenuTextures(renderer))
    {
        printf("Error initializing mode select: %s", SDL_GetError());
        return 1;
    }
    else 
        printf("Menu loaded. Let's go\nPress LEFT/RIGHT to navigate\nA to confirm selection | B/Start to return to menu\n");
    
    
    while (isRunning)
    {
        prevFrame = frameStart;
        frameStart = SDL_GetTicksNS();
        while (SDL_PollEvent(&ev) != 0)
        {
            if (ev.type == SDL_EVENT_QUIT)
                isRunning = false;
        }

        // Switch to game view or menu view
        if (showGameView != gamestate.run_game)
        {
            showGameView = gamestate.run_game;
            if(gamestate.run_game)
                DestroyMenuTextures();
            else
                InitMenuTextures(renderer);
        }

        Update( PollController() );
        Render(renderer);

        // How long it took to execute the functions
        frameTime = SDL_GetTicksNS() - frameStart;

        //Wait out the remainder
        if (FRAME_DELAY > frameTime) 
            SDL_DelayPrecise(FRAME_DELAY - frameTime);
    }
    
    DestroyGame();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}