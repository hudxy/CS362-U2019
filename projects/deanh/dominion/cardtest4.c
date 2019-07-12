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


    /******************************************
    *** Test to check unused Players score  ***
    *******************************************/
    p=0; //p = first player
    printf("Test to check unused Players score\n");

    Gtest = G;

    getWinners(players, &Gtest);

    for(i = numPlayer; i < MAX_PLAYERS; i++) {
      if(players[i] != 0) {
        checker = 1;
      }
    }

    str = "Test to check unused Players score\n";
    asserts += asserttrue(0, checker, str);

    /************************************************
    *** Test to check if one player is the winners  ***
    ************************************************/
    printf("Test to check if one player is the winners\n");

    str = "Test to check if one player is the winners\n";
    asserts += asserttrue(players[p] +1, players[p+1], str);

    /**************************************************************
    *** Test to check if all players have same victory cards  ***
    **************************************************************/
    printf("Test to check if all players have same victory cards\n");

    memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &Gtest); // initialize a new game

    Gtest.handCount[p] = 0;
    Gtest.deckCount[p] = 0;
    Gtest.discardCount[p] = 0;

    Gtest.handCount[p+1] = 0;
    Gtest.deckCount[p+1] = 0;
    Gtest.discardCount[p+1] = 0;

    Gtest.whoseTurn = 1;

    getWinners(players, &Gtest);

    str = "Test to check if all players have same victory cards\n";
    asserts += asserttrue(players[p], players[p+1], str);

    printf("# FAILED TESTS: %d\n", asserts);

    return 0;
  }
