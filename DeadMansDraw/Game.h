#ifndef GAME_H
#define GAME_H

#include "Types.h"
#include "Player.h"
#include <stdio.h>

std::string names[] = { "Sam", "Billy", "Jen", "Bob", "Sally", "Joe", "Sue", "Sasha", "Tina", "Marge" };

class Game {
private:
    CardCollection _gameDeck;
    CardCollection _discardPile;

    Player* _gamePlayers[2];

    int _currentPlayerIndex;
    int _currentRound;
    int _currentTurn;

    void initializeDeck();
    void shuffleDeck();

public:
    Game();
    ~Game();

    void startGame();
    void nextTurn();

    //Getters to interact with players and cards
    CardCollection& getDiscardPile() { return _discardPile; }
    CardCollection& getGameDeck() { return _gameDeck; }
    Player* getTheOtherPlayer(const Player* currentPlayer) const;
};
#endif // GAME_H
