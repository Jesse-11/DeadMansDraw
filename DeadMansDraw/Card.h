#ifndef CARD_H
#define CARD_H

#include <string>
#include "Types.h"

class Game;
class Player;

class Card {
protected:
    CardType type;
    int value;

public:
    Card(CardType cardType, int cardValue) : type(cardType), value(cardValue) {}
    virtual ~Card() {}

    const CardType& getCardType() const { 
        return type; 
    }

    int getCardValue() const { 
        return value; 
    }

    virtual std::string str() const = 0; 

    virtual void play(Game& game, Player& player) = 0;

    virtual void chestAndKey(Game& game, Player& player) {}

};



#endif //CARD_H
