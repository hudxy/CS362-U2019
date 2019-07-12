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
  int tributeRevealedCards[2] = {-1, -1};
  int maxHandCount = 5;
  int k[10] = {(int)ambassador, (int)tribute, (int)minion, (int)gardens, (int)mine, (int)remodel, (int)smithy, (int)village, (int)baron, (int)great_hall};
  struct gameState G, Gtest;

  memset(&G, 0, sizeof(struct gameState));   // clear the game state
  memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state
  r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

  for(j = 0; j < numPlayer; j++) {
    for(i = 0; i < maxHandCount; i++) {
      G.hand[j][i] = baron;   //set all cards in players hand to be curse cards
      G.deck[j][i] = copper;
      G.discard[j][i] = curse;

    }
    G.handCount[j] = maxHandCount;
    G.deckCount[j] = maxHandCount;
    G.discardCount[j] = maxHandCount;

  }


  printf ("TESTING tributeRe():\n");
  p=0;
  /****************************************************************************************
  *** Test to check when top 2 cards of next players deck are the same                 ***
  ****************************************************************************************/
  printf("Test to check when top 2 cards of next players deck are the same\n");
  memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state
  G.hand[p][maxHandCount -1] = tribute;   //set last card in current player to be tribute


  Gtest = G;  //set test gamestate to controll gameState

  tributeRe(&Gtest, p, tributeRevealedCards);

  str = "Test if coins are incremented by ONLY 2\n";
  asserts += asserttrue(G.coins +2, Gtest.coins, str);

  /****************************************************************************************
  *** Test to check when top 2 cards of next players deck are different (action and treasure card)                ***
  ****************************************************************************************/
  printf("Test to check when top 2 cards of next players deck are different (action and treasure card)\n");
  memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state
  G.hand[p][maxHandCount -1] = tribute;   //set last card in current player to be tribute
  G.deck[p+1][maxHandCount-1] = baron;
  tributeRevealedCards[0] = -1;
  tributeRevealedCards[1] = -1;

  Gtest = G;  //set test gamestate to controll gameState

  tributeRe(&Gtest, p, tributeRevealedCards);

  str = "Test if actions are incremented by 2\n";
  asserts += asserttrue(G.numActions +2, Gtest.numActions, str);
  str = "Test if coins are incremented by 2\n";
  asserts += asserttrue(G.coins +2, Gtest.coins, str);

  /****************************************************************************************
  *** Test to check when top 2 cards of next players deck are different (victory and treasure card)                ***
  ****************************************************************************************/
  printf("Test to check when top 2 cards of next players deck are different (victory and treasure card)\n");
  memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state
  G.hand[p][maxHandCount -1] = tribute;   //set last card in current player to be tribute
  G.deck[p+1][maxHandCount-1] = estate;
  tributeRevealedCards[0] = -1;
  tributeRevealedCards[1] = -1;

  Gtest = G;  //set test gamestate to controll gameState

  tributeRe(&Gtest, p, tributeRevealedCards);

  str = "Test if cards in players hand are incremented by 2\n";
  asserts += asserttrue(G.handCount[p] +2, Gtest.handCount[p], str);
  str = "Test if coins are incremented by 2\n";
  asserts += asserttrue(G.coins +2, Gtest.coins, str);

  /****************************************************************************************
  *** Test to check when next player has no cards in deck or discard pile        ***
  ****************************************************************************************/
  printf("Test to check when next player has no cards in deck or discard pile\n");
  memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state
  Gtest = G;  //set test gamestate to controll gameState
  Gtest.hand[p][maxHandCount -1] = tribute;   //set last card in current player to be tribute
  Gtest.deckCount[p+1] = 0;
  Gtest.discardCount[p+1] = 0;
  Gtest.handCount[p+1] = 0;

  tributeRevealedCards[0] = -1;
  tributeRevealedCards[1] = -1;

  tributeRe(&Gtest, p, tributeRevealedCards);

  str = "Test if cards in players hand remain constant\n";
  asserts += asserttrue(G.handCount[p], Gtest.handCount[p], str);
  str = "Test if coins remain constant\n";
  asserts += asserttrue(G.coins, Gtest.coins, str);
  str = "Test if actions are remain constant\n";
  asserts += asserttrue(G.numActions, Gtest.numActions, str);

  /****************************************************************************************
  *** Test to check when next player has no cards in deck and 1 card in discard pile        ***
  ****************************************************************************************/
  printf("Test to check when next player has no cards in deck and 1 card in discard pile \n");
  memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state
  Gtest = G;  //set test gamestate to controll gameState
  Gtest.hand[p][maxHandCount -1] = tribute;   //set last card in current player to be tribute
  Gtest.deckCount[p+1] = 0;
  Gtest.discardCount[p+1] = 1;
  Gtest.handCount[p+1] = 0;

  tributeRevealedCards[0] = -1;
  tributeRevealedCards[1] = -1;

  tributeRe(&Gtest, p, tributeRevealedCards);

  str = "Test if cards in players hand remain constant\n";
  asserts += asserttrue(G.handCount[p], Gtest.handCount[p], str);
  str = "Test if coins remain constant\n";
  asserts += asserttrue(G.coins, Gtest.coins, str);
  str = "Test if actions are remain constant\n";
  asserts += asserttrue(G.numActions, Gtest.numActions, str);

  /****************************************************************************************
  *** Test to check when next player has one card in deck and no cards in discard pile        ***
  ****************************************************************************************/
  printf("Test to check when next player has one card in deck and no cards in discard pile\n");
  memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state
  Gtest = G;  //set test gamestate to controll gameState
  Gtest.hand[p][maxHandCount -1] = tribute;   //set last card in current player to be tribute
  Gtest.deckCount[p+1] = 1;
  Gtest.discardCount[p+1] = 0;
  Gtest.handCount[p+1] = 0;

  tributeRevealedCards[0] = -1;
  tributeRevealedCards[1] = -1;


  tributeRe(&Gtest, p, tributeRevealedCards);

  str = "Test if cards in players hand remain constant\n";
  asserts += asserttrue(G.handCount[p], Gtest.handCount[p], str);
  str = "Test if coins remain constant\n";
  asserts += asserttrue(G.coins, Gtest.coins, str);
  str = "Test if actions are remain constant\n";
  asserts += asserttrue(G.numActions, Gtest.numActions, str);

  printf("# FAILED TESTS: %d\n", asserts);

  return 0;
}
