#include "game.h"
#include "input.h"

GameDirection prevInput = NEUTRAL;

void Init_Game()
{
    gamestate.player_pos = 0;
}

void Update(GameDirection input)
{
    // No update if input has not changed
    if (input == prevInput) return;

    if (input == kbdPattern[ gamestate.player_pos % 4 ])
    {
        if (gamestate.player_pos < KBD_INPUT_COUNT - 1)
        {
            gamestate.player_pos++;
            prevInput = input;
        } else
        {
            // Finished the whole input segment
            gamestate.player_pos = 0;
            prevInput = NEUTRAL;
        }
    } else
    {
        // Incorrect input
        // return player to start for now
        printf("Wrong input: %d\n", input);
        gamestate.player_pos = 0;
        prevInput = NEUTRAL;
    }
}