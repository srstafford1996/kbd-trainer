#ifndef INPUT_H
#define INPUT_H

#pragma comment(lib, "XInput.lib") 

#include <Xinput.h>
#include <stdbool.h>
#include <stdio.h>

extern bool isP1;

XINPUT_STATE controllerState;

typedef enum {
    UNKNOWN = 0,
    DISCONNECTED,
    NEUTRAL,
    UP,
    UP_FORWARD,
    FORWARD,
    DOWN_FORWARD,
    DOWN,
    DOWN_BACK,
    BACK,
    UP_BACK
} GameDirection;

static const char *DirectionNames[11] = {
    [0] = "UNKNOWN",
    [1] = "DISCONNECTED",
    [2] = "*",
    [3] = "u",
    [4] = "u/f",
    [5] = "f",
    [6] = "d/f",
    [7] = "d",
    [8] = "d/b",
    [9] = "b",
    [10] = "u/b"
};

GameDirection GetInput(DWORD userIndex);
DWORD GetControllerIndex();

#endif