import sys
import numpy as np
import heapq

# Simple printing function
def print_grid(grid):
    for i in grid:
        for j in i:
            print(j, end = ' ')
        print()
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
def find_src(grid):
    return (tuple(np.argwhere(grid == '0').flatten()))

# Determine if we can make a move at (x,y)
def valid_position(grid, x, y):
    # If negative coordinate
    if x < 0 or y < 0:
        return False
    # If out of bounds
    if x >= len(grid) or y >= len(grid[0]):
        return False
    # If tile is occupied by another player
    if grid[x][y] in ['1', '3']:
        return False
    # Valid tile conditions
    if grid[x][y] in ['.', ',', 'o', '=', '2']:
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
                row.append([9999999, None])
        new_grid.append(row)
    return(new_grid)

def dijkstras(grid, nodes, src, queue):
    visited = set()
                # North, South, West, East, Northwest, Northeast, Southwest, Southeast
    directions = [(0,-1), (0,1), (-1,0), (1,0), (-1,-1), (-1,1), (1,-1), (1,1)]
    # initialize queue
    if queue == None:
        queue = [nodes[src[0]][src[1]]]
        visited.add(src)
        
    while queue != None:
        # Check all neighbors for possible positions
        steps, tmp = queue.pop()
        for dir in directions:
            new_x = tmp[0] + dir[0]
            new_y = tmp[1] + dir[1]
            if valid_position(grid, new_x, new_y) and (new_x, new_y) not in visited:
                tile = grid[new_x][new_y]
                if tile == '.':
                    cost = 1
                if tile == ',':
                    cost = 2
                if tile == 'o':
                    cost = 3
                if tile == '=':
                    cost = 50
                nodes[new_x][new_y][0] = cost + steps
                nodes[new_x][new_y][1] = src
                queue.append(nodes[new_x][new_y])
                visited.add((new_x, new_y))
        print(queue)
    return 
    
def main():
    cost_dict = {
    '.': 1, # Road 
    ',': 2, # Grass
    'o': 3, # Rocks
    '=': 50, # Wall
    '0': 'S', # Source
    '1': 'X', # Friendly player
    '2': 'D', # Destination
    '3': 'X' # Enemy player
    }

    grid_data = np.loadtxt(sys.stdin, dtype=np.str_)
    #cost_grid = replace_values(grid_data, cost_dict)
    src = find_src(grid_data)
    nodes = grid_nodes(grid_data)
    dijkstras(grid_data, nodes, src, None)
    return

if __name__ == "__main__":
    main()