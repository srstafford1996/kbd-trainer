#pragma once

#define MAX_PATH_LEN 1024;
#define ICON_WIDTH 100

#include <windows.h>
#include <stdbool.h>

extern bool r_initialized;

typedef struct {
    HBITMAP bm;
    int bmWidth;
    int bmHeight;
} r_DirectionIcon;

extern r_DirectionIcon r_directionIcons[9];

const static TCHAR* r_iconPaths[9] = {
    [0] = TEXT("assets\\n.bmp"),
    [1] = TEXT("assets\\u.bmp"),
    [2] = TEXT("assets\\uf.bmp"),
    [3] = TEXT("assets\\f.bmp"),
    [4] = TEXT("assets\\df.bmp"),
    [5] = TEXT("assets\\d.bmp"),
    [6] = TEXT("assets\\db.bmp"),
    [7] = TEXT("assets\\b.bmp"),
    [8] = TEXT("assets\\ub.bmp")
};

bool LoadRenderAssets();