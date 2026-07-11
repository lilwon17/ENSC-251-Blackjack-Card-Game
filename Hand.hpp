#ifndef HAND_HPP
#define HAND_HPP

#include <iostream>
#include "card.hpp"
using namespace std;

const int MAX_HAND_SIZE = 12;  // Enough for worst case hand without busting, worst case: A+A+A+A+2+2+2+2+3+3+3, one more space for buffer

class Hand {
private:
    // Properties of a hand
    Card cards[MAX_HAND_SIZE]; // Array holding the cards currently in hand
    int numCards;              // Number of cards currently in the hand

public:
    // Constructor
    Hand();
    // Postcondition:
    // Creates an empty hand with numCards set to 0

    // Destructor
    ~Hand();

    // Member Functions
    // Add a card to the hand
    void addCard(Card c);
    // Precondition:
    // numCards must be less than MAX_HAND_SIZE
    // Postcondition:
    // Adds card c to the hand and increases numCards by 1

    // Calculate the total Blackjack value of the hand
    int getValue() const;
    // Precondition:
    // Hand must contain at least one card
    // Postcondition:
    // Returns the best possible hand value, counting each Ace as 11
    // then reducing individual Aces to 1 as needed to avoid busting

    // Check if the hand exceeds 21
    bool isBust() const;
    // Precondition:
    // Hand must contain at least one card
    // Postcondition:
    // Returns true if getValue() is greater than 21, false otherwise

    // Return the number of cards in hand
    int getNumCards() const;
    // Postcondition:
    // Returns numCards

    // Empty the hand for a new round
    void clear();
    // Postcondition:
    // Removes all cards from the hand and sets numCards to 0

    // Display all cards in the hand
    void display() const;
    // Precondition:
    // Hand must be valid
    // Postcondition:
    // Prints every card in the hand along with the total hand value

    // Display only the first card in the hand
    void displayPartial() const;
    // Precondition:
    // Hand must contain at least one card
    // Postcondition:
    // Prints only the first card in the hand (used for the dealer's
    // hidden hand) followed by a placeholder for the hidden card(s)

    // Overload Insertion Operator
    friend ostream& operator<<(ostream& os, const Hand& h);
    // Precondition:
    // Hand must be valid
    // Postcondition:
    // Displays all cards in the hand and the total hand value
};

#endif
