#include "CardAbilities.h"
#include "Game.h"
#include "Player.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

// Helper functions

/// <summary>
/// Helper function to return a valid players choice based on avaliable options (mainly card abilities)
/// </summary>
/// <param name="numberOfPlayerChoices"></param>
/// <returns>An int referencing an indexed option pasted to the terminal</returns>
int getCurrentPlayerChoice(int numberOfPlayerChoice) {
    int playerChoice = 0;
    
    std::cout << "Which card do you want to pick? ";
    std::cin >> playerChoice;

    while (playerChoice < 1 || playerChoice > numberOfPlayerChoice) {
        std::cout << "Invalid choice. Which card do you pick? ";
        std::cin >> playerChoice;
    }
    return playerChoice;
}

/// <summary>
/// Helper function to return the highest value card of each suit from a players bank
/// </summary>
/// <param name="bank"></param>
/// <returns>Returns a vector of each hyighest values card of each suit</returns>
std::vector<Card*> getBestCardOfEachSuitFromBank(const CardCollection& bank) {
    std::map<CardType, Card*> bestCardOfEachSuitMap;
    for (Card* card : bank) {
        CardType cardType = card->getCardType();
        if (bestCardOfEachSuitMap.find(cardType) == bestCardOfEachSuitMap.end() || card->getCardValue() > bestCardOfEachSuitMap[cardType]->getCardValue()) {
            bestCardOfEachSuitMap[cardType] = card;
        }
    }

    std::vector<Card*> bestCards;
    for (const auto& pair : bestCardOfEachSuitMap) {
        bestCards.push_back(pair.second);
    }
    return bestCards;
}

/// <summary>
/// Helper fucntions to remove a card from any collection
/// </summary>
/// <param name="collection"></param>
/// <param name="cardToRemove"></param>
void removeCardFromCollection(CardCollection& collection, Card* cardToRemove) {
    auto it = std::find(collection.begin(), collection.end(), cardToRemove);
    if (it != collection.end()) {
        collection.erase(it);
    }
}


//Implementation of card abilities


//Mermaid
MermaidCard::MermaidCard(int value) : Card(CardType::Mermaid, value) {}
std::string MermaidCard::str() const {
    return "Mermaid(" + std::to_string(value) + ")";
}

void MermaidCard::play(Game& game, Player& player) {
    std::cout << "No effect but Mermaids are with more. \n";
}



//Oracle
OracleCard::OracleCard(int value) : Card(CardType::Oracle, value) {}
std::string OracleCard::str() const {
    return "Oracle(" + std::to_string(value) + ")";
}

void OracleCard::play(Game& game, Player& player) {
    CardCollection& gameDeck = game.getGameDeck();

    if (!gameDeck.empty()) {
        Card* topCard = gameDeck.back();
        std::cout << "The Oracle sees a " << topCard->str() << "\n";
    }
    else {
        std::cout << "The deck is empty. No card to peek at.\n";
    }
}


//Kraken
KrakenCard::KrakenCard(int value) : Card(CardType::Kraken, value) {}
std::string KrakenCard::str() const {
    return "Kraken(" + std::to_string(value) + ")";
}

void KrakenCard::play(Game& game, Player& player) {
    std::cout << "Drawing 3 cards from the deck and playing each:\n";
    CardCollection& gameDeck = game.getGameDeck();

    for (int i = 0; i < 3; i++) {
        if (!gameDeck.empty()) {
            Card* drawnCard = gameDeck.back();
            gameDeck.pop_back();
            std::cout << player.getPlayerName() << " draws a " << drawnCard->str() << "\n";
            bool busted = player.playCard(game, drawnCard);
            if (busted) {
                player.bust(game);
                break;
            }
        }
        else {
            std::cout << "The deck is empty. No more cards to draw.\n";
            break;
        }
    }
}


//Cannon
CannonCard::CannonCard(int value) : Card(CardType::Cannon, value) {}
std::string CannonCard::str() const {
    return "Cannon(" + std::to_string(value) + ")";
}

void CannonCard::play(Game& game, Player& player) {
    Player* otherPlayer = game.getTheOtherPlayer(&player);
    CardCollection& otherPlayerBank = otherPlayer->getPlayerBank();

    if (otherPlayerBank.empty()) {
        std::cout << "The cards in the other playes Bank, Continuing.\n";
        return;
    }

    std::vector<Card*> bestCards = getBestCardOfEachSuitFromBank(otherPlayerBank);
    std::cout << "Discarding the top cards of each suit from other players bank: \n";

    for (size_t i = 0; i < bestCards.size(); i++) {
        std::cout << "(" << i + 1 << ")" << bestCards[i]->str() << "\n";
    }

    int playerChoice = getCurrentPlayerChoice(bestCards.size());
    Card* cardToDiscard = bestCards[playerChoice - 1];

    removeCardFromCollection(otherPlayerBank, cardToDiscard);
    game.getDiscardPile().push_back(cardToDiscard);
}


//Sword
SwordCard::SwordCard(int value) : Card(CardType::Sword, value) {}
std::string SwordCard::str() const {
    return "Sword(" + std::to_string(value) + ")";
}

