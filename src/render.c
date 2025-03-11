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

// Input accuracy stuff
SDL_Texture *acc_textures[3];

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

    // Build input acc result textures
    SDL_Surface *surface;
    acc_textures[0] = NULL;
    
    SDL_Color successColor =  {51, 255, 51};
    const char *successText = "GREAT";
    surface = TTF_RenderText_Solid(score_font, successText, strlen(successText), successColor);
    acc_textures[SUCCESS] = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    
    SDL_Color failColor = {255, 51, 51};
    const char *failText = "MISS";
    surface = TTF_RenderText_Solid(score_font, failText, strlen(failText), failColor);
    acc_textures[FAIL] = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
 
    return true;
}

void _updateScore(SDL_Renderer *renderer)
{
    char scoreText[32];
    SDL_Color scoreColor = {255, 255, 255};
    if (gamestate.score == curr_score && gamestate.highscore == curr_highscore)
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
    curr_highscore = gamestate.highscore;
    if (highscore_texture != NULL)
    SDL_DestroyTexture(highscore_texture);
    
    snprintf(scoreText, 32, "%06lld\0", gamestate.highscore);
    scoreSurface = TTF_RenderText_Solid(score_font, scoreText, strlen(scoreText), scoreColor);
    
    highscore_texture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    SDL_DestroySurface(scoreSurface);
}



// Initialize the view for mode select
bool InitMenuTextures(SDL_Renderer *renderer)
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

// Destroy mode select textures from memory after game starts
void DestroyMenuTextures()
{
    for (int i = 0; i < GAME_MODE_COUNT; i++)
    SDL_DestroyTexture(menu_textures[i]);
}

void Render(SDL_Renderer *renderer)
{
    if (gamestate.run_game) 
        _renderGame(renderer);
    else
        _renderMenu(renderer);
}

void _renderMenu(SDL_Renderer *renderer)
{
    SDL_RenderClear(renderer);
    
    SDL_FRect destRect = {SIDE_PADDING, INITIAL_VIEW_HEIGHT / 2 - ICON_HEIGHT / 2, INITIAL_VIEW_WIDTH - (SIDE_PADDING * 2), ICON_HEIGHT};
    SDL_RenderTexture(renderer, menu_textures[selected_mode], NULL, &destRect);
    
    SDL_RenderPresent(renderer);
}

// ************* GAME RENDER ******************//
SDL_FRect next_input_rect = {
    SIDE_PADDING,
    (INITIAL_VIEW_HEIGHT / 2) - (ICON_HEIGHT / 2),
    ICON_WIDTH,
    ICON_HEIGHT
};

SDL_FRect score_rect = {
    ICON_WIDTH + SIDE_PADDING * 2,
    VERT_PADDING,
    SCORE_COUNTER_WIDTH,
    SCORE_COUNTER_HEIGHT
};

SDL_FRect highscore_rect = {
    ICON_WIDTH + SIDE_PADDING * 2,
    SCORE_COUNTER_HEIGHT + VERT_PADDING,
    SCORE_COUNTER_WIDTH,
    SCORE_COUNTER_HEIGHT
};

SDL_FRect last_input_acc_rect = {
    (ICON_WIDTH + SIDE_PADDING * 2) / 2 - ACC_DISPLAY_WIDTH / 2,
    INITIAL_VIEW_HEIGHT - ACC_DISPLAY_HEIGHT - VERT_PADDING,
    ACC_DISPLAY_WIDTH,
    ACC_DISPLAY_HEIGHT
};

SDL_FRect failed_input_rect = {
    (ICON_WIDTH + SIDE_PADDING * 2) / 2 - ACC_DISPLAY_WIDTH / 2,
    INITIAL_VIEW_HEIGHT - VERT_PADDING - 27,
    24,
    24
};

SDL_FRect failed_acc_rect = {
    (ICON_WIDTH + SIDE_PADDING * 2) / 2 - ACC_DISPLAY_WIDTH / 2 + 30,
    INITIAL_VIEW_HEIGHT - ACC_DISPLAY_HEIGHT - VERT_PADDING,
    ACC_DISPLAY_WIDTH - 30,
    ACC_DISPLAY_HEIGHT
};


void _renderGame(SDL_Renderer *renderer) 
{
    SDL_RenderClear(renderer);
    
    // Render inputs
    SDL_Texture *nextInputTexture = direction_textures[ gamestate.current_mode->pattern[ gamestate.player_pos % gamestate.current_mode->pattern_size ] ];
    SDL_RenderTexture(renderer, nextInputTexture, NULL, &next_input_rect);
    
    // Render score and high score panel
    _updateScore(renderer);
    SDL_RenderTexture(renderer, score_texture, NULL, &score_rect);
    SDL_RenderTexture(renderer, highscore_texture, NULL, &highscore_rect);
    
    // Success fail panel
    if (gamestate.last_input_acc == FAIL)
    {
        // First the arrow on the left
        SDL_RenderTexture(renderer, direction_textures[ gamestate.last_input ], NULL, &failed_input_rect);
        SDL_RenderTexture(renderer, acc_textures[ gamestate.last_input_acc ], NULL, &failed_acc_rect);
        
        SDL_RenderPresent(renderer);

        // Multiple delay calls to prevent Windows from "not responding"
        SDL_DelayPrecise(500000000);
        SDL_DelayPrecise(500000000);
        SDL_DelayPrecise(200000000);

        return;
    }
    else if (gamestate.last_input_acc != NONE)
    {
        /* TODO: Figure out how to display success in a way that doesn't look fucking stupid */
        //SDL_RenderTexture(renderer, acc_textures[ gamestate.last_input_acc ], NULL, &last_input_acc_rect);
    }

    SDL_RenderPresent(renderer);
}
