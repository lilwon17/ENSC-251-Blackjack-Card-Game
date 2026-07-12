#ifndef BLACKJACKGAME_HPP
#define BLACKJACKGAME_HPP

#include <iostream>
#include "Deck.hpp"
#include "Player.hpp"
#include "Dealer.hpp"
using namespace std;

class BlackjackGame{

private:
    Deck deck; // The deck that is used for the current round
    Player player; // The user/player
    Dealer dealer; // The program controlled dealer
    bool isGameRunning; // Keeps track of if the user continues the game or not

public:
    // Constrcutor
    BlackjackGame();
    // Postcondition:
    // Creates a BlackjackGame object with a deck, player, and dealer
    // Sets isGameRunning to true

    // Destructor
    ~BlackjackGame();

    // Run the game loop
    void playGame();
    // Postcondition:
    // Run the game
    // Runs repeatedly and starts new rounds until the player/user decides to quit

// Private helper member functions so that the steps of the game doesn't get distrupted by other parts of the program
private:
    // Start a new round
    void startRound();
    // Postcondition:
    // Resetes the deck, shuffles it, and clears the player and dealer's hands
    // Deals the cards to the player and dealer (two cards each)

    // deal the starting cards
    void dealStartingCards();
    // Precondition:
    // the deck must contain enough cards to deal two cards to both the player and dealer
    // and the deck must have enough to play a round
    // Postcondition:
    // The player and the dealer gets dealt two cards each

    // Play the player's turn
    void playerTurn();
    // Precondition:
    // The player and dealer must already have their starting cards
    // Postcondition:
    // Allows the player to hit or stand
    // If the player hits, a card is dealt to the player
    // The player's turn ends when the player stands or busts

    // Play the dealer's turn
    void dealerTurn();
    // Precondition:
    // The player did not bust
    // Postcondition:
    // Reveal the dealer's hidden card
    // Delear draws until their hand value reaches 17 or higher, or until it busts

    // Determine and display the winner 
    void determineWinner() const;
    // Precondition:
    // The round must be finished
    // Postcondition:
    // Compares the hand value of the player and the dealer
    // Display whether the player wins, the dealer wins, or if it's a push

    // Ask the user if they want to play another round
    bool playAgain();
    // Postcondition:
    // Prompt player/user to enter whether they want to play again
    // returns true if the player wants another round and false if otherwise

    // Reset all round-specific data
    void resetRound();
    // Postcondition;
    // Clears the player and dealer's hands and resets the deck for a new round
};

#endif