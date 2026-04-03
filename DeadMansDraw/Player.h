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

    bool _hasAnchor;

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
    void setHasAnchor(bool hasAnchorNew) { _hasAnchor = hasAnchorNew; }
    size_t getPlayAreaSize() const { return _playArea.size(); }
};


#endif // PLAYER_H
