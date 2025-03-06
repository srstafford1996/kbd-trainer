#pragma once
#include <stdint.h>
#include "input.h"

typedef struct{
    // Number 0 to 3 representing position in the kbd string
    int player_pos;

    uint64_t score;
    uint64_t highscore;
    
    bool failed;
    GameDirection failed_input;
} GameState;

GameState gamestate;

void InitGame();
void Update(GameDirection);