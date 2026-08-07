#include "Dealer.hpp"

// Constructor
Dealer::Dealer(){

    Hand hand; // Initialize the dealer's hand
    hiddenCard = false; // Set hiddenCard to false

}

// Destructor
Dealer::~Dealer(){

    delete &hand; // Delete the dealer's hand

}


// receiveCard()
// Receive a card
void Dealer::receiveCard(Card c){

    hand.addCard(c); // Add a card 'c' to the dealer's hand

}

// getHand()
// Get the dealer's hand
Hand Dealer::getHand() const{

    return hand; // Return the dealer's hand

}

// getHandValue()
// Get hand value
int Dealer::getHandValue() const{

    return hand.getValue(); // Return the dealer's hand value

}

// isBust()
// Check if the dealer's hand is a bust
bool Dealer::isBust() const{

    return hand.isBust(); // Return true if the dealer's hand is abust and false otherwise

}

// shouldHit()
// Determine if the dealer should hit
bool Dealer::shouldHit() const{

    return getHandValue() < 17; // Return true if the dealer's hand value is less than 17 and false if otherwise

}

// playTurn()
// Play the dealer's turn
void Dealer::playTurn(Deck& deck){

    revealHiddenCard();
    displayFullHand();

    while(shouldHit() && !isBust()){

        Card drawnCard = deck.dealCard(); // Draw a card from the deck
        receiveCard(drawnCard); // Add drawnCard to dealer's hand
        displayFullHand(); // Display the dealer's hand
    }

    if(isBust()){

        cout << "The dealer busts!" << endl;

    }
    else{

        cout << "The dealer stands on " << getHandValue() << endl;

    }
}

// displayHiddenHand()
// Display the dealer's hand with a hidden card
void Dealer::displayHiddenHand() const{

    if(hand.getNumCards() >= 2){

        hand.displayPartial();

    }
    else{

        cout << "Dealer's hand is not valid." << endl;

    }
}

// displayFullHand()
// Display the dealer's full hand
void Dealer::displayFullHand() const{

    hand.display(); // Display the dealer's full hand

}

// revealHiddenCard()
// Reveal the dealer's hidden card
void Dealer::revealHiddenCard(){

    hiddenCard = true; // Set hiddenCard to true

}

// isHiddenCardShown()
// Check if the hidden card has been shown
bool Dealer::isHiddenCardShown() const{

    return hiddenCard; // Return true if the hidden card has been shown and false if otherwise

}

// clearhand()
// Clear the dealer's hand for a new round
void Dealer::clearHand(){

    hand.clear();
    hiddenCard = false;
    
}
