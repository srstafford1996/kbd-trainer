#include <SDL3/SDL.h>

#include <stdbool.h>
#include <stdio.h>

#include "input.h"

SDL_Gamepad *gamepad = NULL;

ControllerState controller_state = {0};

// 4 DIGIT BITMASK FOR DIRECTIONS
short dpad_state = 0;


bool InitController()
{
    SDL_SetGamepadEventsEnabled(false);

    int numOfPads = 0;
    SDL_JoystickID *padIds = SDL_GetGamepads(&numOfPads);

    if (numOfPads == 0)
        return false;
    
    for (int i = 0; i < numOfPads; i++)
    {
        // Get first compatible gamepad
        if (SDL_IsGamepad(padIds[i]))
        {
            gamepad = SDL_OpenGamepad(padIds[i]);
            if (gamepad == NULL)
            {
                printf("Error initializing gamepad: %s", SDL_GetError());
                return false;
            }  
            
            return true;
        }
    }    

    return false;
}

ControllerState *PollController()
{

    // Just gonna group all these into a bitmask following the XINPUT standard
    // so i dont have to rewrite the XINPUT implementation
    // it's 10pm on a weeknight
    if (gamepad != NULL)
    {
        // 0001 is UP, 0010 is DOWN, 0100 is LEFT, 1000 is RIGHT
        SDL_UpdateGamepads();
        dpad_state = 1 * SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP);
        dpad_state |= 2 * SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
        dpad_state |= 4 * SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
        dpad_state |= 8 * SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);

        // For Buttons...
        controller_state.select_pressed = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_SOUTH);
        controller_state.back_pressed = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_EAST) | SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_START);
    }
    else
    {
        // PumpEvents updates keyboard state
        // GetKeyboardState returns a bool array of keys
        // array is indexed by enumeration
        SDL_PumpEvents();
        const bool *keys = SDL_GetKeyboardState(NULL);
    
        dpad_state = 1 * keys[SDL_SCANCODE_W];
        dpad_state |= 2 * keys[SDL_SCANCODE_S];
        dpad_state |= 4 * keys[SDL_SCANCODE_A];
        dpad_state |= 8 * keys[SDL_SCANCODE_D];

        controller_state.select_pressed = keys[SDL_SCANCODE_SPACE];
        controller_state.back_pressed = keys[SDL_SCANCODE_ESCAPE];
    }
    
    _parseDirection();

    return &controller_state;
}

void _parseDirection()
{
    switch( dpad_state & 0xF )
    {
        // 0000
        case 0x0:
            controller_state.direction = NEUTRAL;
            break;
        // 0001
        case 0x1:
            controller_state.direction = UP;
            break;
    
        // 1001
        case 0x9:
            controller_state.direction = UP_FORWARD; 
            break;
        
        // 0101
        case 0x5:
            controller_state.direction = UP_BACK;
            break;
        
        // 0100
        case 0x4:
            controller_state.direction = BACK;
            break;
        
        // 1000
        case 0x8:
            controller_state.direction = FORWARD;
            break;
        
        // 0010
        case 0x2:
            controller_state.direction = DOWN;
            break;
        
        // 1010
        case 0xA:
            controller_state.direction = DOWN_FORWARD;
            break;
        
        // 0110
        case 0x6:
            controller_state.direction = DOWN_BACK;
            break;
        
        default:
            controller_state.direction = UNKNOWN; 
    }
}