#include "Player.h"
#include "Game.h"
#include "Card.h"

#include <iostream>
#include <map>

Player::~Player() {
    for (auto i = 0; i < _playArea.size(); i++) {
        delete _playArea[i];
    }
    for (auto i = 0; i < _bank.size(); i++) {
        delete _bank[i];
    }
}


/// <summary>
/// Play a card and check if they busted. If not bust, add card and play the ability
/// </summary>
/// <param name="game"></param>
/// <param name="card"></param>
/// <returns>True if busted - False if safe</returns>
bool Player::playCard(Game& game, Card* card) {
    //Check if drawed card will bust
    for (Card* alreadyDrawnCards : _playArea) {
        if (alreadyDrawnCards->getCardType() == card->getCardType()) {
            //Busted - Added anyway so we can print the card that caused the bust
            _playArea.push_back(card);
            return true;
        }
    }

    _playArea.push_back(card);
    card->play(game, *this);
    return false;
}

void Player::bust(Game& game) {
    std::cout << "BUST! " << getPlayerName() << " loses all cards in play area.\n";
}