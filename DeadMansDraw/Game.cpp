#include "Game.h"

#include <iostream>



Game::Game() : _currentRound(1), _currentTurn(1), _currentPlayerIndex(0) {
    _gamePlayers[0] = new Player();
    _gamePlayers[1] = new Player();

    initializeDeck();
    shuffleDeck();
}

Game::~Game() {
    delete _gamePlayers[0];
    delete _gamePlayers[1];

    for (Card* card : _gameDeck) {
        delete card;
    }
    for (Card* card : _discardPile) {
            delete card;
    }
}

/// <summary>
/// Returns a pointer to the other player. Used for card abilities that interact with the other player.
/// </summary>
/// <param name="currentPlayer"></param>
/// <returns></returns>
Player* Game::getTheOtherPlayer(const Player* currentPlayer) const {
    if (currentPlayer == _gamePlayers[0]) {
        return _gamePlayers[1];
    }
    else {
        return _gamePlayers[0];
    }
}