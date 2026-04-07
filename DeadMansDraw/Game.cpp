#include "Game.h"
#include "CardAbilities.h"
#include "game_title.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

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
/// Get a reference to the other player
/// </summary>
/// <param name="currentPlayer"></param>
/// <returns>Returns a pointed to the other players index</returns>
Player* Game::getTheOtherPlayer(const Player* currentPlayer) const {
    if (currentPlayer == _gamePlayers[0]) {
        return _gamePlayers[1];
    }
    else {
        return _gamePlayers[0];
    }
}

/// <summary>
/// Initializes the game deck by creating and adding all card types with their respective values.
/// </summary>
void Game::initializeDeck() {
    int cardValues[] = { 2,3,4,5,6,7 };
    int mermaidCardValues[] = { 4,5,6,7,8,9 };

    for ( int i = 0; i < 6; i++) {
        _gameDeck.push_back(new CannonCard(cardValues[i]));
        _gameDeck.push_back(new ChestCard(cardValues[i]));
        _gameDeck.push_back(new KeyCard(cardValues[i]));
        _gameDeck.push_back(new SwordCard(cardValues[i]));
        _gameDeck.push_back(new HookCard(cardValues[i]));
        _gameDeck.push_back(new OracleCard(cardValues[i]));
        _gameDeck.push_back(new KrakenCard(mermaidCardValues[i]));

        //Bonus anchor
        _gameDeck.push_back(new AnchorCard(cardValues[i]));

        //Mermaid cards
        _gameDeck.push_back(new MermaidCard(mermaidCardValues[i]));
    }
}

/// <summary>
/// Randomly shuffles the game deck using the Mersenne Twister algorithm. Algorithm provided by spec sheet. Not seeded
/// </summary>
void Game::shuffleDeck() {
    CardCollection shuffleDeck{ _gameDeck.begin(), _gameDeck.end() };

    //Logic to generate random based on spec sheet
    std::random_device random;
    std::mt19937 generator(random());

    std::shuffle(shuffleDeck.begin(), shuffleDeck.end(), generator);
    std::copy(shuffleDeck.begin(), shuffleDeck.end(), _gameDeck.begin());
}

/// <summary>
/// Starts and runs the main game loop, then determines and announces the winner.
/// </summary>
void Game::startGame() {
    //Run game title
    std::cout << GAME_TITLE << "\n";

    //Main loop
    while (!_gameDeck.empty() && -_currentRound <= 20) {
        nextTurn();
    }

    std::cout << "Game over! \n";

    if (_currentRound > 20) {
        std::cout << "Reached maximum rounds. Ending game.\n";
    }

    _gamePlayers[0]->printPlayerBank();
    _gamePlayers[1]->printPlayerBank();

    int player1Score = _gamePlayers[0]->getAndCalculateScore();
    int player2Score = _gamePlayers[1]->getAndCalculateScore();

    if (player1Score > player2Score) {
        std::cout << _gamePlayers[0]->getPlayerName() << " wins with a score of " << player1Score << "!\n";
    }
    else if (player2Score > player1Score) {
        std::cout << _gamePlayers[1]->getPlayerName() << " wins with a score of " << player2Score << "!\n";
    }
    else {
        std::cout << "It's a tie with both players scoring " << player1Score << "!\n";
    }
}

/// <summary>
/// Executed the logic for a single players turn, rounds, and switching between players. 
/// Handles player input for whether to continue playing or bank, and checks for busts and end of game conditions.
/// </summary>
void Game::nextTurn() {
    Player* currentPlayer = _gamePlayers[_currentPlayerIndex];

    std::cout << "Round " << _currentRound << ", Turn " << _currentTurn << ": " << currentPlayer->getPlayerName() << "'s turn.\n";

    //Bank and current score
    currentPlayer->printPlayerBank();
    std::cout << "Current score: " << currentPlayer->getAndCalculateScore() << "\n";

    bool turnStillGoing = true;

    while (turnStillGoing && !_gameDeck.empty()) {
        Card* drawnCard = _gameDeck.back();
        _gameDeck.pop_back();

        std::cout << currentPlayer->getPlayerName() << " draws a " << drawnCard->str() << "\n";

        //Play drawn card and check if player busted
        bool busted = currentPlayer->playCard(*this, drawnCard);

        if (busted || currentPlayer->getPlayAreaSize() == 0) {
            //This was needed to fix issues of busting under kraken ability.
            if (busted) {
                currentPlayer->bust(*this);
            }
            turnStillGoing = false;
        }
        else {
            currentPlayer->printPlayerPlayArea();

            //Continue or bank
            char playerChoicePlayOrBank;
            std::cout << "Do you want to (y) play another card or (n) bank your current cards? ";
            std::cin >> playerChoicePlayOrBank;

            if (playerChoicePlayOrBank == 'n' || playerChoicePlayOrBank == 'N') {
                currentPlayer->bankCards(*this);
                turnStillGoing = false;
            }
        }
    }

    //Logic to bank cards when the deck runs out (had issues where play area on deck finish was not being added to score)
    if (_gameDeck.empty() && currentPlayer->getPlayAreaSize() > 0) {
        currentPlayer->bankCards(*this);
    }

    //Cleanup
    _currentTurn++;
    _currentPlayerIndex++;

    if (_currentPlayerIndex >= 2) {
        _currentPlayerIndex = 0;
        _currentRound++;
    }
    std::cout << "\n";
}