#ifndef TYPES_H
#define TYPES_H

#include <vector>

enum class CardType {
	Cannon, Chest, Key, Anchor, Kraken, Map, Oracle, Mermaid, Hook, Sword
};

class Card;

//Recommend in assignment spec.
typedef std::vector<Card*> CardCollection;

#endif // TYPES_H
