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
    int checker = -5;
    int maxHandCount = 5;
    int k[10] = {(int)ambassador, (int)tribute, (int)minion, (int)gardens, (int)mine, (int)remodel, (int)smithy, (int)village, (int)baron, (int)great_hall};
    struct gameState G, Gtest;

    memset(&G, 0, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

    /***********************************************
    *** Test to check MAX/MIN number of players  ***
    ************************************************/
    printf("Test to check if valid gained card is placed in player's hand and mine card is discarded\n");

    //check max
    memset(&G, 0, sizeof(struct gameState));   // clear the game state
    r = initializeGame(MAX_PLAYERS +1, k, seed, &G); // initialize a new game
    str = "Test Max Players\n";
    asserts += asserttrue(-1, r, str);

    //check min
    memset(&G, 0, sizeof(struct gameState));   // clear the game state
    r = initializeGame(1, k, seed, &G); // initialize a new game
    str = "Test Min Players\n";
    asserts += asserttrue(-1, r, str);

    /***********************************************
    *** Test to check duplicate kingdom cards  ****
    ************************************************/
    int kt[10] = {(int)mine, (int)tribute, (int)minion, (int)gardens, (int)mine, (int)remodel, (int)smithy, (int)village, (int)baron, (int)great_hall};

    memset(&G, 0, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, kt, seed, &G); // initialize a new game
    str = "Test duplicate kingdomcards\n";
    asserts += asserttrue(-1, r, str);

    /***********************************************
    *** Test to check supply                    ****
    ************************************************/
    printf("Test to check supply\n");
    str = "";
    memset(&G, 0, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

    //set number of Curse cards
    if (numPlayer == 2)
      {
        asserts += asserttrue(G.supplyCount[curse], 10, str);
      }
    else if (numPlayer == 3)
      {
        asserts += asserttrue(G.supplyCount[curse], 20, str);
      }
    else
      {
        asserts += asserttrue(G.supplyCount[curse], 30, str);
      }

    //check number of Victory cards
    if (numPlayer == 2)
      {
        asserts += asserttrue(G.supplyCount[estate], 8, str);
        asserts += asserttrue(G.supplyCount[duchy], 8, str);
        asserts += asserttrue(G.supplyCount[province], 8, str);

      }
    else
      {
        asserts += asserttrue(G.supplyCount[estate], 12, str);
        asserts += asserttrue(G.supplyCount[duchy], 12, str);
        asserts += asserttrue(G.supplyCount[province], 12, str);

      }

    //check number of Treasure cards
    asserts += asserttrue(G.supplyCount[copper], 60 - (7 * numPlayer), str);
    asserts += asserttrue(G.supplyCount[silver], 40, str);
    asserts += asserttrue(G.supplyCount[gold], 30, str);

    //check kingdom cards
    for (i = adventurer; i <= treasure_map; i++)       	//loop all cards
    {
      for (j = 0; j < 10; j++)           		//loop chosen cards
      {
        if (k[j] == i)
        {
          //check if card is a 'Victory' Kingdom card
          if (k[j] == great_hall || k[j] == gardens)
          {
            if (numPlayer == 2){
              asserts += asserttrue(G.supplyCount[i], 8, str);
            }
            else{ asserts += asserttrue(G.supplyCount[i], 12, str); }
          }
          else
          {
            asserts += asserttrue(G.supplyCount[i], 10, str);
          }
          break;
        }
        else    //card is not in the set choosen for the game
         {
           //state->supplyCount[i] = -1;
           asserts += asserttrue(G.supplyCount[i], -1, str);
         }
      }

    }

    printf("# FAILED TESTS: %d\n", asserts);

    return 0;
    }
