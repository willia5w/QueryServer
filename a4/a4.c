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
    Card* card = (Card*)malloc(sizeof(Card));
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


//----------------------------------------
// Hand functions
//----------------------------------------

// Creates a Hand struct and initializes any necessary fields.
// Returns a pointer to the new Hand, which has been allocated on the heap.
Hand* CreateHand() {
    Hand* hand = (Hand*)malloc(sizeof(Hand));
    if (hand == NULL) {
        printf("Unable to malloc hand");
        return NULL;
    }
    hand->num_cards_in_hand = kEmptyHand;
    hand->first_card = NULL;
    return hand;
}

// Creates a CardNode to be used for adding a card to a hand
CardNode* CreateCardNode(Card* card) {
    CardNode* new_card = (CardNode*)malloc(sizeof(CardNode));
    if (new_card == NULL) {
        printf("Unable to malloc new card");
        return NULL;
    }
    new_card->next_card = NULL;
    new_card->this_card = card;
    new_card->prev_card = NULL;
    return new_card;
}

// Destroys a CardNode when a Card is removed from a Hand.
void DestroyCardNode(CardNode* node) {
    free(node);
}

// Adds a card to the hand.
void AddCardToHand(Card *card, Hand *hand) {
    CardNode* new_card = CreateCardNode(card);

    // Case when added card is first card in hand
    if (hand->num_cards_in_hand == kEmptyHand) {
        hand->num_cards_in_hand++;  // There will be 1 card in the hand
        hand->first_card = new_card;
    } else {
        hand->first_card->prev_card = new_card;
        new_card->next_card = hand->first_card;
        hand->first_card = new_card;
        // At last card, able to add new card
        hand->num_cards_in_hand++;  // Increment number of cards in hand
    }
}

// Removes a card from the hand.
// Does not free the card; it is the responsibility
// of the caller to free the card at the appropriate
// time.
// Returns a pointer to the card that is no longer in the hand.
Card* RemoveCardFromHand(Card *card, Hand *hand) {
    CardNode* check_card = hand->first_card;
  
    if (check_card->prev_card == NULL 
        && check_card->next_card == NULL) { 
        DestroyCardNode(check_card);
        hand->first_card = NULL;
        hand->num_cards_in_hand--;
        return card;
    }
    while (check_card->this_card != card) {
        check_card = check_card->next_card;
    }
    if (check_card->prev_card == NULL 
        && check_card->next_card != NULL) {
        check_card->next_card->prev_card = NULL;
        hand->first_card = check_card->next_card;
        hand->num_cards_in_hand--;
        DestroyCardNode(check_card);
        return card;
    } else if (check_card->next_card == NULL
         && check_card->prev_card != NULL) {
        check_card->prev_card->next_card = NULL;
        check_card->prev_card = NULL;
        DestroyCardNode(check_card);
        hand->num_cards_in_hand--;
        return card;
    } else {
        check_card->prev_card->next_card = check_card->next_card;
        check_card->next_card->prev_card = check_card->prev_card;
        check_card->prev_card = NULL;
        check_card->next_card = NULL;
        DestroyCardNode(check_card);
        hand->num_cards_in_hand--;
        return card;
    }
}


// Determines if there are any cards in the hand.
// Return 1 if the hand is empty; 0 otherwise.
int IsHandEmpty(Hand* hand) {
    if (hand->num_cards_in_hand == 0) {
        return 1;
    }
    return 0;
}


// Destroys the hand, freeing any memory allocated for it.
void DestroyHand(Hand* hand) {
    free(hand);
}

//----------------------------------------
// Game functions
//----------------------------------------

// Shuffle the deck.
// Put them in a random order.
// Uses the Fischer-Yates algorithm from geeksforgeeks.org
void Shuffle(Deck *aDeck) {
    Card* temp;
    for (int i = aDeck->top_card; i > 0; i--) {
        int j = rand() % (kNumCardsInDeck);
        temp = aDeck->cards[i];
        aDeck->cards[i] = aDeck->cards[j];
        aDeck->cards[j] = temp;
    }
}

// Given a deck (assume that it is already shuffled),
// take the top card from the deck and alternately give
// it to player 1 and player 2, until they both have
// kNumCardsInHand.
void Deal(Deck *aDeck, Hand *p1hand, Hand *p2hand) {
    for (int i = 0; i < kNumCardsInHand; i++) {
        AddCardToHand(PopCardFromDeck(aDeck), p1hand);
        AddCardToHand(PopCardFromDeck(aDeck), p2hand);
    }
}

// Create a Deck for this game, and add any
// needed cards to the deck.
// Return a pointer to the deck to be used for the game
Deck* PopulateDeck() {
    Deck* deck = CreateDeck();
    for (int s = HEARTS; s <= DIAMONDS; s++) {
        for (int n = NINE; n <= ACE; n++) {
            deck = PushCardToDeck(CreateCard(s, n), deck);
        }
    }
    return deck;
}

// Given a lead card, a players hand, and the card the player wants
// to play, is it legal?
// If the player has a card of the same suit as the ledCard, they
// must play a card of the same suit.
// If the player does not have a card of the same suit, they can
// play any card.
// Returns 1 if the move is legal; 0 otherwise.
int IsLegalMove(Hand *hand, Card *lead_card, Card *played_card) {
    if (played_card->suit == lead_card->suit) {
        return 1;
    }
    for (int i = 0; i < hand->num_cards_in_hand; i++) {
        if (hand->first_card->this_card->suit == lead_card->suit) {
            return 0;   // Found card that must be played
        }
        if (hand->first_card->next_card != NULL) {
            hand->first_card->this_card
                    = hand->first_card->next_card->this_card;
        }
    }
    return 1;  // Played card is legal, no suit match in player hand
}


// Given two cards that are played in a hand, which one wins?
// If the suits are the same, the higher card value wins.
// If the suits are not the same, player 1 wins, unless player 2 played trump.
// Returns 1 if the person who led won, 0 if the person who followed won.
int WhoWon(Card *lead_card, Card *followed_card, Suit trump) {
    if (lead_card->suit == followed_card->suit
        && trump != followed_card->suit) {
        if (lead_card->name == followed_card->name) {
            return 1;
        } else {
            return 0;
        }
    }
    if (lead_card->suit != followed_card->suit
        && trump == followed_card->suit) {
        return 0;
    } else {
        return 1;
    }
}

// Take all the cards out of a given hand, and put them
// back into the deck.
void ReturnHandToDeck(Hand *hand, Deck *deck) {
     while (hand->num_cards_in_hand > 0) {
        CardNode* check = hand->first_card;
        Card* removed_card = RemoveCardFromHand(check->this_card, hand);
        deck = PushCardToDeck(removed_card, deck);
    }
}

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


