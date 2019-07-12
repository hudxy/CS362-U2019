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

    G.deck[0][G.deckCount[0]] = baron;
    G.deckCount[0]++;
    G.deck[0][G.deckCount[0]] = tribute;
    G.deckCount[0]++;
    G.deck[0][G.deckCount[0]] = mine;
    G.deckCount[0]++;
    /***********************************************
    *** Test to check deck count after shuffle  ***
    ************************************************/
    printf("Test to check shuffle of deck\n");

    Gtest = G;

    shuffle(0, &Gtest); //call shuffle

    //test deck count
    str = "Test to check shuffle of deck\n";
    asserts += asserttrue(G.deckCount[0], Gtest.deckCount[0], str);

    /***********************************************
    *** Test to check shuffle of deck  ***
    ************************************************/
    checker = 0;
    printf("Test to check that decks are not the same after shuffle\n");
    for(i = 0; i < G.deckCount[0]; i++) {
      if(G.deck[0][i] == Gtest.deck[0][i]) {
        checker++;
      }
    }


    if(checker != G.deckCount[0]) {
      asserts += asserttrue(1, 1, str);
    } else {
      asserts += asserttrue(0, 1, str);
    }


    printf("# FAILED TESTS: %d\n", asserts);

    return 0;
  }
