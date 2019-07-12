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
    int checker = -5;
    int maxHandCount = 5;
    int k[10] = {(int)ambassador, (int)tribute, (int)minion, (int)gardens, (int)mine, (int)remodel, (int)smithy, (int)village, (int)baron, (int)great_hall};
    struct gameState G, Gtest;

    memset(&G, 0, sizeof(struct gameState));   // clear the game state
    memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

    for(j = 0; j < numPlayer; j++) {
      for(i = 0; i < maxHandCount; i++) {
        G.hand[j][i] = copper;   //set all cards in players hand to be curse cards
        G.deck[j][i] = copper;
      }
      G.handCount[j] = maxHandCount;
      G.deckCount[j] = maxHandCount;
    }


    printf ("TESTING mineRe():\n");
    for (p = 0; p < numPlayer; p++) {
      if(p == 0) {
        /****************************************************************************************
        *** Test to check if trashed card choice is not a treasure                         ***
        ****************************************************************************************/
        printf("Test to check if trashed card choice is not a treasure\n");
        memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state
        G.hand[p][maxHandCount -1] = mine;   //set last card in current player to be mine


        Gtest = G;  //set test gamestate to controll gameState

        checker = mineRe(&Gtest, p, mine, silver, maxHandCount-1);

        str = "Test if trashed card choice is not a treasure \n";
        asserts += asserttrue(-1, checker, str);

        /****************************************************************************************
        *** Test to check if gained card choice is not a treasure                         ***
        ****************************************************************************************/
        printf("Test to check if gained card choice is not a treasure\n");
        memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state

        Gtest = G;  //set test gamestate to controll gameState

        checker = mineRe(&Gtest, p, copper, mine, maxHandCount-1);

        str = "Test if gained card choice is not a treasure \n";
        asserts += asserttrue(-1, checker, str);

        /****************************************************************************************
        *** Test to check if gained card choice costs more than 3                       ***
        ****************************************************************************************/
        printf("Test to check if gained card choice costs more than 3\n");
        memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state

        Gtest = G;  //set test gamestate to controll gameState

        checker = mineRe(&Gtest, p, copper, gold, maxHandCount-1);

        str = "Test if gained card choice costs more than 3\n";
        asserts += asserttrue(-1, checker, str);

        /****************************************************************************************
        *** Test to check if valid gained card is placed in player's hand and mine card is discarded  ***
        ****************************************************************************************/
        pre = 0;
        post = 0;
        printf("Test to check if valid gained card is placed in player's hand and mine card is discarded\n");
        memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state

        Gtest = G;  //set test gamestate to controll gameState

        checker = mineRe(&Gtest, p, maxHandCount-2, silver, maxHandCount-1);

        pre = checkCards(G.hand[p], G.handCount[p], mine);
        post = checkCards(Gtest.hand[p], Gtest.handCount[p], mine);

        int discardPre = checkCards(G.discard[p], G.discardCount[p], mine);
        int discardPost = checkCards(Gtest.discard[p], Gtest.discardCount[p], mine);

        str = "Test to check if mine card is discarded\n";
        asserts += asserttrue(pre -1, post, str);
        asserts += asserttrue(discardPre +1, discardPost, str);

        pre = checkCards(G.hand[p], G.handCount[p], silver);
        post = checkCards(Gtest.hand[p], Gtest.handCount[p], silver);


        str = "Test to check if valid gained card is placed in player's hand\n";
        asserts += asserttrue(pre +1, post, str);
        str = "Test to check if function returned successfully\n";
        asserts += asserttrue(checker, 0, str);

      }
    }
    printf("# FAILED TESTS: %d\n", asserts);

    return 0;
  }
