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
    int asserts = 0;
    int seed = 1000;
    int numPlayer = 2;
    int p, r, handCount, pre, post;
    int maxHandCount = 5;
    int k[10] = {(int)ambassador, (int)tribute, (int)minion, (int)gardens, (int)mine, (int)remodel, (int)smithy, (int)village, (int)baron, (int)great_hall};
    struct gameState G, Gtest;


    printf ("TESTING baron():\n");
      memset(&G, 0, sizeof(struct gameState));   // clear the game state
      r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
      memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state
      Gtest = G;
      /******************************************************
      *** Test to check number of buy turns               ***
      *******************************************************/
      p = 0;
      pre = G.numBuys;
      baronRe(&G, p, 0);
      post = G.numBuys;

      #if (NOISY_TEST == 1)
            printf("Test player %d with %d # of buys before and %d after baron().\n", p, pre, post);
            printf("Expected result of number of buys is %d\n", pre+1);
      #endif

      char * str = "Number of buys test.";
      asserts += asserttrue(pre + 1 , post, str);

      /******************************************************
      *** Test case if player is not discarding estate    ***
      *******************************************************/
      memset(&G, 0, sizeof(struct gameState));   // clear the game state
      r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

      G.handCount[p] = 0; //set handCount to 0 for each player
      for (handCount = 0; handCount < maxHandCount; handCount++) {
        drawCard(p, &G); //draw cards for player
      }
      G.hand[p][maxHandCount-1] = estate; //last card in players hand is estate card

      printf("Player not discarding estate!\n");
      int supplyPre = 0;
      int supplyPost = 0;
      pre = 0;
      post = 0;  //check how many estates are in player hand

      pre = checkCards(G.discard[p], G.discardCount[p], estate);
      supplyPre = supplyCount(estate, &G);  //get supply count of estate cards

      Gtest = G;
      baronRe(&G, p, 0);

      post = checkCards(G.discard[p], G.discardCount[p], estate);
      supplyPost = supplyCount(estate, &G);  //get supply count of estate cards

      #if (NOISY_TEST == 1)
            printf("Test player %d with %d estate card(s) in discard pile before and %d after baron().\n", p, pre, post);
            printf("Expected result is estate of estate card(s) in discard pile is %d\n", pre+1);
            printf("%d estate card(s) in Supply before and %d after baron().\n", supplyPre, supplyPost);
            printf("Expected result is estate of estate card(s) in Supply is %d\n", supplyPre-1);
      #endif

      str = "Test adding estate from supply.\n";
      asserts += asserttrue(pre + 1 , post, str);
      str = "Test removing estate from supply.\n";
      asserts += asserttrue(supplyPre - 1 , supplyPost, str);

      /*************************************************************************
      *** Test case if player is discarding estate w/ estate in their hand   ***
      *************************************************************************/
      printf("Player is discarding estate with estate in their hand!\n");
      pre = 0;
      post = 0;
      int discardEstatePre = 0;
      int discardEstatePost = 0;
      int coinsPre = 0;
      int coinsPost = 0;

      pre  = checkCards(G.hand[p], G.handCount[p], estate);
      discardEstatePre = checkCards(G.discard[p], G.discardCount[p], estate);
      coinsPre = G.coins;

      baronRe(&G, p, 1);

      post  = checkCards(G.hand[p], G.handCount[p], estate);
      discardEstatePost = checkCards(G.discard[p], G.discardCount[p], estate);
      coinsPost = G.coins;

      #if (NOISY_TEST == 1)
            printf("Test player %d with %d estate card(s) in discard pile before and %d after baron().\n", p, discardEstatePre, discardEstatePost);
            printf("Expected result is estate of estate card(s) in discard pile is %d\n", discardEstatePre+1);
            printf("Test player %d with %d estate card(s) in hand before and %d after baron().\n", p, pre, post);
            printf("Expected result is estate of estate card(s) in hand is %d\n", pre-1);
            printf("Test player %d with %d coins before and %d after baron().\n", p, coinsPre, coinsPost);
            printf("Expected result is coins is %d\n", coinsPre+4);
      #endif

      str = "Test if estate is placed in discard.\n";
      asserts += asserttrue(discardEstatePre+1 , discardEstatePost, str);
      str = "Test if estate is removed from hand.\n";
      asserts += asserttrue(pre-1 , post, str);
      str = "Testing if coins are have been increased by 4\n";
      asserts += asserttrue(coinsPre +4 , coinsPost, str);

      /*************************************************************************
      *** Test case if player is discarding estate w/o estate in their hand  ***
      *************************************************************************/
      printf("Player is discarding estate without estate in their hand!\n");
      for(i = 0; i < Gtest.handCount[p]; i++) {
        Gtest.hand[p][i] = curse; //ensure no estates are in the players hand
      }
      Gtest.supplyCount[estate] = 5;
      pre = 0;
      post = 0;  //check how many estates are in player hand
      supplyPre = 0;
      supplyPost = 0;
      coinsPre = 0;
      coinsPost = 0;

      pre = checkCards(Gtest.discard[p], Gtest.discardCount[p], estate);
      supplyPre = Gtest.supplyCount[estate];  //get supply count of estate cards
      coinsPre = Gtest.coins;

      baronRe(&Gtest, p, 1);

      post = checkCards(Gtest.discard[p], Gtest.discardCount[p], estate);
      supplyPost = Gtest.supplyCount[estate];  //get supply count of estate cards
      coinsPost = Gtest.coins;

      #if (NOISY_TEST == 1)
            printf("Test player %d with %d estate card(s) in discard pile before and %d after baron().\n", p, pre, post);
            printf("Expected result is estate of estate card(s) in discard pile is %d\n", pre+1);
            printf("%d estate card(s) in Supply before and %d after baron().\n", supplyPre, supplyPost);
            printf("Expected result is estate of estate card(s) in Supply is %d\n", supplyPre-1);
      #endif

      str = "Test if estate is placed in discard\n";
      asserts += asserttrue(pre + 1, post, str);
      str = "Testing if coins are unchanged\n";
      asserts += asserttrue(coinsPre, coinsPost, str);
      str = "Testing if supply is decremented\n";
      asserts += asserttrue(supplyPre - 1, supplyPost, str);


  printf("# FAILED TESTS: %d\n", asserts);

  return 0;
}
