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
    int i;
    int seed = 1000;
    int numPlayer = 2;
    int asserts = 0;
    int p, r, handCount, pre, post;
    int maxHandCount = 5;
    int k[10] = {(int)ambassador, (int)tribute, (int)minion, (int)gardens, (int)mine, (int)remodel, (int)smithy, (int)village, (int)baron, (int)great_hall};
    struct gameState G;
    memset(&G, 0, sizeof(struct gameState));   // clear the game state

    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game


    printf ("TESTING minionRe():\n");
      /*********************************************
      *** Test to check number of action turns   ***
      **********************************************/
      pre = G.numActions;
      minionRe(&G, p, 1, 0, maxHandCount-1);
      post = G.numActions;

      #if (NOISY_TEST == 1)
            printf("Test player %d with %d # of actions before and %d after baron().\n", p, pre, post);
            printf("Expected result of actions is %d\n", pre+1);
      #endif

      char * str = "Number of actions test.\n";
      asserts += asserttrue(pre + 1 , post, str);

      /******************************************************
      *** Test case of +2 coins                           ***
      *******************************************************/
      pre = 0;
      post = 0;
      memset(&G, 0, sizeof(struct gameState));   // clear the game state
      r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

      for(i = 0; i < maxHandCount; i++) {
        G.hand[p][i] = curse;   //set all cards in players hand to be curse cards
      }
      G.hand[p][maxHandCount-1] = minion;   //set last card in players hand to be minion
      G.handCount[p] = maxHandCount;      //set hand count of player

      int handpre = checkCards(G.hand[p], *G.handCount, minion);
      int playedcardpre = checkCards(G.playedCards, G.playedCardCount, minion);
      pre = G.coins;
      minionRe(&G, p, 1, 0, maxHandCount-1);
      int handpost = checkCards(G.hand[p], *G.handCount, minion);
      int playedcardpost = checkCards(G.playedCards, G.playedCardCount, minion);
      post = G.coins;

      #if (NOISY_TEST == 1)
          printf("Test player %d with %d coins before and %d after baron().\n", p, pre, post);
          printf("Expected result is coins is %d\n", pre+2);
      #endif

      str = "Number of coins test.\n";
      asserts += asserttrue(pre+2, post, str);
      str = "Minion card added to played cards pile.\n";
      asserts += asserttrue(playedcardpre + 1, playedcardpost, str);
      str = "Minion card removed from hand test.\n";
      asserts += asserttrue(handpre - 1, handpost, str);

      /******************************************************
      *** Test case of discarding hand                    ***
      *******************************************************/
      pre = 0;
      post = 0;
      int playerCardCountPre[numPlayer];
      int playerCardCountPost[numPlayer];
      memset(&G, 0, sizeof(struct gameState));   // clear the game state
      r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
      int j;
      for(j = 0; j < numPlayer; j++) {
        for(i = 0; i < maxHandCount; i++) {
          G.hand[j][i] = copper;   //set all cards in players hand to be curse cards
          G.deck[j][i] = copper;
        }
        G.handCount[j] = maxHandCount;
        G.deckCount[j] = maxHandCount;
      }
      G.hand[p][maxHandCount-1] = minion;   //set last card in players hand to be minion

      for(i = 0; i < numPlayer; i++) {
        playerCardCountPre[i] = G.handCount[i];
      }
      minionRe(&G, p, 0, 1, maxHandCount-1);
      for(i = 0; i < numPlayer; i++) {
        playerCardCountPost[i] = G.handCount[i];
      }

      str = "Discarding other player's hand test.";
      for(i = 0; i < numPlayer; i++) {
        if(i == p) {
          str = "Test that discards current player's hand.";
          asserts += asserttrue(4, playerCardCountPost[i], str);}
        else if(playerCardCountPost[i] == 4)
          asserts += asserttrue(4, playerCardCountPost[i], str);
        else
          asserts += asserttrue(playerCardCountPre[i], playerCardCountPost[i], str);
      }

    printf("# FAILED TESTS: %d\n", asserts);

    return 0;
  }
