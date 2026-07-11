#ifndef DECK_HPP
#define DECK_HPP

#include <iostream>
#include "card.hpp"
using namespace std;

const int DECK_SIZE = 52;

class Deck {
private:
    // Properties of the deck
    Card cards[DECK_SIZE];  // Array holds all cards in the deck
    int numCards;           // Number of cards currently remains in the deck
    int topIndex;           // Index of the next card to be dealt

public:
    // Constructor
    Deck();
    // Postcondition:
    // Creates a standard 52 card deck (all ranks 1-13, all suits 0-3)
    // and sets topIndex to 0, numCards to 52

    // Destructor
    ~Deck();

    // Member Functions
    // Shuffle the deck
    void shuffle();
    // Precondition:
    // Deck must contain valid cards
    // Postcondition:
    // Randomly reorders the cards in the deck and resets topIndex to 0

    // Deal one card from the top of the deck
    Card dealCard();
    // Precondition:
    // Deck must not be empty (numCards > 0)
    // Postcondition:
    // Returns the card at topIndex, increments topIndex, decreases numCards by 1

    // Return how many cards are left in the deck
    int cardsRemaining() const;
    // Postcondition:
    // Returns numCards

    // Check if the deck has no cards left
    bool isEmpty() const;
    // Postcondition:
    // Returns true if numCards is 0, false otherwise

    // Reset and reshuffle the deck for a new round
    void reset();
    // Postcondition:
    // Refills the deck with all 52 cards, sets numCards to 52,
    // topIndex to 0, then shuffles

    // Overload Insertion Operator
    friend ostream& operator<<(ostream& os, const Deck& d);
    // Precondition:
    // Deck must be valid
    // Postcondition:
    // Displays the number of cards remaining in the deck
};

#endif
