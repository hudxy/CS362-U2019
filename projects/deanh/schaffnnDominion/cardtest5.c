#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 0

int checkCards(int * array, int size, int value) {
  int i;
  int count = 0;
  for(i = 0; i < size; i++) {
    if(array[i] == value) {
      count++;
    }
  }
  return count;
}

int asserttrue(int left, int right, char * test) {
  if(left == right) {
    printf("PASSED: %s\n", test);
    return 0;
  }
  else {
    printf("FAILED: %s\n", test);
    return 1;
  }
}

int main() {
    int i, j;
    char * str;
    int seed = 1000;
    int numPlayer = 2;
    int players[MAX_PLAYERS];
    int asserts = 0;
    int p, r, handCount, pre, post;
    int checker = 0;
    int maxHandCount = 5;
    int k[10] = {(int)ambassador, (int)tribute, (int)minion, (int)gardens, (int)mine, (int)remodel, (int)smithy, (int)village, (int)baron, (int)great_hall};
    struct gameState G, Gtest;
    memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state

    memset(&G, 0, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

    p=0; //p = first player
    //random cards added to deck
    //deck =  baron
    //Hand = 5* Curse
    //Discard = 5 * ambassador
    for(p=0; i < numPlayer; i++) {
      G.deck[p][G.deckCount[p]] = baron;
      G.deckCount[p]++;
      // G.deck[p][G.deckCount[p]] = tribute;
      // G.deckCount[p]++;
      // G.deck[p][G.deckCount[p]] = mine;
      // G.deckCount[p]++;
      for(i = 0; i < maxHandCount; i++) {
        G.hand[p][i] = curse;   //set all cards in players hand to be curse cards
        G.discard[p][i] = ambassador;
      }
      G.handCount[p] = maxHandCount;
      G.discardCount[p] = maxHandCount;
    }

    //check when deck is empty & discard pile is not empty
    //check when deck is empty & discard pile is empty
    //check when deck is not empty

    /***********************************************
    *** Test to check when deck is not empty     ***
    ***********************************************/
    p=0; //p = first player
    printf("Test to check when deck is not empty\n");

    Gtest = G;
    checker = drawCard(p, &Gtest);

    str = "Test for function returned correctly\n";
    asserts += asserttrue(0, checker, str);
    str = "Test for card in hand\n";
    checker = checkCards(Gtest.hand[p], Gtest.handCount[p], baron);
    asserts += asserttrue(1, checker, str);
    str = "Test for card removed from deck\n";
    checker = checkCards(Gtest.deck[p], Gtest.deckCount[p], baron);
    asserts += asserttrue(0, checker, str);

    /************************************************************************
    *** Test to check when deck is empty & discard pile is not empty      ***
    *************************************************************************/
    p=0; //p = first player
    printf("Test to check when deck is empty & discard pile is not empty\n");
    memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state

    Gtest = G;
    Gtest.deckCount[p] = 0;
    checker = drawCard(p, &Gtest);


    str = "Test for function returned correctly\n";
    asserts += asserttrue(0, checker, str);
    str = "Test for card in hand\n";
    checker = checkCards(Gtest.hand[p], Gtest.handCount[p], ambassador);
    asserts += asserttrue(1, checker, str);
    str = "Test for card removed from deck\n";
    checker = checkCards(Gtest.deck[p], Gtest.deckCount[p], ambassador);
    if(checker > 0) {
      asserts += asserttrue(1, 1, str);
    } else {
      asserts += asserttrue(0, 1, str);
    }
    str = "Test for discard pile changed to deck\n";
    asserts += asserttrue(0,  Gtest.discardCount[p], str);

    /************************************************************************
    *** Test to check when deck is empty & discard pile is empty      ***
    *************************************************************************/
    printf("Test to check when deck is empty & discard pile is empty\n");
    memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state

    Gtest = G;
    Gtest.deckCount[p] = 0;
    Gtest.discardCount[p] = 0;

    checker = drawCard(p, &Gtest);

    str = "Test for function returned correctly\n";
    asserts += asserttrue(-1, checker, str);

    printf("# FAILED TESTS: %d\n", asserts);

    return 0;
  }
