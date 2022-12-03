# Project 5: Tactical Path-Finding
# Author: Amethyst Skye
# Language: Python

import sys
import numpy as np
import heapq
import time

INF = 9999999
cost_dict = {
    '.': 1, # Road 
    ',': 2, # Grass
    'o': 3, # Rocks
    '=': 50, # Wall
    '0': INF, # Source
    '1': INF, # Friendly player
    '2': 0,
    '3': INF # Enemy player
    }

# All directions checked during path finding
directions = [(0,-1), (0,1), (-1,0), (1,0)]
directions_diag = [(-1,-1), (-1,1), (1,-1), (1,1)]

# Game board stored in 2D array
grid_data = np.loadtxt(sys.stdin, dtype=np.str_)

# m x n gameboard
m_dim = len(grid_data)
n_dim = len(grid_data[0])

# Dictionary to store unique key for each pair of coordinates on board
coord_dict = {} 
count = 0
for i in range(m_dim):
    for j in range(n_dim):
        coord_dict[count] = (i, j)
        count += 1

# So we can access keys according to coordinate pair
val_list = list(coord_dict.values())

# Dictionary to store unique key for each tile on board
value_dict = {} 
count = 0
for i in range(m_dim):
    for j in range(n_dim):
        value_dict[count] = grid_data[i][j]
        count += 1

# Simple printing function
def print_grid(grid, cost):
    for i in grid:
        for j in i:
            print(j, end = ' ')
        print()
    print("Total cost:", cost)
    return

# Takes input file and creates grid from it
def file_to_grid(file_name):
    with open(file_name) as file:
        grid = [i.split() for i in file.readlines()]
    return(grid)

# Renames values in our grid to the cost associated with it
def replace_values(grid, dict):
    for i, row in enumerate(grid):
        grid[i] = [dict[x] for x in row]
    return(grid)

# Find position of starting point
def find_item(grid, item):
    return (tuple(np.argwhere(grid == item).flatten()))

# Determine if we can make a move at (x,y)
def valid_position(x, y):
    # If negative coordinate
    if x < 0 or y < 0:
        return False
    # If out of bounds
    if x >= m_dim or y >= n_dim:
        return False
    # If tile is occupied by another player
    if grid_data[x][y] in ['0', '1', '3']:
        return False
    # Valid tile conditions
    if grid_data[x][y] in ['.', ',', 'o', '=', '2']:
        return True
    
# Gives us a 2d array with cost and parent node
def grid_nodes(grid):
    new_grid = []
    for x in range(len(grid)):
        row = []
        for y in range(len(grid)):
            # (cost, parent)
            if grid[x][y] == '0':
                row.append([0, (x,y)])
            else:
                row.append([INF, None])
        new_grid.append(row)
    return(new_grid)

# Gets all adjacent neighbor tiles and their associated cost scalar
def get_neighbors(node):
    node_coord = coord_dict[node]
    neighbors = []
    all_dirs = directions + directions_diag
    # Check all adjacent tiles
    for adj in all_dirs:
        # Gives indicies for neighbor
        x = node_coord[0] + adj[0]
        y = node_coord[1] + adj[1]
        
        if (valid_position(x, y)):
            neighbor_key = val_list.index((x, y))
            cost_scalar = 1
            if adj in directions_diag:
                cost_scalar = 1.5
            neighbors.append([neighbor_key, cost_scalar])
            
    return(neighbors)

def dijkstra(src, dest):
    src_key = val_list.index(src)
    dest_key = val_list.index(dest)
    #print(src_key, "->", dest_key)
    
    visited = []
    shortest_path = {v:INF for v in range(m_dim * n_dim)}
    shortest_path[src_key] = 0
    pred = {v:() for v in range(m_dim * n_dim)}
    queue = []
    queue.append((0, src_key))
    while queue != None:
        cost, current = heapq.heappop(queue)
        if current == dest_key:
            return(shortest_path, pred)
        visited.append(current)
        
        neighbors = get_neighbors(current) # <- [neighbor_key, cost_scalar]
        for key, scalar in neighbors:
            neighbor_cost = cost + (cost_dict[value_dict[key]] * scalar)
            if neighbor_cost < shortest_path[key]:
                shortest_path[key] = neighbor_cost
                queue.append((neighbor_cost, key))
                pred[key] = coord_dict[current]
                
def display_results(src, dest, shortest_path, pred):
    dest_key = val_list.index(dest)
    path = []
    tile = pred[dest_key]
    while tile != src:
        path.append(tile)
        tile = pred[val_list.index(tile)]
    path.append(src)
    path.reverse()
    new_grid = grid_data
    for x, y in enumerate(path):
        new_grid[y[0]][y[1]] = '*'
    total_cost = shortest_path[dest_key]
    print_grid(new_grid, total_cost)
    return
    
def main():
    begin = time.time() 
    src = find_item(grid_data, '0')
    dest = find_item(grid_data, '2')
    shortest_path, pred = dijkstra(src, dest)
    display_results(src, dest, shortest_path, pred)
    end = time.time() 
    print("time taken in microseconds:", (end - begin) * 1000000)
    return

if __name__ == "__main__":
    main()