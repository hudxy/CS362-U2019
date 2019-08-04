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
    for(p=0; i < numPlayer; i++) {
      G.deck[p][G.deckCount[p]] = baron;
      G.deckCount[p]++;
      G.deck[p][G.deckCount[p]] = tribute;
      G.deckCount[p]++;
      G.deck[p][G.deckCount[p]] = mine;
      G.deckCount[p]++;
      for(i = 0; i < maxHandCount; i++) {
        G.hand[p][i] = curse;   //set all cards in players hand to be curse cards
      }
      G.handCount[p] = maxHandCount;
    }

    /**************************************************************************
    *** Test to hand count and if cards from hand hand are in discard pile  ***
    ***************************************************************************/
    p=0; //p = first player
    pre = 0;
    post = 0;
    printf("Test to hand count and if cards from hand hand are in discard pile\n");

    Gtest = G;

    endTurn(&Gtest);

    str = "Test to check hand count of currentPlayer\n";
    asserts += asserttrue(0, Gtest.handCount[p], str);

    str = "Test to check if hand was discarded\n";
    pre = checkCards(G.discard[p], G.discardCount[p], curse);
    post = checkCards(Gtest.discard[p], Gtest.discardCount[p], curse);
    asserts += asserttrue(pre + 5, post, str);

    /*********************************
    *** Test to check whose turn   ***
    *********************************/
    printf("Test to check whose turn\n");
    memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state

    Gtest = G;

    endTurn(&Gtest);

    str = "Test to check whose turn\n";
    asserts += asserttrue(G.whoseTurn + 1, Gtest.whoseTurn, str);

    /***********************************************
    *** Test to check if next player draws hand  ***
    ************************************************/
    for(p=0; i < numPlayer; i++) {
      G.deck[p][G.deckCount[p]] = baron;
      G.deckCount[p]++;
      G.deck[p][G.deckCount[p]] = tribute;
      G.deckCount[p]++;
      G.deck[p][G.deckCount[p]] = mine;
      G.deckCount[p]++;
      for(i = 0; i < maxHandCount; i++) {
        G.hand[p][i] = -1;   //set all cards in players hand to be curse cards
      }
      G.handCount[p] = 0;
    }
    p = 0;

    printf("Test to check if next player draws hand\n");
    memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state

    Gtest = G;

    endTurn(&Gtest);

    str = "Test to check if next player draws hand\n";
    asserts += asserttrue(5, Gtest.handCount[p+1], str);


    printf("# FAILED TESTS: %d\n", asserts);

    return 0;
  }
