#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "a4.h"

#define PRINT_OUT_DECK 0

#define PRINT_P1_HAND 0


Card* TakePlayer1Turn(Hand *p1hand) {
  printf("Player 1 played the card: ");

  PrintCard(p1hand->first_card->this_card);
  Card* out = p1hand->first_card->this_card;
  RemoveCardFromHand(p1hand->first_card->this_card, p1hand);
  printf("\n\n");

  return out;
}

void FlushIn() {
  char c;
  while ((c = getchar()) != '\n' && c != EOF) {
  }
}

Card* TakePlayer2Turn(Hand *p2hand) {
  printf("Player 2's turn: Which card do you want to play?\n");
  PrintHand(p2hand);
  printf("\n");

  int which_one = 0;
  scanf("%d", &which_one);

  while (which_one > (p2hand->num_cards_in_hand-1)) {
    printf("Please choose a number between %d and %d. ",
           0,
           (p2hand->num_cards_in_hand-1));
    scanf("%d", &which_one);
  }

  FlushIn();

  int i = 0;

  CardNode *cih = p2hand->first_card;

  while (i < which_one) {
    cih = cih->next_card;
    i++;
  }
  printf("Player 2 played the card: ");
  PrintCard(cih->this_card);
  printf("\n");

  Card* out = cih->this_card;
  RemoveCardFromHand(cih->this_card, p2hand);

  return out;
}

Suit GetTrump(Card *topcard) {
  printf("Time to choose trump!\n");
  return rand()%4; // TODO(ahs): replace with rand_r
}

void PrintTrump(Suit trump) {
  printf("\n\n*************************\n");
  printf("**** Trump is %s *****\n", GetSuit(trump));
  printf("*************************\n\n");
}


void PrintScore(int score1, int score2) {
  printf("Player 1: %d\tPlayer 2: %d", score1, score2);
}

// Returns 1 or 2, based on who won.
int PlayRound(Deck *game_deck) {
  Shuffle(game_deck);

  if (PRINT_OUT_DECK) {
    PrintDeck(game_deck);
  }

  // Deal the hand
  Hand *p1hand = CreateHand();
  Hand *p2hand = CreateHand();

  Deal(game_deck, p1hand, p2hand);

  printf("\n\n");

  if (PRINT_P1_HAND) {
    printf("Player 1: \n");
    PrintHand(p1hand);
  }

  // For each round:

  // Determine trump
  Suit trump = GetTrump(PeekAtTopCard(game_deck));
  PrintTrump(trump);

  // Start playing.
  int num_tricks = 0;
  int p1score = 0, p2score = 0;
  Card *led_card, *followed_card;

  printf("\n\n");
  printf("Starting the round...");

  while (num_tricks < kNumCardsInHand) {
    printf("\n---------------------\n");

    led_card = TakePlayer1Turn(p1hand);
    followed_card = TakePlayer2Turn(p2hand);

    if (IsLegalMove(p2hand, led_card, followed_card)) {
      // figure out who won

      if (WhoWon(led_card, followed_card, trump)) {
        printf("Player 1 took the trick. \n");
        // P1 won
        p1score++;
      } else {
        printf("Player 2 took the trick. \n");
        // P2 won
        p2score++;
      }
      printf("\n");
      PrintScore(p1score, p2score);
      printf("\n");
    } else {
      // It was not a legal move; player 2 loses!
      printf("\n\nPlayer 2 did not make a legal move!! \n");
      printf("The round is over. \n");
      printf("Player 1 wins by default. \n");
      p2score = -1;

      // Put the cards back in the deck
      // so we don't "lose" them
      PushCardToDeck(led_card, game_deck);
      PushCardToDeck(followed_card, game_deck);
      break;
    }

    // Put the cards back in the deck.
    PushCardToDeck(led_card, game_deck);
    PushCardToDeck(followed_card, game_deck);

    num_tricks++;
  }

  if (p2score > 0) {
    printf("\n\n");
    printf("Player %d won this round with %d tricks!\n",
      (p1score > p2score ? 1 : 2),
      (p1score > p2score ? p1score : p2score));
  }

  ReturnHandToDeck(p1hand, game_deck);
  ReturnHandToDeck(p2hand, game_deck);

  DestroyHand(p1hand);
  DestroyHand(p2hand);

  return (p1score > p2score ? 1 : 2);
}

int PlayGame(Deck *game_deck) {
  int num_rounds = 5;

  int which_player_won, i;

  int player1score = 0, player2score = 0;

  for (i = 0; i < num_rounds; i++) {
    printf("\n\n===========================\n");
    printf("Round # %d\n", i+1);
    printf("===========================\n\n");

    which_player_won = PlayRound(game_deck);

    if (which_player_won == 1) {
      player1score++;
    }    else {
      player2score++;
    }

    printf("\n\n\n");
    printf("Game Score so far: \n");
    PrintScore(player1score, player2score);
    printf("\n===========================\n\n");

    char input[5];

    printf("When you're ready, press <enter> to go to the next round. \n");
    char whichOne = getchar();
  }
  printf("\n\n");
  printf("Player %d won the game!\n",
    (player1score > player2score ? 1 : 2));

  PrintScore(player1score, player2score);

  printf("\n\n");
  return (player1score > player2score ? 1 : 2);
}

int main() {
  srand(time(NULL));

  printf("Welcome to NEUchre!\n");
  printf("When you're ready to play, press <enter>\n");

  char in;
  in = getchar();

  printf("Okay. ");

  Deck *game_deck = PopulateDeck();

  if (PRINT_OUT_DECK) {
    PrintDeck(game_deck);
  }

  printf("Would you like to play a [R]ound or a [G]ame?\n");

  in = getchar();

  switch (in) {
    case 'r':
      PlayRound(game_deck);
      break;
    case 'g':
      PlayGame(game_deck);
      break;
    default:
      printf("Quitting the game. ");
  }
  // Don't forget to free the deck!
  // I created it in this function, going to free it in this function.
  DestroyDeck(game_deck);
}
