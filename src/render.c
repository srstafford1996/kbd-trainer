#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#include "render.h"
#include "input.h"

int ViewWidth = INITIAL_VIEW_WIDTH; 
int ViewHeight = INITIAL_VIEW_HEIGHT; 

SDL_Texture *direction_textures[9];

// Initialize Textures
bool Init_Textures(SDL_Renderer *renderer)
{
    SDL_Texture *curr = NULL;
    SDL_Surface *surface = NULL;

    for (int i = 0; i < sizeof(assetLocations) / sizeof(const char*); i++)
    {
        curr = IMG_LoadTexture(renderer, assetLocations[i]);    
        if (curr == NULL)
        {
            printf("Error loading asset(%s): %s\n", assetLocations[i], SDL_GetError());
            return false;
        }

        direction_textures[i] = curr;
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