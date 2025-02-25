#include <Xinput.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "input.h"


int main() {
    system("cls");
    DWORD userIndex = GetControllerIndex();

    GameDirection currDirection;
    GameDirection lastDirection = NEUTRAL;
    while(true) {
        currDirection = GetInput(userIndex);
        if(currDirection != lastDirection) {
            printf(" %s ", DirectionNames[currDirection]);
            lastDirection = currDirection;
        }
    }

    return 0;
}