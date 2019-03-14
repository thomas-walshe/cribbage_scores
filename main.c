#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Card{
  char ID;
  char suit;
  int val;
};

struct Deck_52{
  struct Card contents[52];
};

struct Deck_46{
  struct Card contents[46];
};

struct Deck_40{
  struct Card contents[40];
};

struct Deck_39{
  struct Card contents[39];
};

struct Hand_4{
  struct Card contents[4];
};

struct Hand_6{
  struct Card contents[6];
};


/**Prototypes**/
void print_card(struct Card card);
void print_deck_52(struct Deck_52 deck);
void print_hand_6(struct Hand_6 hand);
void print_hand_4(struct Hand_4 hand);
void fill_deck_52(struct Deck_52* deck);
void deck_copy(struct Deck_52* dst_deck, struct Deck_52* src_deck, int index_map[52]);
void fill_shuffle_map(int shuffle_map[52]);
void deal_hand_6(struct Deck_52 deck, struct Deck_46* dst_deck, struct Hand_6* dst_hand);
void score_6(struct Hand_6 hand);
void hand_copy_4_6(struct Hand_6 src_hand, struct Hand_4* dst_hand, int index_map[4]);
/**Prototypes**/

int main(void){
  srand(time(NULL)); //Initiate random number generator
  int shuffle_map[52];
  struct Deck_52 plain_deck;
  struct Deck_52 shuffled_deck;
  fill_deck_52(&plain_deck);
  fill_shuffle_map(shuffle_map);
  deck_copy(&shuffled_deck, &plain_deck, shuffle_map);
  //print_deck_52(shuffled_deck);

  struct Deck_46 final_deck;
  struct Hand_6 hand_6;

  //Deal the hand of 6
  deal_hand_6(shuffled_deck, &final_deck, &hand_6);
  score_6(hand_6);
  
  return 0;
}

//Prints the value of the card
void print_card(struct Card card){
  printf("Card: %c%c\n", card.ID, card.suit);
}

//Prints out a 52 card deck
void print_deck_52(struct Deck_52 deck){
  for(int i = 0; i < 52; i++){
    print_card(deck.contents[i]);
  }
}

void print_hand_6(struct Hand_6 hand){
  for(int i = 0; i < 6; i++){
    print_card(hand.contents[i]);
  }
}

void print_hand_4(struct Hand_4 hand){
  for(int i = 0; i < 4; i++){
    print_card(hand.contents[i]);
  }
}

//Fills up a 52 card deck
void fill_deck_52(struct Deck_52* deck){
  char suit_current = 'H';
  int card_count = 0;
  int card_val = 0;
  char index_to_ID[14] = {'0', '1','2','3','4','5','6','7','8','9','T','J','Q','K'};
  for(int i = 1; i <= 13; i++){
    deck->contents[card_count].suit = suit_current;
    deck->contents[card_count].ID = index_to_ID[i];
    (i > 10) ? (card_val = 10) : (card_val = i);
    deck->contents[card_count].val = card_val;
    card_count++;
  }

  suit_current = 'D';
  for(int i = 1; i <= 13; i++){
    deck->contents[card_count].suit = suit_current;
    deck->contents[card_count].ID = index_to_ID[i];
    (i > 10) ? (card_val = 10) : (card_val = i);
    deck->contents[card_count].val = card_val;
    card_count++;
  }

  suit_current = 'C';
  for(int i = 1; i <= 13; i++){
    deck->contents[card_count].suit = suit_current;
    deck->contents[card_count].ID = index_to_ID[i];
    (i > 10) ? (card_val = 10) : (card_val = i);
    deck->contents[card_count].val = card_val;
    card_count++;
  }

  suit_current = 'S';
  for(int i = 1; i <= 13; i++){
    deck->contents[card_count].suit = suit_current;
    deck->contents[card_count].ID = index_to_ID[i];
    (i > 10) ? (card_val = 10) : (card_val = i);
    deck->contents[card_count].val = card_val;
    card_count++;
  }

}

//Used to produce the shuffle map randomly
void fill_shuffle_map(int shuffle_map[52]){
  int MAX_ROLLS = 10000;
  int swap_s_index = 0;
  int swap_d_index = 0;
  int temp_s = 0;
  int temp_d = 0;

  for(int i = 0; i < 52; i++){
    shuffle_map[i] = i;
  }

  for(int i = 0; i < MAX_ROLLS; i++){
    swap_s_index = rand() % 52;
    swap_d_index = rand() % 52;
    temp_s = shuffle_map[swap_s_index];
    temp_d = shuffle_map[swap_d_index];
    shuffle_map[swap_d_index] = temp_s;
    shuffle_map[swap_s_index] = temp_d;
  }
}


//Used to copy across a deck, and can randomise
void deck_copy(struct Deck_52* dst_deck, struct Deck_52* src_deck, int index_map[52]){
  for(int i = 0; i < 52; i++){
    dst_deck->contents[index_map[i]] = src_deck->contents[i];
  }
}

//Deal a hand of 6 from the a deck
void deal_hand_6(struct Deck_52 deck, struct Deck_46* dst_deck, struct Hand_6* dst_hand){
  for(int i = 0; i < 6; i++){
    dst_hand->contents[i] = deck.contents[i];
  }

  for(int i = 6; i < 52; i++){
    dst_deck->contents[i] = deck.contents[i];
  }
}


//Calculate all possible scores from hand of 6 
void score_6(struct Hand_6 hand){
  //Now split into all possible hands of 4, there should be 15 of them
  //I have not indexed from 0 here! Needs changing (and in hand_copy_4_6)
  int combinations[15][4] = { {1,2,3,4}, {1,2,3,5}, {1,2,3,6},	\
			      {1,2,4,5}, {1,2,4,6}, {1,2,5,6},	\
			      {1,3,4,5}, {1,3,4,6}, {1,3,5,6},	\
			      {1,4,5,6}, {2,3,4,5}, {2,3,4,6},	\
			      {2,3,5,6}, {2,4,5,6}, {3,4,5,6} };
  int index_array[4] = {0,0,0,0};
  struct Hand_4 sub_hand;

  print_hand_6(hand);
  printf("\n");
  
  for(int hand_index = 0; hand_index < 15; hand_index++){
    for(int i = 0; i < 4; i++){
      index_array[i] = combinations[hand_index][i];
    }
    hand_copy_4_6(hand, &sub_hand, index_array);
    print_hand_4(sub_hand);
    printf("\n");
  }
}

void hand_copy_4_6(struct Hand_6 src_hand, struct Hand_4* dst_hand, int index_map[4]){
  for(int i = 0; i < 4; i++){
    dst_hand->contents[i] = src_hand.contents[index_map[i] - 1];
  }
}

//Now I need to write the score_4 function, this may call a score 3 and score 2 function
//or something like that, to check for doubles, triples ECT....
//This needs to be planned out well on paper
//Flowchart with all scoring routes
