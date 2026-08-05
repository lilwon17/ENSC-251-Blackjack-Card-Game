#include <cstdlib>
#include <ctime>
#include "Deck.hpp"
using namespace std;

// Constructor
// Fills the deck with all 52 cards
Deck::Deck() {
    int index = 0;
    // Go through each suit
    for (int s = 0; s < 4; s++) {
        // Go through each rank
        for (int r = 1; r <= 13; r++) {
            cards[index] = Card(r, s);
            index++;
        }
    }
    numCards = DECK_SIZE;
    topIndex = 0;
}

// Destructor
// Nothing to do here
Deck::~Deck() {
}

// Mixes up the order of the cards
void Deck::shuffle() {
    // Start the random number generator
    srand(time(0));

    // Swap two random cards, over and over, to mix the deck
    for (int i = 0; i < 1000; i++) {
        int spot1 = rand() % DECK_SIZE;
        int spot2 = rand() % DECK_SIZE;

        Card temp = cards[spot1];
        cards[spot1] = cards[spot2];
        cards[spot2] = temp;
    }

    // Start dealing from the beginning again
    topIndex = 0;
}

// Gives out the card on top of the deck
Card Deck::dealCard() {
    // Get the card at the top
    Card dealt = cards[topIndex];

    // Move to the next card and count one less card left
    topIndex = topIndex + 1;
    numCards = numCards - 1;

    return dealt;
}

// Tells how many cards are left
int Deck::cardsRemaining() const {
    return numCards;
}

// Checks if there are no cards left
bool Deck::isEmpty() const {
    if (numCards == 0) {
        return true;
    }
    return false;
}

// Fills the deck back up and shuffles it
void Deck::reset() {
    int index = 0;
    // Put all 52 cards back in
    for (int s = 0; s < 4; s++) {
        for (int r = 1; r <= 13; r++) {
            cards[index] = Card(r, s);
            index++;
        }
    }

    numCards = DECK_SIZE;
    topIndex = 0;
    // Mix up the new deck
    shuffle();
}

// Prints how many cards are left
ostream& operator<<(ostream& out, const Deck& d) {
    out << "Cards remaining in deck: " << d.numCards;
    return out;
}
