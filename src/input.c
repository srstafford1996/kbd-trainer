#include <SDL3/SDL.h>

#include <stdbool.h>
#include <stdio.h>

#include "input.h"

SDL_Gamepad *gamepad = NULL;
short dpad_state;


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

GameDirection GetInput()
{
    GameDirection d;
    
    
    // Just gonna group all these into a bitmask following the XINPUT standard
    // so i dont have to rewrite the XINPUT implementation
    // it's 10pm on a weeknight
    
    // 0001 is UP, 0010 is DOWN, 0100 is LEFT, 1000 is RIGHT
    if (gamepad != NULL)
    {
        SDL_UpdateGamepads();
        dpad_state = 1 * SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP);
        dpad_state |= 2 * SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
        dpad_state |= 4 * SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
        dpad_state |= 8 * SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
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
    }

    switch( dpad_state )
    {
        // 0000
        case 0x0:
            d = NEUTRAL;
            break;
        // 0001
        case 0x1:
            d = UP;
            break;

        // 1001
        case 0x9:
            d = UP_FORWARD; 
            break;
        
        // 0101
        case 0x5:
            d = UP_BACK;
            break;
        
        // 0100
        case 0x4:
            d = BACK;
            break;
        
        // 1000
        case 0x8:
            d = FORWARD;
            break;
        
        // 0010
        case 0x2:
            d = DOWN;
            break;
        
        // 1010
        case 0xA:
            d = DOWN_FORWARD;
            break;
        
        // 0110
        case 0x6:
            d = DOWN_BACK;
            break;
        
        default:
            d = UNKNOWN; 
    }

    return d;
}