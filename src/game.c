#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "input.h"

ControllerState prev_input = {0};
int selected_mode = 0;

void InitGame()
{
    _initGameModes();
    
    gamestate.player_pos = 0;
    gamestate.score = 0;
    gamestate.highscore = 0;
    gamestate.failed = false;
    gamestate.run_game = false;
    gamestate.current_mode = gamemodes; 
}

void Update(ControllerState *cs)
{
    if (gamestate.run_game)
        _updateGame(cs);
    else
        _updateMenu(cs);
}

void _updateMenu(ControllerState *cs)
{
    if (cs->back_pressed == prev_input.back_pressed
        && cs->select_pressed == prev_input.select_pressed
        && cs->direction == prev_input.direction)
        return;

    prev_input = *cs;
    
    // Start game
    if (cs->select_pressed)
    {
        _startGame();
        return;
    }

    if (cs->direction == FORWARD)
    {
        if (selected_mode == GAME_MODE_COUNT - 1)
        selected_mode = 0;
        else
        selected_mode += 1;
    }
    else if (cs->direction == BACK)
    {
        if (selected_mode == 0)
        selected_mode = GAME_MODE_COUNT - 1;
        else
        selected_mode -= 1;
    }
    
    gamestate.current_mode = &gamemodes[selected_mode];
}

void _updateGame(ControllerState *cs)
{
    // Reset failed flag
    if (gamestate.failed)
        gamestate.failed = false;

    if (cs->back_pressed)
    {
        gamestate.run_game = false;
        return;
    }
    
    // No update if input has not changed
    if (cs->direction == prev_input.direction) return;
    
    if (cs->direction == gamestate.current_mode->pattern[ gamestate.player_pos % gamestate.current_mode->pattern_size ])
    {
        // Correct input
        gamestate.score += 50;
        if (gamestate.score > gamestate.highscore)
        gamestate.highscore = gamestate.score;
        
        if (gamestate.player_pos == gamestate.current_mode->pattern_size - 1)
        gamestate.player_pos = 0;
        else
        gamestate.player_pos += 1;
        
        prev_input.direction = cs->direction;
    }
    else
    {
        // Incorrect input
        // return player to start for now
        gamestate.player_pos = 0;
        gamestate.score = 0;
        
        gamestate.failed = true;
        gamestate.failed_input = cs->direction;
        
        prev_input.direction = cs->direction;
    }
}

void _startGame()
{
    gamestate.player_pos = 0;
    gamestate.score = 0;
    gamestate.highscore = 0;
    gamestate.failed = false;
    gamestate.run_game = true;
}

void DestroyGame()
{
    for(int i = 0; i < GAME_MODE_COUNT; i++)
    {
        free(gamemodes[i].pattern);
    }
}

void _initGameModes()
{
    GameDirection *pattern;

    // P1 KBD
    pattern = malloc(4 * sizeof(GameMode));
    pattern[0] = BACK;
    pattern[1] = NEUTRAL;
    pattern[2] = BACK;
    pattern[3] = DOWN_BACK;

    gamemodes[0].mode_name = "P1 KBD";
    gamemodes[0].pattern_size = 4;
    gamemodes[0].pattern = pattern;
    
    // P2 KBD
    pattern = malloc(4 * sizeof(GameMode));
    pattern[0] = FORWARD;
    pattern[1] = NEUTRAL;
    pattern[2] = FORWARD;
    pattern[3] = DOWN_FORWARD;

    gamemodes[1].mode_name = "P2 KBD";
    gamemodes[1].pattern_size = 4;
    gamemodes[1].pattern = pattern;

    // P1 WD
    pattern = malloc(6 * sizeof(GameMode));
    pattern[0] = FORWARD;
    pattern[1] = NEUTRAL;
    pattern[2] = DOWN;
    pattern[3] = DOWN_FORWARD;
    pattern[4] = FORWARD;
    pattern[5] = NEUTRAL;

    gamemodes[2].mode_name = "P1 WD";
    gamemodes[2].pattern_size = 6;
    gamemodes[2].pattern = pattern;


    // P2 WD
    pattern = malloc(6 * sizeof(GameMode));
    pattern[0] = BACK;
    pattern[1] = NEUTRAL;
    pattern[2] = DOWN;
    pattern[3] = DOWN_BACK;
    pattern[4] = BACK;
    pattern[5] = NEUTRAL;

    gamemodes[3].mode_name = "P2 WD";
    gamemodes[3].pattern_size = 6;
    gamemodes[3].pattern = pattern;

    printf("Gamemodes Initialized.\n");
}