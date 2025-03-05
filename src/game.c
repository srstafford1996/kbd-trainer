#include "game.h"
#include "input.h"

GameDirection prevInput = NEUTRAL;

void Init_Game()
{
    gamestate.player_pos = 0;
    gamestate.score = 0;
    gamestate.highscore = 0;
    gamestate.failed = false;
}

void Update(GameDirection input)
{
    // Reset failed flag
    if (gamestate.failed)
        gamestate.failed = false;

    // No update if input has not changed
    if (input == prevInput) return;

    if (input == kbdPattern[ gamestate.player_pos % 4 ])
    {
        // Correct input
        gamestate.score += 50;
        if (gamestate.score > gamestate.highscore)
            gamestate.highscore = gamestate.score;
        
       gamestate.player_pos = gamestate.player_pos == 3 ? 0 : gamestate.player_pos + 1;
       prevInput = input;
    } else
    {
        // Incorrect input
        // return player to start for now
        gamestate.player_pos = 0;
        gamestate.score = 0;

        gamestate.failed = true;
        gamestate.failed_input = input;

        prevInput = NEUTRAL;
    }
}