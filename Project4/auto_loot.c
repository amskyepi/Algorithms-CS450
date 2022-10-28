#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Purpose: structure for holding information associated with loot item
 * int weight: weight of item
 * int value: value of item
 * char *name: name of item */
typedef struct item{
  int weight;
  int value;
  char *name;
}ITEM;

typedef struct answer{
  unsigned char items[128];
  int maxVal;
}ANSWER;

int itemCount;
ANSWER cache[1200];
ITEM items[128];

ANSWER maxVal(int capacity){
  /* Base Case */
  if(cache[capacity].maxVal != 0){ 
    return(cache[capacity]);
  } 
  ANSWER max = {0};
  max.maxVal = 0;
    if(capacity <= 0){
      cache[capacity] = max;
      return(max);
    }
    for(int i = 0; i < itemCount; i++){
    /* if items weight is less than capacity, continue */
      if(items[i].weight <= capacity){ 
        /* recursive maxVal call */
        ANSWER temp = maxVal(capacity - items[i].weight); 
        /* if item is considered for knapsack */
        if (temp.maxVal + items[i].value > max.maxVal){
          max = temp;
          max.maxVal = temp.maxVal + items[i].value;
          max.items[i]++;
        }
      }
    }
    cache[capacity] = max;
    return (max);
}

int main(int argc, char *argv[]){
    int capacity, weight, value;
    char buffer[200], name[128];
    itemCount = 0;

    /* Get bag capacity */
    fgets(buffer, 200, stdin);
    sscanf(buffer,"%d", &capacity);

    /* Parse input */
    while (fgets(buffer, 200, stdin) != NULL){
    sscanf(buffer, "%[^;];%d;%d", name, &weight, &value);
      items[itemCount].name = malloc(sizeof(char) *strlen(name));
      strcpy(items[itemCount].name, name);
      items[itemCount].weight = weight;
      items[itemCount].value = value;
      itemCount++;
    }
    ANSWER highV = maxVal(capacity);
    int max = highV.maxVal; 
    printf("Highest possible value=%d\n",max); 
    for (int i = 0; i < itemCount; i++){ 
      if (highV.items[i] != 0){
        printf("Item %d (%s): %d\n",i,items[i].name,highV.items[i]);
      }
    }
}
