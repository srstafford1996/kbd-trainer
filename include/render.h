#pragma once

#include <SDL.h>

#include "input.h"
#include "game.h"

#define ICON_WIDTH 70
#define ICON_HEIGHT 70

#define SIDE_PADDING 30

#define SCORE_COUNTER_WIDTH 200
#define SCORE_COUNTER_HEIGHT 70

#define INITIAL_VIEW_WIDTH ICON_WIDTH + SCORE_COUNTER_WIDTH + (SIDE_PADDING * 3)
#define INITIAL_VIEW_HEIGHT ICON_HEIGHT * 2

static const char * _directionAssets[9] = {
    [0] = "assets/n.bmp",
    [1] = "assets/u.bmp",
    [2] = "assets/uf.bmp",
    [3] = "assets/f.bmp",
    [4] = "assets/df.bmp",
    [5] = "assets/d.bmp",
    [6] = "assets/db.bmp",
    [7] = "assets/b.bmp",
    [8] = "assets/ub.bmp"
};

static const char * _playerAsset = "assets/ez1.png";
static const char * _fontAsset = "assets/scorefont.ttf";

bool Init_Textures(SDL_Renderer *);
bool Render(SDL_Renderer *);