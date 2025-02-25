#include <windows.h>
#include <Xinput.h>
#include <stdbool.h>
#include <stdio.h>

#include "input.h"

bool isP1 = true;

GameDirection GetInput(DWORD userIndex) {
    GameDirection d;
    
    ZeroMemory( &controllerState, sizeof(XINPUT_STATE) );
    DWORD result = XInputGetState( userIndex, &controllerState );
    if ( result != ERROR_SUCCESS ) {
        d = DISCONNECTED;
        return d;
    }

    // XINPUT Directions are stored in the least significant byte
    // BITMASK: 0001 is UP, 0010 is DOWN, 0100 is LEFT, 1000 is RIGHT
    switch( controllerState.Gamepad.wButtons & 0x0F ) {
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
            d = isP1 ? UP_FORWARD : UP_BACK;
            break;
        
        // 0101
        case 0x5:
            d = isP1 ? UP_BACK : UP_FORWARD;
            break;
        
        // 0100
        case 0x4:
            d = isP1 ? BACK : FORWARD;
            break;
        
        // 1000
        case 0x8:
            d = isP1 ? FORWARD : BACK;
            break;
        
        // 0010
        case 0x2:
            d = DOWN;
            break;
        
        // 1010
        case 0xA:
            d = isP1 ? DOWN_FORWARD : DOWN_BACK;
            break;
        
        // 0110
        case 0x6:
            d = isP1 ? DOWN_BACK : DOWN_FORWARD;
            break;
        
        default:
            d = UNKNOWN; 
    }

    return d;
}

DWORD GetControllerIndex() {
    DWORD userIndex = XUSER_MAX_COUNT;
    DWORD dwResult;

    printf("Please press START on controller\n");
    while( userIndex == XUSER_MAX_COUNT ) {
        for(DWORD i = 0; i < XUSER_MAX_COUNT; i++) {
            ZeroMemory( &controllerState, sizeof(XINPUT_STATE) );
            dwResult = XInputGetState( i, &controllerState );

            if( dwResult == ERROR_SUCCESS && (controllerState.Gamepad.wButtons & 0x10) != 0 ) {
                userIndex = i;
                break;
            }
        }
    }

    return userIndex;
}