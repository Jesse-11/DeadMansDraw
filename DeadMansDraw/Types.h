#ifndef TYPES_H
#define TYPES_H

#include <vector>

// Card types as specified in the assignment spec (Anchor included for bonus marks)
enum class CardType {
	Cannon, Chest, Key, Anchor, Kraken, Map, Oracle, Mermaid, Hook, Sword
};

//Forward declaration to avoid circular dependencies
class Card;

//Recommend in assignment spec.
typedef std::vector<Card*> CardCollection;

#endif // TYPES_H
