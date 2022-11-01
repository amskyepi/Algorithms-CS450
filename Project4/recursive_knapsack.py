# Language: Python
# Project 4: Auto-loot (recursive)
# CS 450 
# Author: Amethyst Skye

import sys
import time
from functools import lru_cache

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
# names: list of item names
# weights: list of item weights
# values: list of item values
# capacity: bag/knapsack capacity
# return: list which contains the maximum value of knapsack, along with the items
def knapsack(names, weights, values, capacity):
    items = [(values[i], weights[i], names[i]) for i in range(0, len(values))]
    @lru_cache(maxsize=None)
    def bestvalue(i, j):
        # Return the value of the most valuable subsequence of the first
        # i elements in items whose weights sum to no more than j
        if j < 0:
            return float('-inf')
        if i == 0:
            return 0
        value, weight, names = items[i - 1]
        return max(bestvalue(i - 1, j), bestvalue(i - 1, j - weight) + value)

    j = capacity
    result = []
    for i in reversed(range(len(items))):
        if bestvalue(i + 1, j) != bestvalue(i, j):
            result.append(items[i])
            j -= items[i][1]
    result.reverse()
    return bestvalue(len(items), capacity), result

begin = time.time() 
capacity, name_list, weight_list, value_list = read_input()
pack = knapsack(name_list, weight_list, value_list, capacity)
final_value = pack[0]
final_weight = 0
for result in pack[1]:
    final_weight += result[1]
    print(str(result[2]) + ", " + str(result[1]) + ", " + str(result[0]))
print("final weight:", final_weight)
print("final value:", final_value)
end = time.time() 
print("time taken in microseconds:", (end - begin) * 1000000)