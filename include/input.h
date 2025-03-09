#pragma once

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

typedef struct {
    GameDirection direction;
    bool select_pressed;
    bool back_pressed;
} ControllerState;

bool InitController();
ControllerState *PollController();
void _parseDirection();