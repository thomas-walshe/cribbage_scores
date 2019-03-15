#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SHOW_CARDS 1
#define SHOW_SCORE 1

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
int ID_to_index(char index);
void sort_4(int value_array[4], int index_array[4]);
void sort_3(int value_array[3], int index_array[3]);

//Scoring functions
int find_15_4(struct Hand_4 hand);
int find_multi_4(struct Hand_4 hand);
int find_run_4(struct Hand_4 hand);
int find_flush_4(struct Hand_4 hand);
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

int ID_to_index(char index){
  int val = 0;
  (index == '1') ? (val = 1) : 0;
  (index == '2') ? (val = 2) : 0;
  (index == '3') ? (val = 3) : 0;
  (index == '4') ? (val = 4) : 0;
  (index == '5') ? (val = 5) : 0;
  (index == '6') ? (val = 6) : 0;
  (index == '7') ? (val = 7) : 0;
  (index == '8') ? (val = 8) : 0;
  (index == '9') ? (val = 9) : 0;
  (index == 'T') ? (val = 10) : 0;
  (index == 'J') ? (val = 11) : 0;
  (index == 'Q') ? (val = 12) : 0;
  (index == 'K') ? (val = 13) : 0;
  return val;
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
  int points_hand = 0;

  (SHOW_CARDS) ? (print_hand_6(hand)) : 0;
  printf("\n");
  
  for(int hand_index = 0; hand_index < 15; hand_index++){
    for(int i = 0; i < 4; i++){
      index_array[i] = combinations[hand_index][i];
    }
    hand_copy_4_6(hand, &sub_hand, index_array);
    (SHOW_CARDS) ? (print_hand_4(sub_hand)) : 0;
    points_hand = find_15_4(sub_hand);
    points_hand += find_multi_4(sub_hand);
    points_hand += find_run_4(sub_hand);
    points_hand += find_flush_4(sub_hand);
    if(SHOW_SCORE){
      printf("->%d Points\n\n", points_hand);
    } else {
      printf("%d\n", points_hand);
    }
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

int find_15_4(struct Hand_4 hand){
  //This checks all the subsets of minimum size 2
  int combinations_2[6][2] = { {1,2}, {1,3}, {1,4},	\
			       {2,3}, {2,4}, {3,4} };

  int combinations_3[4][3] = { {1,2,3}, {1,2,4},	\
			       {1,3,4}, {2,3,4} };

  int combinations_4[4] = {1,2,3,4};

  int temp_sum = 0;
  int total_points = 0;
  for(int i = 0; i < 4; i++){
    temp_sum += hand.contents[combinations_4[i] - 1].val;
  }

  if(temp_sum == 15){
    (SHOW_SCORE) ? (printf("All cards sum to 15\n")) : 0;
    return 2;
  } else {
    temp_sum = 0;
  }

  for(int j = 0; j < 4; j++){
    for(int i = 0; i < 3; i++){
      temp_sum += hand.contents[combinations_3[j][i] - 1].val;
    }
    if(temp_sum == 15){
      total_points += 2;
      (SHOW_SCORE) ? (printf("15 found from: ")) : 0;
      for(int i = 0; i < 3; i++){
	(SHOW_SCORE) ? (printf(" %c%c", hand.contents[combinations_3[j][i] - 1].ID, hand.contents[combinations_3[j][i] - 1].suit)) : 0;
      }
      (SHOW_SCORE) ? (printf("\n")) : 0;
    }
    temp_sum = 0;
  }

  for(int j = 0; j < 6; j++){
    for(int i = 0; i < 2; i++){
      temp_sum += hand.contents[combinations_2[j][i] - 1].val;
    }
    if(temp_sum == 15){
      total_points += 2;
      (SHOW_SCORE) ? (printf("15 found from: ")) : 0;
      for(int i = 0; i < 2; i++){
	(SHOW_SCORE) ? (printf(" %c%c", hand.contents[combinations_2[j][i] - 1].ID, hand.contents[combinations_2[j][i] - 1].suit)) : 0;
      }
      (SHOW_SCORE) ? (printf("\n")) : 0;
    }
    temp_sum = 0;
  }

  return total_points;
}

int find_multi_4(struct Hand_4 hand){
  int total_points = 0;
  int combinations_2[6][2] = { {1,2}, {1,3}, {1,4},	\
			       {2,3}, {2,4}, {3,4} };

  char ID_1;
  char ID_2;
  
  for(int j = 0; j < 6; j++){
    ID_1 = hand.contents[combinations_2[j][0] - 1].ID;
    ID_2 = hand.contents[combinations_2[j][1] - 1].ID;

    if(ID_1 == ID_2){
      total_points += 2;
      (SHOW_SCORE) ? (printf("Pair found from: ")) : 0;
      for(int i = 0; i < 2; i++){
	(SHOW_SCORE) ? (printf(" %c%c", hand.contents[combinations_2[j][i] - 1].ID, hand.contents[combinations_2[j][i] - 1].suit)) : 0;
      }
      (SHOW_SCORE) ? (printf("\n")) : 0;
    }
  }
  return total_points;
}

int find_run_4(struct Hand_4 hand){
  int value_array[4];
  int index_array[4];
  int value_array_3[3];
  int index_array_3[3];
  int total_points = 0;
  for(int i = 0; i < 4; i++){
    value_array[i] = ID_to_index(hand.contents[i].ID);
    index_array[i] = i;
  }

  sort_4(value_array, index_array);
  
  if(value_array[1] == (value_array[0] + 1)){
    if(value_array[2] == (value_array[1] + 1)){
      if(value_array[3] == (value_array[2] + 1)){
	(SHOW_SCORE) ? (printf("Run of 4\n")) : 0;
	return 4;
      }
    }
  }

  //Now check all possible runs of 3
  int combinations_3[4][3] = { {1,2,3}, {1,2,4},	\
			       {1,3,4}, {2,3,4} };

  for(int j = 0; j < 4; j++){
    for(int i = 0; i < 3; i++){
      value_array_3[i] = ID_to_index(hand.contents[combinations_3[j][i] - 1].ID);
      index_array_3[i] = combinations_3[j][i] - 1;
    }
    sort_3(value_array_3, index_array_3);
    if(value_array_3[1] == (value_array_3[0] + 1)){
      if(value_array_3[2] == (value_array_3[1] + 1)){
	(SHOW_SCORE) ? (printf("Run of 3:")) : 0;
	for(int x = 0; x < 3; x++){
	  (SHOW_SCORE) ? (printf(" %c%c", hand.contents[index_array_3[x]].ID, hand.contents[index_array_3[x]].suit)) : 0;
	}
	(SHOW_SCORE) ? (printf("\n")) : 0;
	total_points += 3;
      }
    }
  }
  
  return total_points;
}

//Sort the array and preserve original indexes
void sort_4(int value_array[4], int index_array[4]){
  int changes = 1;
  int temp = 0;
  while(changes){
    changes = 0;
    if(value_array[0] > value_array[1]){
      changes++;
      temp = value_array[0];
      value_array[0] = value_array[1];
      value_array[1] = temp;

      temp = index_array[0];
      index_array[0] = index_array[1];
      index_array[1] = temp;
    }
    if(value_array[1] > value_array[2]){
      changes++;
      temp = value_array[1];
      value_array[1] = value_array[2];
      value_array[2] = temp;

      temp = index_array[1];
      index_array[1] = index_array[2];
      index_array[2] = temp;
    }
    if(value_array[2] > value_array[3]){
      changes++;
      temp = value_array[2];
      value_array[2] = value_array[3];
      value_array[3] = temp;

      temp = index_array[2];
      index_array[2] = index_array[3];
      index_array[3] = temp;
    }
  }
}

void sort_3(int value_array[3], int index_array[3]){
  int changes = 1;
  int temp = 0;
  while(changes){
    changes = 0;
    if(value_array[0] > value_array[1]){
      changes++;
      temp = value_array[0];
      value_array[0] = value_array[1];
      value_array[1] = temp;

      temp = index_array[0];
      index_array[0] = index_array[1];
      index_array[1] = temp;
    }
    if(value_array[1] > value_array[2]){
      changes++;
      temp = value_array[1];
      value_array[1] = value_array[2];
      value_array[2] = temp;

      temp = index_array[1];
      index_array[1] = index_array[2];
      index_array[2] = temp;
    }
  }
}

int find_flush_4(struct Hand_4 hand){
  char base_suit = hand.contents[0].suit;
  if(base_suit == hand.contents[1].suit){
    if(hand.contents[1].suit == hand.contents[2].suit){
      if(hand.contents[2].suit == hand.contents[3].suit){
	if(SHOW_SCORE){
	  printf("Flush of ");
	  (base_suit == 'H') ? (printf("Hearts\n")) : 0;
	  (base_suit == 'C') ? (printf("Clubs\n")) : 0;
	  (base_suit == 'D') ? (printf("Diamonds\n")) : 0;
	  (base_suit == 'S') ? (printf("Spades\n")) : 0;
	}
	return 4;
      }
    }
  }
  return 0;
}
