#include <stdio.h>
#include <stdint.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include "render.h"
#include "game.h"
#include "input.h"


int ViewWidth = INITIAL_VIEW_WIDTH; 
int ViewHeight = INITIAL_VIEW_HEIGHT; 

SDL_Texture *direction_textures[9];


// Score stuff
uint64_t curr_score = -1;
uint64_t curr_highscore = -2;

TTF_Font *score_font;
SDL_Texture *score_texture;
SDL_Texture *highscore_texture;

// Menu stuff
SDL_Texture *menu_textures[GAME_MODE_COUNT];


// Initialize Textures
bool InitTextures(SDL_Renderer *renderer)
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

    // Load font
    highscore_texture = NULL;
    score_texture = NULL;

    score_font = TTF_OpenFont(_fontAsset, 24);
    if (score_font == NULL)
    {
        printf("Error loading font asset(%s): %s\n", _fontAsset, SDL_GetError());
        return false;
    }

    return true;
}

void UpdateScore(SDL_Renderer *renderer)
{
    char scoreText[32];
    SDL_Color scoreColor = {255, 255, 255};
    
    if (gamestate.score == curr_score)
    return;
    
    // Update score and destroy old texture
    curr_score = gamestate.score;
    if (score_texture != NULL)
    SDL_DestroyTexture(score_texture);
    
    
    snprintf(scoreText, 32, "%06lld\0", gamestate.score);
    SDL_Surface *scoreSurface = TTF_RenderText_Solid(score_font, scoreText, strlen(scoreText), scoreColor);
    
    score_texture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    SDL_DestroySurface(scoreSurface);
    
    if (gamestate.highscore == curr_highscore)
    return;
    
    // Update high score and destroy old texture
    curr_highscore = gamestate.score;
    if (highscore_texture != NULL)
    SDL_DestroyTexture(highscore_texture);
    
    snprintf(scoreText, 32, "%06lld\0", gamestate.highscore);
    scoreSurface = TTF_RenderText_Solid(score_font, scoreText, strlen(scoreText), scoreColor);
    
    highscore_texture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    SDL_DestroySurface(scoreSurface);
}


void Render(SDL_Renderer *renderer) 
{
    SDL_RenderClear(renderer);
    
    // Render next input
    SDL_FRect destRect = {SIDE_PADDING, ICON_HEIGHT / 2, ICON_WIDTH, ICON_HEIGHT};
    SDL_RenderTexture(renderer, direction_textures[ gamestate.current_mode->pattern[ gamestate.player_pos % gamestate.current_mode->pattern_size ] ], NULL, &destRect);
    
    // Render score and high score panel
    UpdateScore(renderer);
    
    destRect.x = ICON_WIDTH + SIDE_PADDING*2;
    destRect.y = 0; 
    destRect.w = SCORE_COUNTER_WIDTH;
    destRect.h = SCORE_COUNTER_HEIGHT;
    
    SDL_RenderTexture(renderer, score_texture, NULL, &destRect);
    
    destRect.y = SCORE_COUNTER_HEIGHT;
    SDL_RenderTexture(renderer, highscore_texture, NULL, &destRect);
    
    SDL_RenderPresent(renderer);
}

// Initialize the view for mode select
bool InitModeSelect(SDL_Renderer *renderer)
{
    char text[32];
    
    SDL_Color textColor = {255, 255, 255};

    for (int i = 0; i < GAME_MODE_COUNT; i++)
    {
        snprintf(text, 32, "%s", gamemodes[i].mode_name);
        SDL_Surface *surface = TTF_RenderText_Solid(score_font, text, strlen(text), textColor);

        menu_textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);

        if (menu_textures[i] == NULL )
        {
            printf("Error initializing mode select view: %s", SDL_GetError());
            return false;
        }
    }

    return true;
}

void RenderModeSelect(SDL_Renderer *renderer)
{
    SDL_RenderClear(renderer);

    SDL_FRect destRect = {SIDE_PADDING, ICON_HEIGHT / 2, INITIAL_VIEW_WIDTH - (SIDE_PADDING * 2), ICON_HEIGHT};
    SDL_RenderTexture(renderer, menu_textures[selected_mode], NULL, &destRect);

    SDL_RenderPresent(renderer);
}

// Destroy mode select textures from memory after game starts
void DestroyModeSelect()
{
    for (int i = 0; i < GAME_MODE_COUNT; i++)
        SDL_DestroyTexture(menu_textures[i]);
}