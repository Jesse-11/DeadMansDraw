#include "Player.h"
#include "Game.h"
#include "Card.h"

#include <iostream>
#include <map>
#include <cstdlib>
#include <algorithm>

//Helper functions

/// <summary>
/// Helper function to sort, format and print a collection of cards. 
/// </summary>
/// <param name="collection"></param>
/// <param name="collectionTitle"></param>
void printCardCollection(const CardCollection& collection, const std::string nameOfCollection) {
    std::cout << nameOfCollection << "\n";

    if (collection.empty()) {
        return;
    }

    std::map<CardType, std::vector<Card*>> pairsOfCardsInCollection;
    for (Card* card : collection) {
        pairsOfCardsInCollection[card->getCardType()].push_back(card);
    }

    for (auto& pair : pairsOfCardsInCollection) {
        // Sort descending by value
        std::sort(pair.second.begin(), pair.second.end(), [](Card* firstCard, Card* secondCard) 
            {
            return firstCard->getCardValue() > secondCard->getCardValue();
            }
        );

        for (Card* card : pair.second) {
            std::cout << card->str() << " ";
        }
        std::cout << "\n";
    }
}


// Randomly assigns name and set anchor to -1 which represents no anchor.
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

/// <summary>
/// Handles the games bust logic, discading cards and baking safe cards from anchor. Then resetting play area. 
/// </summary>
/// <param name="game"></param>
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

/// <summary>
/// Banks all the players cards in play area, checking for any chest and key to activate and clearing play area for next turn.
/// </summary>
/// <param name="game"></param>
void Player::bankCards(Game& game) {
    std::cout << getPlayerName() << " banks the cards in their play area.\n";

    for (Card* card : _playArea) {
        card->chestAndKey(game, *this);
        _bank.push_back(card);
    }
    _playArea.clear();
    _anchorIndex = -1;
}

/// <summary>
/// Calcualtes and returns the players scores based on the highest value card of each suit in their bank.
/// </summary>
/// <returns></returns>
int Player::getAndCalculateScore() const {

    std::map<CardType, int> scoredSuitAndValueMap;
    
    for (Card* card: _bank) {
        CardType currentCardType = card->getCardType();
        int currentCardValue = card->getCardValue();

        // if suit not in map add, otherwise if card value is higher update score
        if (scoredSuitAndValueMap.find(currentCardType) == scoredSuitAndValueMap.end() || scoredSuitAndValueMap[currentCardType] < currentCardValue) {
            scoredSuitAndValueMap[currentCardType] = currentCardValue;
        }
    }

    int totalPlayerScore = 0;
    for (auto const& cardPair : scoredSuitAndValueMap) {
        //.second is the vlaue of the card
        totalPlayerScore += cardPair.second;
    }
    return totalPlayerScore;
}

/// <summary>
/// Prints the cards in the player's play area to the console.
/// </summary>
void Player::printPlayerPlayArea() const {
    printCardCollection(_playArea, getPlayerName() + "'s play area: ");
}

/// <summary>
/// Prints the player's bank of cards to the output.
/// </summary>
void Player::printPlayerBank() const {
    printCardCollection(_bank, getPlayerName() + "'s bank: ");
}