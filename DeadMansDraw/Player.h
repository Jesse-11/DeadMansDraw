#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "Types.h"

class Game;

class Player {
private:
    std::string _name;
    CardCollection _playArea;
    CardCollection _bank;

    // -1 means no anchor, otherwise stores the index of the card in play area that is the anchor
    int _anchorIndex;

public:
    Player();
    ~Player() {}

    //Base player actions
    bool playCard(Game& game, Card* card);
    void bankCards(Game& game);
    void bust(Game& game);

    //Printing and terminal output
    int getAndCalculateScore() const;
    void printPlayerPlayArea() const;
    void printPlayerBank() const;

    //Getters
    const std::string& getPlayerName() const { return _name; }
    CardCollection& getPlayerBank() { return _bank; }

    //Helper functions
    void setAnchorIndex(int newAnchorIndex) { _anchorIndex = newAnchorIndex; }
    size_t getPlayAreaSize() const { return _playArea.size(); }
};


#endif // PLAYER_H
