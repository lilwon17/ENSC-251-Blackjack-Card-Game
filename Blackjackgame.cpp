#include "Blackjackgame.hpp"
#include <cctype>
#include <limits>


// Constructor
BlackjackGame::BlackjackGame() {

    Deck deck(); // Create a new deck
    Player player(); // Create a new player
    Dealer dealer(); // Create a new dealer
    isGameRunning = true; // Set isGameRunning to true

}

// Destructor
BlackjackGame::~BlackjackGame() {


}

// playGame()
// Run the game loop
void BlackjackGame::playGame() {

    cout << "Welcome to Blackjack!" << endl;

    while (isGameRunning) {
        startRound(); // Start a new round

        const int playerValue = player.getHand().getValue(); // Get the player's hand value
        const int dealerValue = dealer.getHand().getValue(); // Get the dealer's hand value
        const bool openingBlackjack = playerValue == 21 || dealerValue == 21; // Check if either the player or dealer has a blackjack

        if (!openingBlackjack) {
            playerTurn(); // Play the player's turn

            if (!player.isBust()) {
                dealerTurn(); // Play the dealer's turn
            }

        }
        else {
            dealer.revealHiddenCard(); // Reveal the dealer's hidden card
            dealer.displayFullHand(); // Display the dealer's full hand

        }
        determineWinner(); // Determine and display the winner
        isGameRunning = playAgain(); // Ask the player if they want to play again | isGameRunning is dependent on playAgain()


    }

    cout << "Thanks for playing our game!" << endl;

}

// startRound()
// Start a new round
void BlackjackGame::startRound() {
    resetRound(); // Reset the round
    dealStartingCards(); // Deal the starting cards to the player and dealer

    cout << "\nYour hand: " << player.getHand() << endl; // Display the player's hand
    dealer.displayHiddenHand(); // Display the dealer's hand with a hidden card

}


// dealStartingCards()
// Deal the starting cards to the player and dealer
void BlackjackGame::BlackjackGame::dealStartingCards() {

    for (int i = 0; i < 2; i++) {
        player.receiveCard(deck.dealCard()); // Deal a card to the player
        dealer.receiveCard(deck.dealCard()); // Deal a card to the dealer

    }
}

// playerTurn()
// Play the player's turn
void BlackjackGame::playerTurn() {
    while (!player.isBust() && !player.getHasStood()) {
        char action = player.chooseAction(); // Ask the player whether they want to hit or stand
        if (action == 'h') {
            Card drawn = deck.dealCard(); // Deal a card to the player
            cout << "You drew: " << drawn << endl; // Display the card drawn
            player.receiveCard(drawn); // Add the card to the player's hand 
            cout << "Your hand: " << player.getHand() << endl; // Display the player's new hand after drawn card
        }

        if (player.isBust()) {
            cout << "You busted!" << endl; // Let player know they busted
        }
    }
}

// dealerTurn()
// Play the dealer's turn
void BlackjackGame::dealerTurn() {
    dealer.playTurn(deck); // Play the dealer's turn

}

// determineWinner()
// Determine and display the winner of the round
void BlackjackGame::determineWinner() const {
    int playerValue = player.getHand().getValue(); // Get the player's hand value
    int dealerValue = dealer.getHand().getValue(); // Get the dedaler's hand value

    cout << "\nFinal Hands: " << endl;
    cout << "Your hand: " << player.getHand() << endl; // Display the player's final hand
    dealer.displayFullHand(); // Display the dealer's final hand

    if (player.isBust() || dealerValue > playerValue) {
        cout << "Dealer wins!" << endl; // Dealer wins if player busts or has lower value
    }
    else if (dealer.isBust() || playerValue > dealerValue) {
        cout << "You win!" << endl; // Player wins if dealer busts or has lower value
    }
    else {
        cout << "It's a push (tie)" << endl; // It's a tie if neither busts and values are equal
    }   
}

// playAgain()
// Ask the player if they want to play another round
bool BlackjackGame::playAgain() {
    char choice;
    while (true) {
        cout << "\nDo you want to play again? (y/n): ";
        if (cin >> choice) {
            choice = static_cast<char>(tolower(static_cast<unsigned char>(choice))); // Convert to lowercase
            if (choice == 'y') {
                return true; // Player chooses to play again
            } 
            else if (choice == 'n') {
                return false; // Player chooses not to play again
            }
            else {
                cout << "Invalid input. Please enter 'y' or 'n'." << endl; // User enters invalid input
                cin.clear(); // Clear the error 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the line
            }
        }
    }
}

// resetRound()
// Reset all round specific data
void BlackjackGame::resetRound() {
    deck.reset(); // Reset the deck
    player.clearHand(); // Clear the player's hand
    dealer.clearHand(); // Clear the player's hand
}
