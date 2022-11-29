def print_grid(grid):
    for i in grid:
        for j in i:
            print(j, end = ' ')
        print()
    return

def file_to_grid(file_name):
    with open(file_name) as file:
        grid = [i.split() for i in file.readlines()]
    return(grid)

def replace_values(grid, dict):
    for i, row in enumerate(grid):
        grid[i] = [dict[x] for x in row]
    return(grid)

def main():
    cost_dict = {
    '.': 1, # Road 
    ',': 2, # Grass
    'o': 3, # Rocks
    '=': 50, # Wall
    '0': 'C', # Controlled character
    '1': 'F', # Friendly character
    '2': 'T', # Target character
    '3': 'E'} # Enemy character

    grid_data = file_to_grid('data/sample_input.txt')
    grid_data = replace_values(grid_data, cost_dict)
    print_grid(grid_data)
    return

if __name__ == "__main__":
    main()






