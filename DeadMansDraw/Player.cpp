#include "Player.h"
#include "Game.h"
#include "Card.h"

#include <iostream>
#include <map>
#include <cstdlib>
#include <algorithm>

//Helper functions
void printCardCollection(const CardCollection& collection, const std::string collectionTitle) {
    std::cout << collectionTitle << "\n";

    if (collection.empty()) {
        return;
    }

    std::map<CardType, std::vector<Card*>> groupedCards;
    for (Card* card : collection) {
        groupedCards[card->getCardType()].push_back(card);
    }

    for (auto& pair : groupedCards) {
        // Sort descending by value
        std::sort(pair.second.begin(), pair.second.end(), [](Card* firstCard, Card* secondCard) {
            return firstCard->getCardValue() > secondCard->getCardValue();
            });

        for (Card* card : pair.second) {
            std::cout << card->str() << " ";
        }
        std::cout << "\n";
    }
}



Player::Player() : _anchorIndex(-1) {

    std::string names[] = { "Sam", "Billy", "Jen", "Bob", "Sally", "Joe", "Sue", "Sasha", "Tina", "Marge" };

    _name = names[rand() % 10];
}

Player::~Player() {
    for (size_t i = 0; i < _playArea.size(); i++) {
        delete _playArea[i];
    }
    for (size_t i = 0; i < _bank.size(); i++) {
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
    // If cards drawn dont cuase bust, add and play ability
    _playArea.push_back(card);
    card->play(game, *this);
    return false;
}

void Player::bust(Game& game) {
    std::cout << "BUST! " << getPlayerName() << " loses all cards in play area.\n";
    CardCollection& discardPile = game.getDiscardPile();

    //This is my attempt at handling the anchor logic for bonus marks
    int anchorIndexForDiscarding = 0;
    if (_anchorIndex != -1) {
        anchorIndexForDiscarding = _anchorIndex + 1;
    }

    //Bank safe cards from before the anchor, then discard the rest
    for (int i = 0; i < anchorIndexForDiscarding; i++) {
        _bank.push_back(_playArea[i]);
    }
    for (size_t i = anchorIndexForDiscarding; i < _playArea.size(); i++) {
        discardPile.push_back(_playArea[i]);
    }

    //reset for next turn
    _playArea.clear();
    _anchorIndex = -1;
}

void Player::bankCards(Game& game) {
    std::cout << getPlayerName() << " banks the cards in their play area.\n";

    for (Card* card : _playArea) {
        card->chestAndKey(game, *this);
        _bank.push_back(card);
    }
    _playArea.clear();
    _anchorIndex = -1;
}

int Player::getAndCalculateScore() const {

    std::map<CardType, int> scoredCardTypes;
    
    for (Card* card: _bank) {
        CardType currentCardType = card->getCardType();
        int currentCardValue = card->getCardValue();

        // if suit not in map add, otherwise if card value is higher update score
        if (scoredCardTypes.find(currentCardType) == scoredCardTypes.end() || scoredCardTypes[currentCardType] < currentCardValue) {
            scoredCardTypes[currentCardType] = currentCardValue;
        }
    }

    int totalScore = 0;
    for (auto const& cardPair : scoredCardTypes) {
        //.second is the vlaue of the card
        totalScore += cardPair.second;
    }
    return totalScore;
}

void Player::printPlayerPlayArea() const {
    printCardCollection(_playArea, getPlayerName() + "'s play area: ");
}

void Player::printPlayerBank() const {
    printCardCollection(_bank, getPlayerName() + "'s bank: ");
}