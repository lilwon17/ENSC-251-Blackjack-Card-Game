#ifndef CARD_CLASS
#define CARD_CLASS

class Card{

private:

    // Properties of playing cards
    int suit;
    int rank;

public:

    // Constructors

    // Default
    // Postcondition:
    // Create a variable of class Card with rank(1) and suit(0)
    Card();

    // Specific
    // Postcondition:
    // Create a variable of class Card with rank(r) and suit(s)
    Card(int r, int s);

    // Member Functions

    // isFaceCard() - Check Face Card
    // *Mainly used for debugging purposes*
    // Precondition:
    // Card rank must be valid; Card must be a valid card
    // Postcondition:
    // Return true if card has rank value greater or equal to 11 and false if otherwise
    bool isFaceCard();

    // isValid() - Check if card is valid
    // *Mainly used for debugging purposes*
    // Precondition:
    // Card components must be numerical values
    // Postcondition:
    // If the value for rank or suit is invalid return false and error message
    // else, return true and a message saying the card is valid
    bool isValid();

    /* Mapping

    * 0 = Clubs
    * 1 = Diamonds
    * 2 = Hearts
    * 3 = Spades

    * 1 = Ace
    ...
    * 11 = Jack
    * 12 = Queen
    * 13 = King
    
    */

    // Overload Insertion Operator
    friend ostream& operator<<(ostream& os, const Card& c);

    // Overload Extraction Operator
    friend ostream& operator<<(ostream& is, Card& c);

};

#endif
