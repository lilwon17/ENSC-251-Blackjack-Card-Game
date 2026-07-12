#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include "hand.hpp"
#include "deck.hpp"
using namespace std;

class Player {
private:
    Hand hand;          // The player's current hand
    bool hasStood;       // Whether the player has chosen to stand

public:
    // Constructor
    Player();
    // Postcondition:
    // Creates a player with an empty hand and hasStood set to false

    // Destructor
    ~Player();

    // Member Functions
    // Add a card dealt from the deck to the player's hand
    void receiveCard(Card c);
    // Precondition:
    // Card c must be valid
    // Postcondition:
    // Adds card c to the player's hand

    // Return the player's hand
    Hand getHand() const;
    // Postcondition:
    // Returns a copy of the player's current hand

    // Check if the player has busted
    bool isBust() const;
    // Precondition:
    // Player's hand must contain at least one card
    // Postcondition:
    // Returns true if the player's hand value is greater than 21

    // Ask the player whether they want to hit or stand
    char chooseAction();
    // Precondition:
    // Player must be able to receive input from the keyboard
    // Postcondition:
    // Prompts the user for 'h' (hit) or 's' (stand), reprompts on invalid
    // input, sets hasStood to true if the player chooses to stand,
    // and returns the chosen action

    // Check if the player has chosen to stand
    bool getHasStood() const;
    // Postcondition:
    // Returns hasStood

    // Reset the player's hand for a new round
    void clearHand();
    // Postcondition:
    // Clears the player's hand and sets hasStood back to false
};

#endif
