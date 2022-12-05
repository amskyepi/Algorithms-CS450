# Project 5: Tactical Path-Finding
# Author: Amethyst Skye
# Language: Python

import sys
import numpy as np
import heapq
import time
from math import inf

cost_dict = {
    '.': 1, # Road 
    ',': 2, # Grass
    'o': 3, # Rocks
    '=': 50, # Wall
    '0': inf, # Source
    '1': inf, # Friendly player
    '2': 0, # Destination
    '3': inf # Enemy player
    }

# All directions checked during path finding
directions = [(0,-1), (0,1), (-1,0), (1,0)]
directions_diag = [(-1,-1), (-1,1), (1,-1), (1,1)]

# Simple printing function
def print_grid(grid, cost):
    for i in grid:
        print(" ".join(i))
    print("Total cost:", cost)
    return

# Find position of starting point
def find_item(grid, item):
    return (tuple(np.argwhere(grid == item).flatten()))

# Determine if we can make a move at (x,y)
def valid_position(grid, x, y):
    m_dim = len(grid)
    n_dim = len(grid[0])
    # If negative coordinate
    if x < 0 or y < 0:
        return False
    # If out of bounds
    if x >= m_dim or y >= n_dim:
        return False
    # If tile is occupied by another player
    if grid[x][y] in {'0', '1', '3'}:
        return False
    # Valid tile conditions
    if grid[x][y] in {'.', ',', 'o', '=', '2'}:
        return True

# Dijkstra's algorithm
def dijkstra(grid, src, dest):
    m_dim = len(grid)
    n_dim = len(grid[0])
    visited = set()
    shortest_path = [[inf for v in range(m_dim)] for v in range(n_dim)]
    shortest_path[src[0]][src[1]] = 0
    pred = [[() for v in range(m_dim)] for v in range(n_dim)]
    queue = []
    heapq.heappush(queue, (0, src))

    while queue != None:
        # get tile with smallest cost
        cost, current = heapq.heappop(queue)
        
        # Check if we reached destination
        if current == dest:
            return(shortest_path, pred)
        visited.add(current)
        
        # Gives indicies for neighbor
        all_dirs = directions + directions_diag
        neighbors = []
        for adj in all_dirs:
            x = current[0] + adj[0]
            y = current[1] + adj[1]
            if (valid_position(grid, x, y)):
                cost_scalar = 1
                if adj in directions_diag:
                    cost_scalar = 1.5
                neighbors.append([(x, y), cost_scalar])
                
        # Check if we are taking shortest path
        for coord, scalar in neighbors:
            neigh_x = coord[0]
            neigh_y = coord[1]
            neighbor_cost = cost + (cost_dict[grid[neigh_x][neigh_y]] * scalar)
            if neighbor_cost < shortest_path[neigh_x][neigh_y]:
                shortest_path[neigh_x][neigh_y] = neighbor_cost
                heapq.heappush(queue,(neighbor_cost, coord))
                pred[neigh_x][neigh_y] = current

# Gets path and total cost for shortest path              
def get_results(grid, src, dest, shortest_path, pred):
    path = []
    tile = pred[dest[0]][dest[1]]
    new_grid = grid
    while tile != src:
        path.append(tile)
        tile = pred[tile[0]][tile[1]]
    path.reverse()
    for x, y in enumerate(path):
        new_grid[y[0]][y[1]] = '*'
    total_cost = shortest_path[dest[0]][dest[1]]
    return (new_grid, total_cost)

def main():
    begin = time.time() 
    grid_data = np.loadtxt(sys.stdin, dtype=np.str_)
    src = find_item(grid_data, '0')
    dest = find_item(grid_data, '2')
    shortest_path, pred = dijkstra(grid_data, src, dest)
    new_grid, total_cost = get_results(grid_data, src, dest, shortest_path, pred)
    print_grid(new_grid, total_cost)
    end = time.time()
    print("time taken in microseconds:", (end - begin) * 1000000)
    return

if __name__ == "__main__":
    main()