#ifndef INPUT_H
#define INPUT_H

#pragma comment(lib, "XInput.lib") 

#include <Xinput.h>
#include <stdbool.h>
#include <stdio.h>

extern bool isP1;

XINPUT_STATE controllerState;

typedef enum {
    NEUTRAL = 0,
    UP,
    UP_FORWARD,
    FORWARD,
    DOWN_FORWARD,
    DOWN,
    DOWN_BACK,
    BACK,
    UP_BACK,
    UNKNOWN,
    DISCONNECTED
} GameDirection;

static const GameDirection kbdPattern[4] = {BACK, NEUTRAL, BACK, DOWN_BACK};

static const char *DirectionNames[11] = {
    [0] = "*",
    [1] = "u",
    [2] = "u/f",
    [3] = "f",
    [4] = "d/f",
    [5] = "d",
    [6] = "d/b",
    [7] = "b",
    [8] = "u/b",
    [9] = "UNKNOWN",
    [10] = "DISCONNECTED"
};

GameDirection GetInput(DWORD userIndex);
DWORD GetControllerIndex();

#endif