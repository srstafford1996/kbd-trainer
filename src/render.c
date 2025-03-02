#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#include "render.h"
#include "game.h"
#include "input.h"


int ViewWidth = INITIAL_VIEW_WIDTH; 
int ViewHeight = INITIAL_VIEW_HEIGHT; 

SDL_Texture *player_texture;
SDL_Texture *direction_textures[9];

// Initialize Textures
bool Init_Textures(SDL_Renderer *renderer)
{
    SDL_Texture *curr = NULL;

    for (int i = 0; i < sizeof(_directionAssets) / sizeof(const char*); i++)
    {
        curr = IMG_LoadTexture(renderer, _directionAssets[i]);    
        if (curr == NULL)
        {
            printf("Error loading asset(%s): %s\n", _directionAssets[i], SDL_GetError());
            return false;
        }

        direction_textures[i] = curr;
    }

    // Load player texture
    player_texture = IMG_LoadTexture(renderer, _playerAsset);
    {
        printf("Error loading asset(%s): %s\n", _playerAsset, SDL_GetError());
        return false;
    }

    return true;
}

// Generate KBD Rect Sequence 
bool Render_KBD_Sequence(SDL_Renderer *renderer)
{
    // Rect Initizliation: x, y, width, height
    SDL_Rect destRect = {ViewWidth, ICON_HEIGHT, ICON_WIDTH, ICON_HEIGHT};

    for(int i = 0; i < KBD_INPUT_COUNT; i++)
    {
        destRect.x -= ICON_WIDTH;

        // KBD Input at i % 4 represents the texture we want
        SDL_Texture *texture = direction_textures[ (int) kbdPattern[i % 4] ];
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
    }

    return true;
}

bool Render_Player(SDL_Renderer *renderer)
{
    SDL_Rect destRect = {ViewWidth, 0, ICON_WIDTH, ICON_HEIGHT};
    SDL_Texture *texture = NULL;

    // Render the successful inputs
    for(int i = 0; i < gamestate.player_pos; i++)
    {
        destRect.x -= ICON_WIDTH;
        texture = direction_textures[ (int) kbdPattern[i % 4] ];
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
    }
    
    // Then the actual player
    destRect.x -= ICON_WIDTH;
    texture = player_texture;
    SDL_RenderCopy(renderer, texture, NULL, &destRect);

    return true;
}

bool Render(SDL_Renderer *renderer) 
{
    SDL_RenderClear(renderer);

    Render_Player(renderer);
    Render_KBD_Sequence(renderer);
    
    SDL_RenderPresent(renderer);
}