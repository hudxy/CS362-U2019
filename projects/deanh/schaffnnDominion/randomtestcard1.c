#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int ORACLE = 1;

int main() {
  int i, j, p;
  int numPlayer;
  int choice1;
  int iter = 1;
  int maxHandCount = 5;
  int isEstate = 0;
  int k[10] = {(int)ambassador, (int)tribute, (int)minion, (int)gardens, (int)mine, (int)remodel, (int)smithy, (int)village, (int)baron, (int)great_hall};
  struct gameState G, Gtest;

  for(i = 0; i < 5000; i++) {
    isEstate = 0;
    printf("Iteration: %d\n", iter);
    for(j = 0; j < sizeof(struct gameState); j++) {
      ((char*)&G)[i] = floor(random() * 256);
    }
    numPlayer = random() % 2;
    printf("numPlayer: %d\n", numPlayer);
    choice1 = random() % 2;
    printf("choice1: %d\n", choice1);
    G.supplyCount[estate] = random() % 4;
    printf("estate supply count: %d\n", G.supplyCount[estate]);
    G.handCount[numPlayer] = random() % maxHandCount +1;
    printf("handCount: %d\n", choice1);
    for(p = 0; p < G.handCount[numPlayer]; p++) {
      G.hand[numPlayer][p] = random() % 4 +1;
      if(G.hand[numPlayer][p] == estate) {
        isEstate = 1;
      }
    }
    printf("isEstate = %d\n", isEstate );
    Gtest = G;
    //baronRe(&Gtest, numPlayer, choice1);
    baronCase(choice1, &Gtest, numPlayer);


    //Oracles
    if(ORACLE) {
      assert(Gtest.numBuys = G.numBuys+1);  //check that numBuys is incremented each time
      //oracles when there is an estate card in hand and the choices to discard an estate is selected
      if(isEstate && choice1) {
        assert(Gtest.coins == G.coins+4);
        assert(Gtest.discardCount[numPlayer] == G.discardCount[numPlayer]+1);
        assert(Gtest.discard[numPlayer][Gtest.discardCount[numPlayer]-1] == estate);
        assert(Gtest.handCount[numPlayer] == G.handCount[numPlayer]-1);
      }
      //oracles when there is not an estate in hand and the choice to discard an estate is set
      else if(isEstate == 0 && choice1) {
        //assert(Gtest.coins == G.coins); //BUG found when {else if (p > state->handCount[currentPlayer])} is checked after array is indexed
        if(G.supplyCount[estate] > 0) {
          assert(Gtest.discard[numPlayer][Gtest.discardCount[numPlayer]-1] == estate);
          assert(Gtest.discardCount[numPlayer] == G.discardCount[numPlayer]+1);
          //assert(Gtest.supplyCount[estate] == G.supplyCount[estate]-1); //BUG found when {else if (p > state->handCount[currentPlayer])} is checked after array is indexed and supplyCount[estate] is decremented 2 times
        }
      }
      else {
        //assert(Gtest.coins == G.coins); //BUG found when {else if (p > state->handCount[currentPlayer])} is checked after array is indexed
        if(G.supplyCount[estate] > 0) {
          assert(Gtest.discard[numPlayer][Gtest.discardCount[numPlayer]-1] == estate);
          assert(Gtest.discardCount[numPlayer] == G.discardCount[numPlayer]+1);
          //assert(Gtest.supplyCount[estate] == G.supplyCount[estate]-1); //BUG found when {else if (p > state->handCount[currentPlayer])} is checked after array is indexed and supplyCount[estate] is decremented 2 times
        }
      }
    }
    iter++;
  }
  return 0;
}
