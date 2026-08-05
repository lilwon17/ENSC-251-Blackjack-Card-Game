#include "Card.hpp"
using namespace std;
 
// Default constructor
// Just makes a basic card: rank 1 (Ace), suit 0 (Clubs)
Card::Card() {
    rank = 1;
    suit = 0;
}
 
// Constructor with parameters
// Sets the rank and suit to whatever the user passed in
Card::Card(int r, int s) {
    rank = r;
    suit = s;
}
 
// Destructor
// Nothing special to clean up here since we only have plain ints
Card::~Card() {
}
 
// Returns the rank (1-13)
int Card::getRank() const {
    return rank;
}
 
// Returns the suit (0-3)
int Card::getSuit() const {
    return suit;
}
 
// Returns the Blackjack point value of the card
int Card::getValue() const {
    // Face cards (Jack, Queen, King) are all worth 10
    if (isFaceCard()) {
        return 10;
    }
    // Ace is worth 11 here; the Hand class can lower it to 1 later if needed
    if (isAce()) {
        return 11;
    }
    // Otherwise the card's value is just its rank (2-10)
    return rank;
}
 
// Checks if the card is a face card (Jack=11, Queen=12, King=13)
bool Card::isFaceCard() const {
    if (rank == 11 || rank == 12 || rank == 13) {
        return true;
    }
    return false;
}
 
// Checks if the card is an Ace
bool Card::isAce() const {
    if (rank == 1) {
        return true;
    }
    return false;
}
 
// Checks that the card's rank and suit are within valid ranges
bool Card::isValid() const {
    // Rank must be between 1 and 13
    if (rank < 1 || rank > 13) {
        cout << "Error: invalid rank." << endl;
        return false;
    }
    // Suit must be between 0 and 3
    if (suit < 0 || suit > 3) {
        cout << "Error: invalid suit." << endl;
        return false;
    }
    // If we get here, both rank and suit are okay
    return true;
}
 
// Compares two cards based on rank only
bool Card::operator==(const Card& other) const {
    return rank == other.rank;
}
 
bool Card::operator<(const Card& other) const {
    return rank < other.rank;
}
 
bool Card::operator>(const Card& other) const {
    return rank > other.rank;
}
 
// Prints the card like "King of Spades"
ostream& operator<<(ostream& out, const Card& c) {
    // Turn the numeric rank into a word
    string rankName;
    switch (c.rank) {
        case 1:  rankName = "Ace";   break;
        case 11: rankName = "Jack";  break;
        case 12: rankName = "Queen"; break;
        case 13: rankName = "King";  break;
        default: rankName = to_string(c.rank); break; // 2-10 just print the number
    }
 
    // Turn the numeric suit into a word
    string suitName;
    switch (c.suit) {
        case 0: suitName = "Clubs";    break;
        case 1: suitName = "Diamonds"; break;
        case 2: suitName = "Hearts";   break;
        case 3: suitName = "Spades";   break;
        default: suitName = "Unknown"; break;
    }
 
    out << rankName << " of " << suitName;
    return out;
}
 
// Reads a rank and suit from the input stream into the card
istream& operator>>(istream& is, Card& c) {
    is >> c.rank >> c.suit;
    return is;
}
 
