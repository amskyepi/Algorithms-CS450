# Python
# Project 4: Auto-loot
# CS 450 
# Author: Amethyst Skye

import sys
import time

# Purpose: Reads input from stdin and assigns data to variables
# return: capacity, name_list, weight_list, value_list
def read_input():
    # Arrays for holding item information
    name_list = []
    weight_list = []
    value_list = []
    
    # Read capacity from stdin
    capacity = int(input())

    # Append item information from stdin to appropriate array
    for line in sys.stdin:
        name,weight,value = line.rstrip().split(";")
        name_list.append(name)
        weight_list.append(int(weight))
        value_list.append(int(value))
    return(capacity, name_list, weight_list, value_list)

# Purpose: Looks at all possible loot combinations and decides items
# which are the most valuable given their weight and the total capacity
# of the bag/knapsack.
# capacity: bag/knapsack capacity
# name: list of item names
# weight: list of item weights
# value: list of item values
# return: nothing, but prints out a list of items which are most desirable
# along with the final weight and value of bag
def auto_loot(capacity, name, weight, value):
    n_items = len(value)
    # table of combinations
    sack = [[0 for i in range(capacity + 1)]
            for j in range(n_items + 1)]
             
    # Build sack[][] table
    for i in range(n_items + 1):
        for j in range(capacity + 1):
            if i == 0 or j == 0:
                sack[i][j] = 0
            elif weight[i - 1] <= j:
                sack[i][j] = max(value[i - 1] + sack[i - 1][j - weight[i - 1]], sack[i - 1][j])
            else:
                sack[i][j] = sack[i - 1][j]
 
    items_list = []
    final_value = sack[n_items][capacity]
    final_weight = 0
    j = capacity
    
    for i in range(n_items, 0, -1):
        if final_value <= 0:
            break

        if final_value == sack[i - 1][j]:
            continue
        else:
 
            # This item is included in the bag
            items_list.append(name[i - 1] + ", " + str(weight[i - 1]) + ", " + str(value[i - 1]))
            final_weight += weight[i - 1]
            final_value -= value[i - 1]
            j -= weight[i - 1]
    
    items_list.reverse()    
    for i in items_list:
        print(i)   
    print("final weight:", final_weight)
    final_value = sack[n_items][capacity]
    print("final value:",final_value)
   
begin = time.time() 
capacity, name_list, weight_list, value_list = read_input()
auto_loot(capacity, name_list, weight_list, value_list)
end = time.time() 
print("time taken in microseconds:", (end - begin) * 1000000)