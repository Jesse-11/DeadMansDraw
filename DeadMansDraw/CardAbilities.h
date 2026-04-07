#ifndef CARDABILITIES_H
#define CARDABILITIES_H

#include "Card.h"

//1
//Canon: Discard the top card (i.e. the highest value) of any suit from the other player’s Bank to the Discard Pile [2,3,4,5,6,7]
class CannonCard : public Card {
public:
    CannonCard(int value);
    std::string str() const override;
    void play(Game& game, Player& player) override;
};

//2/3
//Chest/Key: No immediate effect. If banked with a Key card, draw as many bonus cards from the Discard pile as you moved into your Bank [2,3,4,5,6,7]
class ChestCard : public Card {
public:
    ChestCard(int value);
    std::string str() const override;
    void play(Game& game, Player& player) override;
    void chestAndKey(Game& game, Player& player) override;
};

//2/3
//Chest/Key: No immediate effect. If banked with a Key card, draw as many bonus cards from the Discard pile as you moved into your Bank [2,3,4,5,6,7]
class KeyCard : public Card {
public:
    KeyCard(int value);
    std::string str() const override;
    void play(Game& game, Player& player) override;
    void chestAndKey(Game& game, Player& player) override;
};

//4
//Sword: Steal the top card (i.e. the highest value) of any suit fromthe other player’s Bank into your Play Area.You must selectone card [2,3,4,5,6,7]
class SwordCard : public Card {
public:
    SwordCard(int value);
    std::string str() const override;
    void play(Game& game, Player& player) override;
};

//5
//Hook: Play the top card (i.e. the highest value) of any suit fromyour Bank into your play area.You must select one card [2,3,4,5,6,7]
class HookCard : public Card {
public:
    HookCard(int value);
    std::string str() const override;
    void play(Game& game, Player& player) override;
};

//6
//Oracle: Peek at the top card of the deck before choosing whether to draw [2,3,4,5,6,7]
class OracleCard : public Card {
public:
    OracleCard(int value);
    std::string str() const override;
    void play(Game& game, Player& player) override;
};

//7
//Map: Draw 3 cards from discard pile. You must play one of the cards drawn into your play area [2,3,4,5,6,7]
class MapCard : public Card {
public:
    MapCard(int value);
    std::string str() const override;
    void play(Game& game, Player& player) override;
};

//8
//Mermaid: No ability but the cards have a higher point value [4,5,6,7,8,9]
class MermaidCard : public Card {
public:
    MermaidCard(int value);
    std::string str() const override;
    void play(Game& game, Player& player) override;
};

//9
//Kraken: Must draw and play three cards consecutively [2,3,4,5,6,7]
class KrakenCard : public Card {
public:
    KrakenCard(int value);
    std::string str() const override;
    void play(Game& game, Player& player) override;
};

//10
//Anchor: Any cards draw before the Anchor card are safe and moved to the players bank, even if the player busts [2,3,4,5,6,7]
class AnchorCard : public Card {
public:
    AnchorCard(int value);
    std::string str() const override;
    void play(Game& game, Player& player) override;
};


#endif // CARDABILITIES_H