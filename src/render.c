#include <stdio.h>
#include <stdint.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "render.h"
#include "game.h"
#include "input.h"


int ViewWidth = INITIAL_VIEW_WIDTH; 
int ViewHeight = INITIAL_VIEW_HEIGHT; 

SDL_Texture *direction_textures[9];


//Score stuff
uint64_t curr_score = -1;
uint64_t curr_highscore = -2;

TTF_Font *score_font;
SDL_Texture *score_texture;
SDL_Texture *highscore_texture;


// Initialize Textures
bool Init_Textures(SDL_Renderer *renderer)
{
    SDL_Texture *curr = NULL;

    for (int i = 0; i < sizeof(_directionAssets) / sizeof(const char*); i++)
    {
        curr = IMG_LoadTexture(renderer, _directionAssets[i]);    
        if (curr == NULL)
        {
            printf("Error loading asset(%s): %s\n", _directionAssets[i], IMG_GetError());
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
        printf("Error loading font asset(%s): %s\n", _fontAsset, TTF_GetError());
        return false;
    }

    return true;
}

void Update_Score(SDL_Renderer *renderer)
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
    SDL_Surface *scoreSurface = TTF_RenderText_Solid(score_font, scoreText, scoreColor);
    
    score_texture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    SDL_FreeSurface(scoreSurface);

    if (gamestate.highscore == curr_highscore)
        return;
        
    // Update high score and destroy old texture
    curr_highscore = gamestate.score;
    if (highscore_texture != NULL)
        SDL_DestroyTexture(highscore_texture);

    snprintf(scoreText, 32, "%06lld\0", gamestate.highscore);
    scoreSurface = TTF_RenderText_Solid(score_font, scoreText, scoreColor);

    highscore_texture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    SDL_FreeSurface(scoreSurface);
}


bool Render(SDL_Renderer *renderer) 
{
    SDL_RenderClear(renderer);

    // Render next input
    SDL_Rect destRect = {SIDE_PADDING, ICON_HEIGHT / 2, ICON_WIDTH, ICON_HEIGHT};
    SDL_RenderCopy(renderer, direction_textures[ kbdPattern[ gamestate.player_pos % 4 ] ], NULL, &destRect);

    // Render score and high score panel
    Update_Score(renderer);

    destRect.x = ICON_WIDTH + SIDE_PADDING*2;
    destRect.y = 0; 
    destRect.w = SCORE_COUNTER_WIDTH;
    destRect.h = SCORE_COUNTER_HEIGHT;

    SDL_RenderCopy(renderer, score_texture, NULL, &destRect);

    destRect.y = SCORE_COUNTER_HEIGHT;
    SDL_RenderCopy(renderer, highscore_texture, NULL, &destRect);

    SDL_RenderPresent(renderer);

    return true;
}