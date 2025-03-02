#pragma once
#include "input.h"

#define KBD_INPUT_COUNT 11

typedef struct{
    int player_pos;
} GameState;

GameState gamestate;

void Init_Game();
void Update(GameDirection);