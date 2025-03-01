#pragma once

#include <SDL.h>
#include "input.h"

#define ICON_WIDTH 70
#define ICON_HEIGHT 70
#define KBD_INPUT_COUNT 11

#define INITIAL_VIEW_WIDTH ICON_WIDTH * KBD_INPUT_COUNT
#define INITIAL_VIEW_HEIGHT ICON_HEIGHT * 2

static const char * assetLocations[9] = {
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

bool Init_Textures(SDL_Renderer *);
bool Render_KBD_Sequence(SDL_Renderer *);