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
  int i, j, p, m;
  int numPlayer;
  int iter = 1;
  int maxHandCount = 5;
  struct gameState G, Gtest;

  for(i = 0; i < 20000; i++) {
    int tributeRevealedCards[] = {-2, -2};

    printf("Iteration: %d\n", iter);
    for(j = 0; j < sizeof(struct gameState); j++) {
      ((char*)&G)[i] = floor(random() * 256);
    }
    numPlayer = floor(random() % 4 );
    printf("numPlayer: %d\n", numPlayer);


    for(p = 0; p <= numPlayer; p++) {
      G.handCount[p] = floor(random() % maxHandCount) + 1;
      G.deckCount[p] = floor(random() % (MAX_DECK-1));
      G.discardCount[p] = floor(random() % maxHandCount);
      printf("player = %d, handCount: %d\n", p, G.handCount[p]);
      for(m = 0; m < G.handCount[p]; m++) {
        G.hand[p][m] = floor(random() % 4 +1);
      }
      for(m = 0; m < G.deckCount[p]; m++) {
        G.deck[p][m] = floor(random() % 4) +1;
        if(G.deck[p][m] == 3)
          G.deck[p][m] = baron;
      }
      for(m = 0; m < G.discardCount[p]; m++) {
        G.discard[p][m] = floor(random() % 4) +1;
        if(G.discard[p][m] == 3)
          G.discard[p][m] = baron;
      }
    }
    G.playedCardCount = 0;
    G.numPlayers = numPlayer +1;
    int nextPlayer = numPlayer + 1;
    if(nextPlayer >= G.numPlayers)
      nextPlayer = 0;

    Gtest = G;
    printf("Test\n" );
    //tributeRe(&Gtest, numPlayer, tributeRevealedCards);
    tributeCase(&Gtest, numPlayer, nextPlayer, tributeRevealedCards);



    //Oracles
    if(ORACLE) {
      if ((G.discardCount[nextPlayer] + G.deckCount[nextPlayer]) <= 1) {
        if (G.deckCount[nextPlayer] > 0){
          assert(G.deckCount[nextPlayer] - 1 == Gtest.deckCount[nextPlayer]);
          assert(G.deck[nextPlayer][G.deckCount[nextPlayer] - 1] == tributeRevealedCards[0]);
        }
        else if (G.discardCount[nextPlayer] > 0){
          assert(G.discardCount[nextPlayer] - 1 == Gtest.discardCount[nextPlayer]);
          assert(tributeRevealedCards[0] == G.discard[nextPlayer][G.discardCount[nextPlayer]-1]);
        }
        else {
          assert(G.deckCount[nextPlayer] == Gtest.deckCount[nextPlayer]);
          assert(G.discardCount[nextPlayer] == Gtest.discardCount[nextPlayer]);

        }
      } else {
        if(G.deckCount[nextPlayer] == 0){
          //assert(Gtest.discardCount[nextPlayer] <= 0);
          //assert(Gtest.discard[nextPlayer][Gtest.discardCount[nextPlayer] - 1] == -1);
          //assert(Gtest.deckCount[nextPlayer] >= 0);
          assert(tributeRevealedCards[0] && tributeRevealedCards[1]);
        }
      }
      if (tributeRevealedCards[1] == -1) {
        assert(G.playedCardCount+1 == Gtest.playedCardCount);
      }
      for (m = 0; m < 2; m ++){
        printf("tributeRevealedCards[m] = %d\n", tributeRevealedCards[m]);
        if (tributeRevealedCards[m] == copper || tributeRevealedCards[m] == silver || tributeRevealedCards[m] == gold){//Treasure cards
          assert(Gtest.coins >= G.coins + 2);
        }

        else if (tributeRevealedCards[m] == estate || tributeRevealedCards[m] == duchy || tributeRevealedCards[m] == province || tributeRevealedCards[m] == gardens || tributeRevealedCards[m] == great_hall){//Victory Card Found
          //assert(Gtest.handCount[numPlayer] >= G.handCount[numPlayer]+2 );
        }
        else{//Action Card
          assert(Gtest.numActions >= G.numActions +2);
        }
      }
    }

    iter++;
  }
  return 0;
}
