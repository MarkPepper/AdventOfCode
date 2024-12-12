import numpy as np

directions = [(1,0),(-1,0),(0,1),(0,-1)]

def node_neighbours(node, val, grid):
    found_neighbours = []
    for direction in directions:
        if (node[1] + direction[1] >= 0) and (node[1] + direction[1] < len(grid)) and (node[0] + direction[0] >= 0) and (node[0] + direction[0] < len(grid[0])):
            if grid[node[1] + direction[1]][node[0] + direction[0]] == val:
                found_neighbours.append((node[0] + direction[0], node[1] + direction[1]))
    return found_neighbours


def find_region_covered(starting_x, starting_y, grid):
    nodes_searched = [(starting_x,starting_y)]
    nodes_to_search = []
    val = grid[starting_y][starting_x]

    nodes_to_search = node_neighbours((starting_x, starting_y), val, grid)

    while (len(nodes_to_search) > 0):
        node_to_search = nodes_to_search.pop()
        neighbours = node_neighbours(node_to_search, val, grid)
        for n in neighbours:
            if (not n in nodes_searched):
                nodes_to_search.append(n)
        nodes_searched.append(node_to_search)
        nodes_to_search = list(set(nodes_to_search))

    return nodes_searched

def get_number_of_sides(perimeter,grid):
    search_entries = []
    entries_to_search = perimeter.copy()
    sides = 0
    while (len(entries_to_search) > 0):
        current_search_point = entries_to_search.pop()
        search_entries.append(current_search_point)
        found_neighbours = False
        rend = 0
        lend = 0
        for i in [-1,1]:
            if ((current_search_point[0] + i, current_search_point[1]) in perimeter):
                #we have a horizontal
                found_neighbours = True
                rend = 0
                if (current_search_point[0] == len(grid[0]) - 1):
                    rend = 0
                else:
                    for j in range(len(grid[0])):
                        if ((current_search_point[0] + j, current_search_point[1]) in perimeter):
                            rend = j
                        else:
                            break
                if (current_search_point[0] == 0):
                    lend = 0
                else:
                    for j in range(len(grid[0])):
                        if ((current_search_point[0] - j, current_search_point[1]) in perimeter):
                            lend = -j
                        else:
                            break
                
                for j in range(lend,rend+1):
                    search_entries.append((current_search_point[0] + j, current_search_point[1]))
                entries_to_search = [i for i in entries_to_search if (not i in search_entries)]
                sides += 1
                break
            elif ((current_search_point[0], current_search_point[1] + i) in perimeter):
                #we have a vertical
                found_neighbours = True
                rend = 0
                if (current_search_point[1] == len(grid) - 1):
                    rend = 0
                else:
                    for j in range(len(grid)):
                        if ((current_search_point[0], current_search_point[1] + j) in perimeter):
                            rend = j
                        else:
                            break
                if (current_search_point[1] == 0):
                    lend = 0
                else:
                    for j in range(len(grid)):
                        if ((current_search_point[0], current_search_point[1] - j) in perimeter):
                            lend = -j
                        else:
                            break
                

                for j in range(lend,rend+1):
                    search_entries.append((current_search_point[0], current_search_point[1] + j))
                entries_to_search = [i for i in entries_to_search if (not i in search_entries)]
                sides += 1
                break
        if (not found_neighbours):
            sides += 1


    return sides

def calculate_area_perimeter_and_number_of_sides(
        region,
        grid
):
    area = len(region)
    perimeter = []
    old_perim_sum = 0
    for r in region:
        for direction in directions:
            if not ((r[0] + direction[0], r[1] + direction[1]) in region):
                old_perim_sum += 1
                perimeter.append((r[0] + (0.1*direction[0]), r[1] + (0.1*direction[1])))


    sides = get_number_of_sides(perimeter,grid)


    return area,len(perimeter),sides


def aoc12():
    sum1 = 0
    sum2 = 0
    filename = "./Day12Input.txt"
    grid = []
    used_locations = []
    with open(filename, 'r') as file:
        for line in file:
            grid.append([i for i in line if i != "\n"])

    for i in range(len(grid[0])):
        for j in range(len(grid[1])):
            if not (i,j) in used_locations:
                this_area = find_region_covered(i,j,grid)
                area,perimeter,sides = calculate_area_perimeter_and_number_of_sides(this_area,grid)
                used_locations += this_area
                sum1 += area * perimeter
                sum2 += area * sides

    print("Part 1: %s" %sum1)
    print("Part 2: %s" %sum2)


aoc12()