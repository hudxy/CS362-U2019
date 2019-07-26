#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int ORACLE = 0;

int main() {
  int i, j, p, m;
  int numPlayer;
  int choice1, choice2;
  int iter = 1;
  int maxHandCount = 5;
  int k[10] = {(int)ambassador, (int)tribute, (int)minion, (int)gardens, (int)mine, (int)remodel, (int)smithy, (int)village, (int)baron, (int)great_hall};
  struct gameState G, Gtest;

  for(i = 0; i < 5000; i++) {
    printf("Iteration: %d\n", iter);
    for(j = 0; j < sizeof(struct gameState); j++) {
      ((char*)&G)[i] = floor(random() * 256);
    }
    numPlayer = random() % 4 ;
    printf("numPlayer: %d\n", numPlayer);
    choice1 = random() % 2;
    printf("choice1: %d\n", choice1);
    choice2 = random() % 2;
    printf("choice2: %d\n", choice2);

    for(p = 0; p <= numPlayer; p++) {
      G.handCount[p] = random() % maxHandCount +1;
      G.deckCount[p] = random() % (MAX_DECK-1) +1;

      printf("player = %d, handCount: %d\n", p, G.handCount[p]);
      for(m = 0; m < G.handCount[p]; m++) {
        G.hand[p][m] = random() % 4 +1;
      }
      for(m = 0; m < G.deckCount[p]; m++) {
        G.deck[p][m] = random() % 4 +1;
      }
    }
    G.hand[numPlayer][G.handCount[numPlayer]-1] = minion;
    G.whoseTurn = numPlayer;
    G.playedCardCount = 0;
    G.numPlayers = numPlayer +1;
    Gtest = G;
    printf("Test\n" );
    minionRe(&Gtest, numPlayer, choice1, choice2, Gtest.handCount[numPlayer]-1);

    //Oracles
    if(ORACLE) {
      assert(Gtest.numActions = G.numActions+1);  //check that numActions is incremented each time
      assert(Gtest.hand[numPlayer][Gtest.handCount[numPlayer]-1] != minion);
      assert(G.playedCardCount < Gtest.playedCardCount);

      //oracles when there is an estate card in hand and the choices to discard an estate is selected
      if(choice1) {
        assert(Gtest.coins == G.coins+2);
        assert(G.playedCardCount+1 == Gtest.playedCardCount);
        assert(Gtest.playedCards[Gtest.playedCardCount-1] == minion);
      }
      //oracles when there is not an estate in hand and the choice to discard an estate is set
      else if(choice1 == 0 && choice2) {
        assert(Gtest.handCount[numPlayer] == 4);

        assert(Gtest.coins == G.coins);
        for(p = 0; p < numPlayer; p++) {
          if(G.handCount[p] > 4) {
            assert(Gtest.handCount[p] == 4); //BUG found   {discardCard(handPos, i, state, 0);} discards only handPos for each player with handcount > 4 DOes not discard entire hand!
          }
        }
      }
      else {
        assert(Gtest.coins == G.coins);
        assert(G.playedCardCount+1 == Gtest.playedCardCount);
        assert(Gtest.playedCards[Gtest.playedCardCount-1] == minion);
        for(p = 0; p <= numPlayer; p++) {
          printf("p = %d\n", p);
          if(p != numPlayer) {
            printf(" Gtest.handCount[p] = %d G.handCount[p] = %d\n", Gtest.handCount[p] , G.handCount[p]);

            assert(Gtest.handCount[p] == G.handCount[p]);
            assert(Gtest.discardCount[p] == G.discardCount[p]);
          }
        }
      }
    }
    iter++;
  }
  return 0;
}
