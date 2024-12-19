import numpy as np
import heapq

directions = [[0,1],[0,-1],[1,0],[-1,0]]
class Grid:
    def __init__(self, grid_size_x, grid_size_y,memory_space_str):
        self.len_x = grid_size_x
        self.len_y = grid_size_y
        self.corrupted_pointer = 0

        memory_space = []
        for line in memory_space_str.split('\n'):
            memory_space.append([int(i) for i in line.split(',')])

        self.memory_space = memory_space

        self.corrupted_space = np.zeros((self.len_x, self.len_y))
        
        self.start = (0,0)
        self.finish = (grid_size_x - 1, grid_size_y - 1)

    def fill_n_bit_of_corrupted_space(self, n):
        for i in range(n):
            if self.corrupted_pointer >= len(self.memory_space):
                break
            next_space = self.memory_space[self.corrupted_pointer]

            self.corrupted_space[next_space[0], next_space[1]] = 1
            self.corrupted_pointer += 1


    def legal(self, x,y,visited):
        if (x < 0) or  (y < 0) or (x >= self.len_x) or (y >= self.len_y):
            return False
                
        if (x,y) in visited:
            return False
        
        if (self.corrupted_space[x,y] == 1):
            return False
        

        
        return True
    

    def dijkstra(self):
            visited = set()
            queue = [(0,self.start[0], self.start[1])]

            while queue:
                cost, x, y = heapq.heappop(queue)

                if (x,y) == self.finish:
                    return cost

                
                if (x,y) in visited:
                    continue
                visited.add((x,y))

                
                for d in directions:
                    dx,dy = d
                    nx, ny = x + dx, y + dy
                    if self.legal(nx,ny,visited): #ofc no wall problems, just for checking if we've visited it.
                        heapq.heappush(queue, (cost + 1, nx, ny))



            return -1


def part1(filepath, grid_size):
    with open(filepath,'r') as file:
        grid = Grid(grid_size,grid_size,file.read())

    if grid_size == 7:
        grid.fill_n_bit_of_corrupted_space(12)
    elif grid_size == 71:
        grid.fill_n_bit_of_corrupted_space(1024)
    cost = grid.dijkstra()
    print("Part 1: %s" %cost)

def part2(filepath, grid_size):
    with open(filepath,'r') as file:
        grid = Grid(grid_size,grid_size,file.read())

    if grid_size == 7:
        grid.fill_n_bit_of_corrupted_space(12)
    elif grid_size == 71:
        grid.fill_n_bit_of_corrupted_space(1024)

    cost = 0
    while (cost != -1):
        grid.fill_n_bit_of_corrupted_space(1)
        cost = grid.dijkstra()

    print("Part 2: %s" %grid.memory_space[grid.corrupted_pointer - 1])

example = part1("./Day18ExampleInput.txt", 7)
real = part1("./Day18Input.txt", 71)

example = part2("./Day18ExampleInput.txt", 7)
real = part2("./Day18Input.txt", 71)