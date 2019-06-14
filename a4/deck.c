#include <stdio.h>
#include <stdlib.h>

#include "a4.h"

#define PRINT_DEBUG 1
#define kEmptyDeck -1

// Put your deck functions in here
// Deck functions
//----------------------------------------
// Implement these functions in deck.c.
// Creates the deck to be used for NEUchre.
// Assume that the value of cards are:
// Nine=9; Ten=10; Jack=11; and so on, up to Ace=14.

// Creates the deck, initializing any fields necessary.
// Returns a pointer to the deck, which has been allocated on the heap.
Deck* CreateDeck() {
    Deck* deck = (Deck*)malloc(sizeof(Deck));
    deck->top_card = kEmptyDeck;
    return deck;
}

// Adds a card to the top of the deck.
// Returns a pointer to the deck.
Deck* PushCardToDeck(Card* card, Deck* deck) {
    if (deck->top_card < kNumCardsInDeck - 1) {
        deck->top_card++;
        deck->cards[deck->top_card] = card;
    }
    return deck;
}

// Shows the top card, but does not remove it from the stack.
// Returns a pointer to the top card.
Card* PeekAtTopCard(Deck* deck) {
    Card* peek_card;
    if (deck->top_card != kEmptyDeck) {
      peek_card = deck->cards[deck->top_card];
      return peek_card;
    }
    return NULL;
}

// Removes the top card from the deck and returns it.
// The card should NOT be freed at this point; it is the
// responsibility of the caller to free the card at the
// appropriate time.
// Returns a pointer to the top card in the deck.
Card* PopCardFromDeck(Deck* deck) {
    Card* pop_card;
    if (IsDeckEmpty(deck) == 0) {
      pop_card = deck->cards[deck->top_card];
      deck->top_card--;
      return pop_card;
    }
    return NULL;
}


// Determines if the deck is empty.
// Returns 1 if the Deck is empty, 0 otherwise.
int IsDeckEmpty(Deck* deck) {
    if (deck->top_card == kEmptyDeck) {
        return 1;
    } else {
    return 0;
    }
}

// Destroys the deck, freeing any memory allocated
// for this deck (the cards and the deck).
// DestroyDeck should call DestroyCard on all of the
// cards in the deck.
void DestroyDeck(Deck* deck) {
    if (IsDeckEmpty(deck) == 1) {
      free(deck);
    } else if (IsDeckEmpty(deck) == 0) {   
      while (IsDeckEmpty(deck) == 0) {
        int i = deck->top_card;
        DestroyCard(PopCardFromDeck(deck));
      }
    free(deck);
    deck = NULL;
    }
}




