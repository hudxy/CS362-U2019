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
    int playerCountPre[numPlayer];
    int playerCountPost[numPlayer];
    int maxHandCount = 5;
    int k[10] = {(int)ambassador, (int)tribute, (int)minion, (int)gardens, (int)mine, (int)remodel, (int)smithy, (int)village, (int)baron, (int)great_hall};
    struct gameState G, Gtest;

    memset(&G, 0, sizeof(struct gameState));   // clear the game state
    memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

    for(j = 0; j < numPlayer; j++) {
      for(i = 0; i < maxHandCount; i++) {
        if(j == 0)
          G.hand[j][i] = curse;   //set all cards in current players hand to be curse cards
        else
          G.hand[j][i] = copper;  //set all cards in other players hand to be copper cards
      }
      G.handCount[j] = maxHandCount;
    }

    p = 0;
    printf ("TESTING ambassadorRe():\n");
      /****************************************************************************************
      *** Test to check when card is played and only one revelead card is in players hand   ***
      ****************************************************************************************/
      printf("Test to check when card is played and only one revelead card is in players hand\n");

      pre = 0;
      post = 0;
      memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state
      G.hand[p][maxHandCount -1] = ambassador;   //set last card in current player to be ambassador
      G.hand[p][maxHandCount -2] = baron;   //set second to last card in current player to be ambassador (to be revealed)

      Gtest = G;  //set test gamestate to controll gameState


      //int check = ambassadorRe(&Gtest, p, maxHandCount -2, 1, maxHandCount -1);
      int check = ambassadorCase(maxHandCount -2, 1, &Gtest, maxHandCount -1, p);
      //assert(check != -1);

      pre = checkCards(G.hand[p], G.handCount[p], baron);
      post = checkCards(Gtest.hand[p], Gtest.handCount[p], baron);

      str = "Test to check if revealed card was removed from hand\n";
      asserts += asserttrue(pre -1, post, str);

      str = "Test to check if copies of revealed card are return to supply\n";
      asserts += asserttrue(G.supplyCount[baron] + 1 - (numPlayer - 1), Gtest.supplyCount[baron], str);

      for(i = 0; i < numPlayer; i++) {
          playerCountPre[i] = checkCards(G.discard[i], G.discardCount[i], baron);
          playerCountPost[i] = checkCards(Gtest.discard[i], Gtest.discardCount[i], baron);
      }

      for(i = 0; i < numPlayer; i++) {
        if(i != p) {
          str = "Test to check if other player's recieved copy of revealed card\n";
          asserts += asserttrue(playerCountPre[i] + 1, playerCountPost[i], str);
        }
      }

      /************************************************************************************************************************
      *** Test to check when card is played and there are not 2 revelead card is in players hand when choice2 is set to 2   ***
      ************************************************************************************************************************/
      printf("Test to check when card is played and there are not 2 revelead card is in players hand when choice2 is set to 2\n");

      memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state
      Gtest = G;

      //check = ambassadorRe(&Gtest, p, maxHandCount -2, 2, maxHandCount -1);
      check = ambassadorCase(maxHandCount -2, 2, &Gtest, maxHandCount -1, p);
      str = "Function returns -1 when # of cards revealed does not match # of that card in player's hand!\n";
      asserts += asserttrue(check, -1, str);

      /****************************************************************************************
      *** Test to check when card is played and 2 revelead card is in players hand          ***
      ****************************************************************************************/
      printf("Test to check when card is played and 2 revelead card is in players hand \n");
      //G.hand[p][maxHandCount -3] = baron;   //set third to last card in current player to be ambassador (to be revealed)
      //2 barons total in player's hand
      memset(&Gtest, 0, sizeof(struct gameState));   // clear the game state

      Gtest = G;  //set test gamestate to controll gameState

      //check = ambassadorRe(&Gtest, p, 1, 2, maxHandCount -1);
      check = ambassadorCase(1, 2, &Gtest, maxHandCount -1, p);
      pre = 0;
      post = 0;
      memset(playerCountPre, 0, sizeof(playerCountPre));
      memset(playerCountPost, 0, sizeof(playerCountPost));

      pre = checkCards(G.hand[p], G.handCount[p], curse);
      post = checkCards(Gtest.hand[p], Gtest.handCount[p], curse);
      str = "Test to check if revealed cards(2) was removed from hand\n";
      asserts += asserttrue(pre -2, post, str);

      str = "Test to check if copies of revealed card are return to supply\n";
      asserts += asserttrue(G.supplyCount[curse] + 2 - (numPlayer - 1), Gtest.supplyCount[curse], str);


      for(i = 0; i < numPlayer; i++) {
          playerCountPre[i] = checkCards(G.discard[i], G.discardCount[i], curse);
          playerCountPost[i] = checkCards(Gtest.discard[i], Gtest.discardCount[i], curse);
      }

      for(i = 0; i < numPlayer; i++) {
        if(i != p) {
          str = "Test to check if other player's recieved copy of revealed card\n";
          asserts += asserttrue(playerCountPre[i] + 1, playerCountPost[i], str);
        }
      }

    printf("# FAILED TESTS: %d\n", asserts);

    return 0;
  }
