#include "Card.hpp"
using namespace std;

// Default constructor
// Makes the Ace of Clubs
Card::Card() {
    rank = 1;
    suit = 0;
}

// Constructor with parameters
// Makes a card with the given rank and suit
Card::Card(int r, int s) {
    rank = r;
    suit = s;
}

// Destructor
// Nothing to do here
Card::~Card() {
}

// Gives back the rank
int Card::getRank() const {
    return rank;
}

// Gives back the suit
int Card::getSuit() const {
    return suit;
}

// Gives back how many points the card is worth
int Card::getValue() const {
    // Face cards are worth 10
    if (isFaceCard()) {
        return 10;
    }
    // Ace is worth 11 for now
    if (isAce()) {
        return 11;
    }
    // Any other card is just worth its rank
    return rank;
}

// Checks if the card is a Jack, Queen, or King
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

// Checks if the card's rank and suit make sense
bool Card::isValid() const {
    // Rank has to be 1 through 13
    if (rank < 1 || rank > 13) {
        cout << "Error: invalid rank." << endl;
        return false;
    }
    // Suit has to be 0 through 3
    if (suit < 0 || suit > 3) {
        cout << "Error: invalid suit." << endl;
        return false;
    }
    // If we made it here, the card is fine
    return true;
}

// Checks if two cards have the same rank
bool Card::operator==(const Card& other) const {
    return rank == other.rank;
}

// Checks if this card's rank is smaller
bool Card::operator<(const Card& other) const {
    return rank < other.rank;
}

// Checks if this card's rank is bigger
bool Card::operator>(const Card& other) const {
    return rank > other.rank;
}

// Prints the card like "King of Spades"
ostream& operator<<(ostream& out, const Card& c) {
    // Turn the rank number into a word
    string rankName;
    if (c.rank == 1) {
        rankName = "Ace";
    } else if (c.rank == 11) {
        rankName = "Jack";
    } else if (c.rank == 12) {
        rankName = "Queen";
    } else if (c.rank == 13) {
        rankName = "King";
    } else {
        // For 2 through 10, just print the number
        rankName = to_string(c.rank);
    }

    // Turn the suit number into a word
    string suitName;
    if (c.suit == 0) {
        suitName = "Clubs";
    } else if (c.suit == 1) {
        suitName = "Diamonds";
    } else if (c.suit == 2) {
        suitName = "Hearts";
    } else if (c.suit == 3) {
        suitName = "Spades";
    } else {
        suitName = "Unknown";
    }

    out << rankName << " of " << suitName;
    return out;
}

// Reads in a rank and a suit and puts them into the card
istream& operator>>(istream& is, Card& c) {
    is >> c.rank >> c.suit;
    return is;
}
