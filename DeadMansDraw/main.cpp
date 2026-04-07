// DeadMansDraw.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Game.h"
#include <iostream>

int main()
{
    std::cout << "Initializing Dead Man's Draw\n";
    Game game;
    game.startGame();

    //AI generated - issue where console was closing before I could see the final output, so added this to pause the console until user input.
    std::cin.clear();
    std::cout << "\nPlease Enter to exit.";
    std::cin.ignore(10000, '\n');
    std::cin.get();
    return 0;
}

