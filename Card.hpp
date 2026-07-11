#ifndef CARD_HPP
#define CARD_HPP

#include <iostream>
using namespace std;

class Card {
private:
    // Properties of playing cards
    int rank;   // 1-13 (1=Ace, 11=Jack, 12=Queen, 13=King)
    int suit;   // 0-3 (0=Clubs, 1=Diamonds, 2=Hearts, 3=Spades)

public:
    // Constructors
    // Default
    Card();
    // Postcondition:
    // Creates a Card with rank(1) and suit(0) -> Ace of Clubs

    // Constructor with parameters
    Card(int r, int s);
    // Precondition:
    // r must be between 1-13, s must be between 0-3
    // Postcondition:
    // Creates a Card with rank(r) and suit(s)

    // Destructor
    ~Card();

    // Accessors
    // Return the rank of the card
    int getRank() const;
    // Postcondition:
    // Returns the rank value (1-13)

    // Return the suit of the card
    int getSuit() const;
    // Postcondition:
    // Returns the suit value (0-3)

    // Return Blackjack value of the card
    int getValue() const;
    // Precondition:
    // Card must be valid
    // Postcondition:
    // Returns 10 for face cards (J,Q,K), returns rank value for number cards,
    // returns 11 for Ace (Hand class will handle reducing Ace to 1 if needed)

    // Check if card is a face card
    bool isFaceCard() const;
    // Precondition:
    // Card rank must be valid
    // Postcondition:
    // Returns true if rank is Jack, Queen, or King, false otherwise

    // Check if card is an Ace
    bool isAce() const;
    // Precondition:
    // Card rank must be valid
    // Postcondition:
    // Returns true if rank equals 1, false otherwise

    // Check if card is valid
    bool isValid() const;
    // Precondition:
    // Card components must be numerical values
    // Postcondition:
    // Returns false and prints an error message if rank or suit is invalid,
    // else returns true

    // Overloaded comparison operators
    // Compares cards based on rank (useful for sorting/Ace logic)
    bool operator==(const Card& other) const;
    bool operator<(const Card& other) const;
    bool operator>(const Card& other) const;

    // Overload Insertion Operator
    friend ostream& operator<<(ostream& out, const Card& c);
    // Precondition:
    // Card must be valid
    // Postcondition:
    // Displays the card (ex: "King of Spades") to the output stream

    // Overload Extraction Operator
    friend istream& operator>>(istream& is, Card& c);
    // Precondition:
    // Input stream must contain two integers: rank and then suit
    // Postcondition:
    // Reads rank and suit values into the Card object from the input stream
};

#endif
