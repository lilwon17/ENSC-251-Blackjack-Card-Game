#include "Hand.hpp"

//Constructor
Hand::Hand()
{
	numCards = 0;

}

//Destructor
Hand::~Hand()
{

}

//Overloaded insertion operator
ostream& operator<<(ostream& os, const Hand& h)
{
	for (int i = 0; i < h.numCards; i++)
	{
		os << h.cards[i] << endl;
	}

	os << "Total value: " << h.getValue();

	return os;
}

//getNumCards()
//return numCards in the hand
int Hand::getNumCards() const
{
	return numCards;
}

//clear()
//clear all the cards in a hand
void Hand::clear()
{
	numCards = 0;

}

//addCard(Card c)
//Add a card in the hand
void Hand::addCard(Card c)
{
	if (numCards < MAX_HAND_SIZE)
	{
		cards[numCards] = c;
		numCards++;
	}
	else
	{
		cout << "Hand is full. Cannot add more" << endl;
	}
}

//getValue()
//give us the value of the sum of all cards, but also decides if Ace is 1 or 11
int Hand::getValue() const
{
	int total = 0;
	int aceCount = 0;
	for (int i = 0; i < numCards;i++)
	{
		total += cards[i].getValue();

		if (cards[i].isAce())
			aceCount++;
	}
//reduce Aces from 11 to 1 one at a time, only while busting

	while (total > 21 && aceCount > 0)
	{
		total -= 10;
		aceCount--;
	}
	return total;
}
	
//isBust()
//check if hand is over 21
bool Hand::isBust() const
{
	return getValue() > 21;
}

//display()
//show every card and the total value
void Hand::display() const
{
	for (int i = 0; i < numCards;i++)
		cout << cards[i] << endl;

	cout << "Total value: " << getValue() << endl;
}

//displayPartial()
//show only the first card, hide the rest
void Hand::displayPartial() const
{
	if (numCards > 0)
	{
		cout << cards[0] << endl;
		cout << "[Hidden Card]" << endl;
	}
}

