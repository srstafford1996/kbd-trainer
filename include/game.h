#pragma once

#define GAME_MODE_COUNT 4

#include <stdint.h>
#include "input.h"

typedef struct {
    const char * mode_name;

    GameDirection *pattern;
    int pattern_size;
} GameMode;

extern int selected_mode;
GameMode gamemodes[GAME_MODE_COUNT];


typedef struct{
    // current position in the pattern
    int player_pos;
    
    uint64_t score;
    uint64_t highscore;
    
    bool failed;
    GameDirection failed_input;
    
    GameMode *current_mode;
    bool start;
} GameState;

GameState gamestate;

void InitGame();
void DestroyGame();

void Update(GameDirection);
void UpdateModeSelect(GameDirection);