#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <string.h>

#include "a4.h"

#define kPrintDebug 1

// Implement the Hand and other functions in here
// Card functions
//----------------------------------------

// Creates a card, initializing the suit and name to that specified.
// Returns a pointer to the new card, which has beel allocated on the heap.
// It is the responsibility of the caller to call destroyCard()
// when it is done with the Card.
Card* CreateCard(Suit suit, Name name) {
    Card* card = (Card*)malloc(sizeof(card));
    if (card == NULL) {
        printf("Unable to malloc card");
        return NULL;
    }
    card->suit = suit;
    card->name = name;
    return card;
}

// Destroys the card, freeing any memory allocated for the card.
void DestroyCard(Card* card) {
    free(card);
}

/*
//----------------------------------------
// Hand functions
//----------------------------------------

// Creates a Hand struct and initializes any necessary fields.
// Returns a pointer to the new Hand, which has been allocated on the heap.
Hand* CreateHand();

// Adds a card to the hand.
void AddCardToHand(Card *card, Hand *hand);

// Removes a card from the hand.
// Does not free the card; it is the responsibility
// of the caller to free the card at the appropriate
// time.
// Returns a pointer to the card that is no longer in the hand.
Card* RemoveCardFromHand(Card *card, Hand *hand);

// Determines if there are any cards in the hand.
// Return 1 if the hand is empty; 0 otherwise.
int IsHandEmpty(Hand* hand);

// Destroys the hand, freeing any memory allocated for it.
void DestroyHand(Hand* hand);

//----------------------------------------
// Game functions
//----------------------------------------

// Shuffle the deck.
// Put them in a random order.
void Shuffle(Deck *aDeck);

// Given a deck (assume that it is already shuffled),
// take the top card from the deck and alternately give
// it to player 1 and player 2, until they both have
// kNumCardsInHand.
void Deal(Deck *aDeck, Hand *p1hand, Hand *p2hand);

// Create a Deck for this game, and add any
// needed cards to the deck.
// Return a pointer to the deck to be used for the game
Deck* PopulateDeck();

// Given a lead card, a players hand, and the card the player wants
// to play, is it legal?
// If the player has a card of the same suit as the ledCard, they
// must play a card of the same suit.
// If the player does not have a card of the same suit, they can
// play any card.
// Returns 1 if the move is legal; 0 otherwise.
int IsLegalMove(Hand *hand, Card *lead_card, Card *played_card);


// Given two cards that are played in a hand, which one wins?
// If the suits are the same, the higher card value wins.
// If the suits are not the same, player 1 wins, unless player 2 played trump.
// Returns 1 if the person who led won, 0 if the person who followed won.
int WhoWon(Card *lead_card, Card *followed_card, Suit trump);

// Take all the cards out of a given hand, and put them
// back into the deck.
void ReturnHandToDeck(Hand *hand, Deck *deck);

//----------------------------------------
// Challenge functions
//----------------------------------------

// Considering this for a challenge problem.
Card* GetBestMove(Hand *myHand, Card *ledCard, Suit trump);

// Sort the given hand in descending order of power.
//
// The sort order should be: all cards of the given trump should
// be the "highest", and A high down to 9;
// The other suits can be in random order, but the card values must go
// from high to low.
void SortHand(Hand *hand, Suit trump);

void ShuffleHand(Hand* hand);
*/