void SwordCard::play(Game& game, Player& player) {
    Player* otherPlayer = game.getTheOtherPlayer(&player);
    CardCollection& otherPlayerBank = otherPlayer->getPlayerBank();

    if (otherPlayerBank.empty()) {
        std::cout << "No cards in other players bank, Continuing.\n";
        return;
    }

    std::vector<Card*> bestCards = getBestCardOfEachSuitFromBank(otherPlayerBank);
    std::cout << "Steal the top cards of each suit from other players bank: \n";

    for (size_t i = 0; i < bestCards.size(); i++) {
        std::cout << "(" << i + 1 << ")" << bestCards[i]->str() << "\n";
    }

    int playerChoice = getCurrentPlayerChoice(bestCards.size());
    Card* cardToSteal = bestCards[playerChoice - 1];

    removeCardFromCollection(otherPlayerBank, cardToSteal);

    std::cout << player.getPlayerName() << " steals " << cardToSteal->str() << " from " << otherPlayer->getPlayerName() << "'s bank.\n";
    player.playCard(game, cardToSteal);
}


//Hook
HookCard::HookCard(int value) : Card(CardType::Hook, value) {}
std::string HookCard::str() const {
    return "Hook(" + std::to_string(value) + ")";
}

void HookCard::play(Game& game, Player& player) {
    CardCollection& playerBank = player.getPlayerBank();

    if (playerBank.empty()) {
        std::cout << "No cards in your bank, Continuing.\n";
        return;
    }

    std::vector<Card*> bestCards = getBestCardOfEachSuitFromBank(playerBank);
    std::cout << "Select the highest-value card from any suit in your bank: \n";

    for (size_t i = 0; i < bestCards.size(); i++) {
        std::cout << "(" << i + 1 << ")" << bestCards[i]->str() << "\n";
    }

    int playerChoice = getCurrentPlayerChoice(bestCards.size());
    Card* cardToPlay = bestCards[playerChoice - 1];

    removeCardFromCollection(playerBank, cardToPlay);

    std::cout << player.getPlayerName() << " plays " << cardToPlay->str() << " from their bank.\n";
    player.playCard(game, cardToPlay);
}


//Map
MapCard::MapCard(int value) : Card(CardType::Map, value) {}
std::string MapCard::str() const {
    return "Map(" + std::to_string(value) + ")";
}

void MapCard::play(Game& game, Player& player) {
    CardCollection& discardPile = game.getDiscardPile();

    if (discardPile.empty()) {
        std::cout << "No cards in the discard pile. Continuing.\n";
        return;
    }

    std::cout << "Drawing up to 3 cards from the discard pile and pick one to add to play area.\n";
    std::vector<Card*> drawnCardOptions;
    int drawLimit = std::min(3, static_cast<int>(discardPile.size()));

    for (int i = 0; i < drawLimit; i++) {
        drawnCardOptions.push_back(discardPile.back());
        discardPile.pop_back();
        std::cout << "(" << i + 1 << ")" << drawnCardOptions.back()->str() << "\n";
    }

    int playerChoice = getCurrentPlayerChoice(drawnCardOptions.size());
    Card* cardToPlay = drawnCardOptions[playerChoice - 1];


    for (size_t i = 0; i < drawnCardOptions.size(); i++) {
        if (i != static_cast<size_t>(playerChoice - 1)) {
            discardPile.push_back(drawnCardOptions[i]);
        }
    }

    std::cout << player.getPlayerName() << " plays " << cardToPlay->str() << " from the discard pile.\n";
    player.playCard(game, cardToPlay);
}

//Chest & Key
ChestCard::ChestCard(int value) : Card(CardType::Chest, value) {}
std::string ChestCard::str() const {
    return "Chest(" + std::to_string(value) + ")";
}

void ChestCard::play(Game& game, Player& player) {
    std::cout << "No immediate effect but if banked with a Key card, draw as many bonus cards from the Discard pile as you moved into your Bank.\n";
}

void ChestCard::chestAndKey(Game& game, Player& player) {
    bool hasKeyCard = false;

    for (Card* card : player.getPlayerBank()) {
        if (card->getCardType() == CardType::Key) {
            hasKeyCard = true;
            break;
        }
    }
    
    if (hasKeyCard) {
        CardCollection& discardPile = game.getDiscardPile();
        int bonusCardsToDraw = player.getPlayAreaSize();

        if (bonusCardsToDraw > 0 && !discardPile.empty()) {
            std::cout << "Chest and Key activated. Added: \n";

            for (int i = 0; i < bonusCardsToDraw; i++) {
                if (discardPile.empty()) {
                    break;
                } else {
                    Card* bonusCard = discardPile.back();
                    discardPile.pop_back();

                    std::cout << "Bonus card drawn: " << bonusCard->str() << "\n";
                    player.getPlayerBank().push_back(bonusCard);
                }
                std::cout << "to your bank.\n";
            }
        }
    }
}

KeyCard::KeyCard(int value) : Card(CardType::Key, value) {}
std::string KeyCard::str() const {
    return "Key(" + std::to_string(value) + ")";
}

void KeyCard::play(Game& game, Player& player) {
    std::cout << "No immediate effect but if banked with a Chest card, draw as many bonus cards from the Discard pile as you moved into your Bank.\n";
}

void KeyCard::chestAndKey(Game& game, Player& player) {
    // Implemnted for both within the Chest logic. 
    // Doing it here as it would cause double logic issues.
}


//Anchor
AnchorCard::AnchorCard(int value) : Card(CardType::Anchor, value) {}
std::string AnchorCard::str() const {
    return "Anchor(" + std::to_string(value) + ")";
}

void AnchorCard::play(Game& game, Player& player) {
    std::cout << "Anchor card played. Previous cards are safe if you bust.\n";
    player.setAnchorIndex(player.getPlayAreaSize() - 1);
}
