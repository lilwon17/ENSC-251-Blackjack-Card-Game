#include "Player.hpp"
#include <cctype>   // for tolower()
#include <limits>   // for numeric_limits (clearing bad input)

// Constructor
Player::Player(){

    hasStood = false; // The player has not chosen to stand yet
    // Note: 'hand' is a member object, so its own default constructor
    // already runs automatically and creates an empty hand.

}

// Destructor
Player::~Player(){

    // Nothing to do here.
    // 'hand' is a member object (not allocated with new), so it is
    // destroyed automatically when the Player is destroyed.

}

// receiveCard()
// Add a card dealt from the deck to the player's hand
void Player::receiveCard(Card c){

    hand.addCard(c); // Add card 'c' to the player's hand

}

// getHand()
// Return a copy of the player's hand
Hand Player::getHand() const{

    return hand; // Return the player's current hand

}

// isBust()
// Check if the player's hand value is over 21
bool Player::isBust() const{

    return hand.isBust(); // Return true if the hand value is greater than 21

}

// chooseAction()
// Ask the player whether they want to hit or stand
char Player::chooseAction(){

    char choice; // Stores the player's input

    while(true){

        cout << "Would you like to hit (h) or stand (s)? ";
        cin >> choice;

        // If the input stream failed (ex: end of input), stop asking
        if(cin.fail()){

            cin.clear(); // Clear the error flags on cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the bad input
            cout << "Invalid input. Please enter 'h' or 's'." << endl;
            continue; // Ask again

        }

        choice = tolower(choice); // Accept 'H' and 'S' as well as 'h' and 's'

        if(choice == 'h' || choice == 's'){

            break; // Valid input, leave the loop

        }

        cout << "Invalid input. Please enter 'h' or 's'." << endl;

    }

    if(choice == 's'){

        hasStood = true; // The player has chosen to stand, their turn is over

    }

    return choice; // Return the chosen action to BlackjackGame

}

// getHasStood()
// Check if the player has chosen to stand
bool Player::getHasStood() const{

    return hasStood; // Return true if the player has stood and false if otherwise

}

// clearHand()
// Reset the player's hand for a new round
void Player::clearHand(){

    hand.clear();      // Remove all cards from the player's hand
    hasStood = false;  // The player can act again next round

}
