#ifndef DEALER_HPP
#define DEALER_HPP

#include <iostream>
#include "Hand.hpp"
#include "Deck.hpp"
using namespace std;

class Dealer{

private:
    Hand hand; // The dealer's hand
    bool hiddenCard; // Tracks if the hidden card in the dealer's hand is shown

public:
    
    // Constrcutor
    Dealer();
    // Postcondition:
    // Creates a dealer with an empty hand and hiddenCard set to false

    // Destrcutor
    ~Dealer();

    // Member Functions

    // Add a card to the dealer's hand
    void receiveCard(Card c);
    // Precondition:
    // Card c must be valid
    // Postcondition:
    // Adds Card c to the dealer's hand

    // Return the dealer's hand
    Hand getHand() const;
    // Postcondition:
    // Returns the dealer's current hand

    // Return the dealer's current hand value
    int getHandValue() const;
    // Precondition:
    // Dealer's hand must have at least one card
    // Hand value must also be greater than 0
    // Postcondition:
    // Returns the dealer's hand's Blackjack value

    // Check if the dedaler's hand is a bust
    bool isBust() const;
    // Precondition:
    // Dealer's hand must contain at least one card
    // Hand value must also be greater than 0
    // Postcondition:
    // Returns true if the dealer's hand value is greater than 21 and false if otherwise

    // Figure out if the dealer needs to hit
    bool shouldHit() const;
    // Precondtion:
    // Dealer's hand must contain at least one card
    // Hand value must also be greater than 0
    // Postcondition:
    // Returns true if the dealer's hand value is less than 17, and false if otherwise

    // Playing the dealer's turn
    void playTurn(Deck& deck);
    // Precondition:
    // The deck must not be empty
    // The player has already played their turn
    // Postcondition:
    // Reveal the dealer's hidden card
    // Then the dealer draws until its hand value is 17 or higher or until the dealer busts

    // Displaying the dealer's hand with a hidden card
    void displayHiddenHand() const;
    // Precondition:
    // Dealer must have two cards in their hand
    // Postcondition:
    // Displays only one of the cards in the dealer's hand while the other remains hidden

    // Displaying both cards in the dealer's hand
    void displayFullHand() const;
    // Precondition:
    // Dealer must have two cards in their hand
    // Postcondition:
    // Displays all cards in the dealer's hand and the total hand value

    // Reveal the dealer's hidden card
    void revealHiddenCard();
    // Postcondition:
    // Sets the hiddenCardShow value to true

    // Check if the hidden card has been revealed
    bool isHiddenCardShown() const;
    // Postcondition:
    // Returns hiddenCardShown

    // Reset the dealer's hand for a new round
    void clearHand();
    // Postcondition:
    // Clears the dealer's hand and sets hiddenCardShown to false

};


#endif