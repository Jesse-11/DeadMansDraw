#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "Types.h"

//Forward declarations to avoid circular dependencies
class Game;

class Player {
private:
    std::string _name;
    CardCollection _playArea;
    CardCollection _bank;

    //-1 means no anchor, otherwise this stores the index of anchor within the play area
    //Note: did try this a bool but caused more issues with working out valid cards (hence why -1 is no anchor)
    int _anchorIndex;

public:
    Player();
    ~Player();

    //Base player actions
    bool playCard(Game& game, Card* card);
    void bankCards(Game& game);
    void bust(Game& game);

    //Printing and terminal output
    int getAndCalculateScore() const;
    void printPlayerPlayArea() const;
    void printPlayerBank() const;

    //Getters
    const std::string& getPlayerName() const { 
        return _name; 
    }
    CardCollection& getPlayerBank() { 
        return _bank; 
    }

    //Helper functions
    void setAnchorIndex(int newAnchorIndex) { 
        _anchorIndex = newAnchorIndex; 
    }
    size_t getPlayAreaSize() const { 
        return _playArea.size(); 
    }
};


#endif // PLAYER_H
