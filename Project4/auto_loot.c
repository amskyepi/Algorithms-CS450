/* Language: C
 * Project 4: Auto-loot
 * CS 450 
 * Author: Amethyst Skye */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* Purpose: structure for holding information associated with loot item
 * int weight: weight of item
 * int value: value of item
 * char *name: name of item */
typedef struct item{
  int weight;
  int value;
  char *name;
}ITEM;

ITEM items[128];

/* Purpose: Simple function that finds the maximum of two values */
int max(int a, int b){
    if (a > b)
        return (a);
    return(b);
}

/* Purpose: Using an iterative approach, we find the best combination of items
 * to keep in players inventory.
 * int capacity: capacity of bag
 * int item_count: number of items to choose from
 * return: Nothing, but prints contents of optimal knapsack, final weight
 * and final value. */
void auto_loot(int capacity, int item_count){
  int knapsack[item_count + 1][capacity + 1];
  int best_item_count = 0;

  /* Build table with potential knapsack contents */
  for (int i = 0; i < item_count + 1; i++){
    for (int j = 0; j < capacity + 1; j++){
      if (i == 0 || j == 0)
        knapsack[i][j] = 0;
      else if (items[i - 1].weight <= j)
        knapsack[i][j] = max(items[i - 1].value + 
          knapsack[i - 1][j - items[i - 1].weight], 
          knapsack[i - 1][j]);
      else{
        knapsack[i][j] = knapsack[i - 1][j];
        best_item_count++;
      }
    }
  }

  int max_value = knapsack[item_count][capacity],
      max_cap = capacity, 
      final_weight = 0, 
      final_value = knapsack[item_count][capacity];
  ITEM final_knapsack_contents[best_item_count];
  int knap_count = 0;
  
  /* Store results in an array */
  for (int i = item_count; i > 0 && max_value > 0; i--){
    if (max_value == knapsack[i - 1][max_cap])
      continue;
    else{
      final_knapsack_contents[knap_count].name = items[i - 1].name;
      final_knapsack_contents[knap_count].weight = items[i - 1].weight;
      final_knapsack_contents[knap_count].value = items[i - 1].value;
      knap_count++;
      
      max_cap -= items[i - 1].weight;
      max_value -= items[i - 1].value;
      final_weight += items[i - 1].weight;
    }
  }
  for (int i = knap_count; i > 0; i--){
    printf("%s, %d, %d\n", 
            final_knapsack_contents[i - 1].name,
            final_knapsack_contents[i - 1].weight,
            final_knapsack_contents[i - 1].value);
  }
  printf("final weight: %d\n", final_weight);
  printf("final value: %d\n", final_value);
  return;
}

/* Purpose: Reads input from stdin, and uses auto_loot function to print
 * contents of the most optimal loot based on the players current capacity.
 * int argc: number of command line arguments provided by user
 * char* argv[]: command line arguments provided
 * return: 0 on success */
int main(int argc, char* argv[]){
    int capacity, weight, value;
    char buffer[200], name[128];
    int itemCount = 0;

    /* Get bag capacity */
    clock_t start = clock();
    fgets(buffer, 200, stdin);
    sscanf(buffer,"%d", &capacity);

    /* Parse input */
    while (fgets(buffer, 200, stdin) != NULL){
    sscanf(buffer, "%[^;];%d;%d", name, &weight, &value);
      items[itemCount].name = malloc(sizeof(char) * strlen(name) + 1);
      strcpy(items[itemCount].name, name);
      items[itemCount].weight = weight;
      items[itemCount].value = value;
      itemCount++;
    }
    auto_loot(capacity, itemCount);
    clock_t end = clock();
    double total_time = (((double) (end - start)) / CLOCKS_PER_SEC) * 1000000;
    printf("time taken in microseconds: %lf\n", total_time);

    for (int i = 0; i < itemCount; i++)
      free(items[i].name);
    return(0);
}
