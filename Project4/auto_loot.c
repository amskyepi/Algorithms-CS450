#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//definition for data structure handling item data
struct item{
  int weight;
  int value;
  char *name;
};

//deinition for data structure handling maxVal function
struct answer{
  unsigned char items[128];
  int maxVal;
};

int itemCount; //serves as the number of items within data file
struct answer cache[1200]; //cache for maxVal function
struct item items[128];


struct answer maxVal(int capacity){
  if(cache[capacity].maxVal != 0){ //base case
    return(cache[capacity]);
  } 
  struct answer max = {0}; //struct used to return max value
  max.maxVal = 0;
    if(capacity <= 0){ //upon reaching capacity
      cache[capacity] = max;
      return(max); //return max value
    }
    for(int i = 0; i < itemCount; i++){ //checking all items
        if(items[i].weight <= capacity){ /* if items weight is less than capacity, continue */
            struct answer temp = maxVal(capacity - items[i].weight); /* recursive maxVal call */
            /* if item is considered for knapsack */
            if (temp.maxVal + items[i].value > max.maxVal){
                max=temp;
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
    sscanf(buffer, "%[^;];%d;%d", name, &value, &weight);
        items[itemCount].name = malloc(sizeof(char) *strlen(name));
        strcpy(items[itemCount].name, name);
        items[itemCount].value = value;
        items[itemCount].weight = weight; 
        itemCount++;
    }
    struct answer highV = maxVal(capacity);
    int max = highV.maxVal; 
    printf("Highest possible value=%d\n",max); 
    for (int c = 0; c < itemCount; c++){ 
        if (highV.items[c] != 0){
            printf("Item %d (%s): %d\n",c,items[c].name,highV.items[c]);
        }
    }
}
